"""brain_node — o coração cognitivo do Baymax.

Junta o que o usuário disse (/speech_text) com a emoção detectada (/emotion),
monta um prompt com a persona do Baymax, chama o LLM local (Qwen2.5 3B via Ollama)
e divide a resposta em:
  - fala  -> /tts_text   (std_msgs/String)
  - ações -> /motion_cmd (baymax_interfaces/MotionCommand) via tags [HEAD:nod] etc.
  - estado-> /emotion_state (std_msgs/String)

100% local: fala com o Ollama em http://127.0.0.1:11434, sem internet.

Parâmetros ROS:
  model (str)          nome do modelo no Ollama (default qwen2.5:3b)
  ollama_url (str)     endpoint (default http://127.0.0.1:11434)
  temperature (float)  (default 0.6)
  offline_fallback (bool) se Ollama indisponível, usa respostas-regra simples (default True)
  keep_alive (str)     "-1" mantém o modelo na RAM (elimina cold start ~2s). Default "-1"
  num_predict (int)    teto de tokens gerados (Baymax fala curto). Default 80
  num_thread (int)     threads do LLM; usar núcleos físicos (8 no 8845HS). Default 8
  stream (bool)        publica fala por frase, reduzindo latência percebida. Default True
"""
from __future__ import annotations

import json
import re
import threading
import time
import urllib.request
import urllib.error

import rclpy
from rclpy.node import Node
from std_msgs.msg import String

from baymax_interfaces.msg import Emotion, MotionCommand

PERSONA = (
    "Você é Baymax, um robô-enfermeiro pessoal gentil, calmo e atencioso do filme "
    "Big Hero 6. Fale em português do Brasil, com frases curtas, tom tranquilizador e "
    "empático. Pergunte sobre o bem-estar da pessoa. Nunca seja agressivo. "
    "Você pode mover a cabeça e o braço direito. Quando fizer sentido, inclua UMA tag de "
    "ação no final da fala, escolhendo entre: [HEAD:nod] [HEAD:shake] [HEAD:tilt] "
    "[ARM:wave] [ARM:point] [ARM:fistbump]. Use a tag só quando natural."
)

# Mapeia tags [GRUPO:nome] -> (target, kind, name)
TAG_RE = re.compile(r"\[(HEAD|ARM):([a-z_]+)\]", re.IGNORECASE)


class BrainNode(Node):
    def __init__(self):
        super().__init__("brain_node")
        self.declare_parameter("model", "qwen2.5:3b")
        self.declare_parameter("ollama_url", "http://127.0.0.1:11434")
        self.declare_parameter("temperature", 0.6)
        self.declare_parameter("offline_fallback", True)
        # Otimizações de latência:
        self.declare_parameter("keep_alive", -1)        # -1 = nunca descarrega (mata cold start ~2s)
        self.declare_parameter("num_predict", 80)       # teto de tokens (Baymax fala curto)
        self.declare_parameter("num_thread", 8)         # núcleos físicos do Ryzen 8845HS
        self.declare_parameter("stream", True)          # publica por frase, não espera a resposta toda
        self.declare_parameter("warmup", True)          # aquece o modelo no boot (mata o 1º cold start)

        self.model = self.get_parameter("model").value
        self.ollama_url = self.get_parameter("ollama_url").value.rstrip("/")
        self.temperature = float(self.get_parameter("temperature").value)
        self.offline_fallback = bool(self.get_parameter("offline_fallback").value)
        self.keep_alive = int(self.get_parameter("keep_alive").value)
        self.num_predict = int(self.get_parameter("num_predict").value)
        self.num_thread = int(self.get_parameter("num_thread").value)
        self.stream = bool(self.get_parameter("stream").value)
        self.warmup = bool(self.get_parameter("warmup").value)

        self.last_emotion = "neutral"

        self.tts_pub = self.create_publisher(String, "/tts_text", 10)
        self.motion_pub = self.create_publisher(MotionCommand, "/motion_cmd", 10)
        self.state_pub = self.create_publisher(String, "/emotion_state", 10)

        self.create_subscription(String, "/speech_text", self.on_speech, 10)
        self.create_subscription(Emotion, "/emotion", self.on_emotion, 10)

        self.get_logger().info(
            f"brain_node pronto. modelo={self.model} via {self.ollama_url}"
        )

        if self.warmup:
            # Aquece em thread separada para não bloquear a inicialização do nó.
            threading.Thread(target=self._warmup_model, daemon=True).start()

    def _warmup_model(self):
        """Carrega o modelo na RAM no boot, para a 1ª interação já sair quente."""
        payload = json.dumps({
            "model": self.model,
            "prompt": "oi",
            "stream": False,
            "keep_alive": self.keep_alive,
            "options": {"num_predict": 1, "num_thread": self.num_thread},
        }).encode("utf-8")
        req = urllib.request.Request(
            f"{self.ollama_url}/api/generate",
            data=payload,
            headers={"Content-Type": "application/json"},
        )
        t0 = time.monotonic()
        try:
            with urllib.request.urlopen(req, timeout=120) as resp:
                data = json.loads(resp.read().decode("utf-8"))
            dt = (time.monotonic() - t0) * 1000
            load = data.get("load_duration", 0) / 1e6
            self.get_logger().info(
                f"modelo aquecido em {dt:.0f} ms (load {load:.0f} ms). "
                f"Primeira conversa já sai quente."
            )
        except (urllib.error.URLError, OSError, TimeoutError) as exc:
            self.get_logger().warning(
                f"warm-up falhou (Ollama indisponível?): {exc}. "
                f"A 1ª interação pode pagar cold start."
            )

    def on_emotion(self, msg: Emotion):
        self.last_emotion = msg.label or "neutral"

    def on_speech(self, msg: String):
        user_text = msg.data.strip()
        if not user_text:
            return
        t0 = time.monotonic()
        self.get_logger().info(f"usuário: {user_text!r} (emoção={self.last_emotion})")

        prompt = (
            f"[A pessoa parece: {self.last_emotion}]\n"
            f"Pessoa: {user_text}\n"
            f"Baymax:"
        )

        if self.stream:
            self._respond_streaming(prompt, t0)
        else:
            reply = self.query_llm(prompt)
            speech, actions = self.split_actions(reply)
            if speech:
                self.tts_pub.publish(String(data=speech))
            for (target, kind, name) in actions:
                self.publish_motion(target, kind, name)
            self.get_logger().info(f"latência total: {(time.monotonic()-t0)*1000:.0f} ms")

        self.state_pub.publish(String(data=self.last_emotion))

    # ------------------------------------------------------------------
    def _base_options(self) -> dict:
        return {
            "temperature": self.temperature,
            "num_predict": self.num_predict,
            "num_thread": self.num_thread,
        }

    def _respond_streaming(self, prompt: str, t0: float):
        """Consome o stream do Ollama e publica cada frase assim que fica pronta.

        Reduz a latência percebida: o Baymax começa a falar na primeira frase,
        sem esperar a resposta inteira ser gerada.
        """
        payload = json.dumps({
            "model": self.model,
            "system": PERSONA,
            "prompt": prompt,
            "stream": True,
            "keep_alive": self.keep_alive,
            "options": self._base_options(),
        }).encode("utf-8")
        req = urllib.request.Request(
            f"{self.ollama_url}/api/generate",
            data=payload,
            headers={"Content-Type": "application/json"},
        )

        buffer = ""
        full = ""
        first_sentence_done = False
        try:
            with urllib.request.urlopen(req, timeout=60) as resp:
                for raw in resp:
                    line = raw.decode("utf-8").strip()
                    if not line:
                        continue
                    chunk = json.loads(line)
                    piece = chunk.get("response", "")
                    buffer += piece
                    full += piece
                    # Emite frases completas (terminadas em . ! ? ou nova linha)
                    sentence, buffer = self._pop_sentence(buffer)
                    while sentence:
                        self._emit_speech_sentence(sentence)
                        if not first_sentence_done:
                            self.get_logger().info(
                                f"1ª frase em {(time.monotonic()-t0)*1000:.0f} ms"
                            )
                            first_sentence_done = True
                        sentence, buffer = self._pop_sentence(buffer)
                    if chunk.get("done"):
                        break
        except (urllib.error.URLError, OSError, TimeoutError) as exc:
            self.get_logger().warning(f"Ollama indisponível: {exc}")
            if self.offline_fallback:
                reply = self.rule_based_reply(self.last_emotion)
                speech, actions = self.split_actions(reply)
                if speech:
                    self.tts_pub.publish(String(data=speech))
                for (target, kind, name) in actions:
                    self.publish_motion(target, kind, name)
            return

        # Resto do buffer (última frase sem pontuação final)
        tail = buffer.strip()
        if tail:
            self._emit_speech_sentence(tail)

        # Ações (tags) são coletadas da resposta inteira
        _, actions = self.split_actions(full)
        for (target, kind, name) in actions:
            self.publish_motion(target, kind, name)
        self.get_logger().info(f"latência total: {(time.monotonic()-t0)*1000:.0f} ms")

    @staticmethod
    def _pop_sentence(buf: str):
        """Extrai a primeira frase completa do buffer, se houver.

        Retorna (frase_ou_None, resto_do_buffer).
        """
        for i, ch in enumerate(buf):
            if ch in ".!?\n":
                return buf[: i + 1].strip(), buf[i + 1:]
        return None, buf

    def _emit_speech_sentence(self, sentence: str):
        """Remove tags de ação da frase e publica em /tts_text."""
        speech = TAG_RE.sub("", sentence).strip()
        if speech:
            self.tts_pub.publish(String(data=speech))

    def query_llm(self, prompt: str) -> str:
        """Chamada não-streaming (usada quando stream=False)."""
        payload = json.dumps({
            "model": self.model,
            "system": PERSONA,
            "prompt": prompt,
            "stream": False,
            "keep_alive": self.keep_alive,
            "options": self._base_options(),
        }).encode("utf-8")

        req = urllib.request.Request(
            f"{self.ollama_url}/api/generate",
            data=payload,
            headers={"Content-Type": "application/json"},
        )
        try:
            with urllib.request.urlopen(req, timeout=60) as resp:
                data = json.loads(resp.read().decode("utf-8"))
                return data.get("response", "").strip()
        except (urllib.error.URLError, OSError, TimeoutError) as exc:
            self.get_logger().warning(f"Ollama indisponível: {exc}")
            if self.offline_fallback:
                return self.rule_based_reply(self.last_emotion)
            return ""

    def rule_based_reply(self, emotion: str) -> str:
        """Resposta determinística quando o LLM não está disponível."""
        table = {
            "sad": "Eu sinto muito. Estou aqui com você. [HEAD:tilt]",
            "angry": "Vamos respirar fundo juntos. Está tudo bem. [HEAD:nod]",
            "happy": "Que bom ver você bem! [ARM:wave]",
            "fearful": "Você está seguro. Eu vou cuidar de você. [HEAD:nod]",
        }
        return table.get(emotion, "Olá. Eu sou o Baymax, seu assistente pessoal de saúde. Como você está? [HEAD:nod]")

    # ------------------------------------------------------------------
    def split_actions(self, reply: str):
        """Separa o texto de fala das tags de ação [GRUPO:nome]."""
        actions = []
        for m in TAG_RE.finditer(reply):
            group = m.group(1).lower()
            name = m.group(2).lower()
            if group == "head":
                actions.append(("head", "head_gesture", name))
            else:
                actions.append(("arm", "arm_gesture", name))
        speech = TAG_RE.sub("", reply).strip()
        return speech, actions

    def publish_motion(self, target: str, kind: str, name: str):
        cmd = MotionCommand()
        cmd.target = target
        cmd.kind = kind
        cmd.value = 0
        cmd.name = name
        self.motion_pub.publish(cmd)
        self.get_logger().info(f"ação -> {target}/{name}")


def main(args=None):
    rclpy.init(args=args)
    node = BrainNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
