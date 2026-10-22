"""tts_node — sintetiza fala a partir de /tts_text usando Piper (voz PT-BR).

Usa a API Python do Piper (piper1-gpl) com a voz carregada UMA vez no início,
evitando o overhead de recarregar o modelo a cada frase. As frases chegam em
streaming do brain_node e são tocadas em ordem por uma fila (sem sobrepor).

Publica /speaking (Bool) para o mic_node saber quando pausar a captura
(half-duplex — evita o Baymax ouvir a própria voz).

Parâmetros:
  voice (str)         caminho do modelo .onnx (default models/pt_BR-faber-medium.onnx)
  audio_device (str)  device ALSA de saída (default plughw:1,0 = placa analógica).
                      Vazio ("") usa o default do sistema.
  player (str)        player de áudio (default aplay)
  enabled (bool)      se False, apenas loga o texto (default True)
"""
from __future__ import annotations

import queue
import subprocess
import tempfile
import threading
import wave

import rclpy
from rclpy.node import Node
from std_msgs.msg import Bool, String

try:
    from piper import PiperVoice
except ImportError:  # pragma: no cover
    PiperVoice = None


class TtsNode(Node):
    def __init__(self):
        super().__init__("tts_node")
        self.declare_parameter("voice", "models/pt_BR-faber-medium.onnx")
        self.declare_parameter("audio_device", "plughw:1,0")
        self.declare_parameter("player", "aplay")
        self.declare_parameter("enabled", True)

        self.voice_path = self.get_parameter("voice").value
        self.audio_device = self.get_parameter("audio_device").value
        self.player = self.get_parameter("player").value
        self.enabled = bool(self.get_parameter("enabled").value)

        # Carrega a voz uma vez (elimina overhead por frase)
        self.voice = None
        if self.enabled and PiperVoice is not None:
            try:
                self.voice = PiperVoice.load(self.voice_path)
                self.get_logger().info(f"voz carregada: {self.voice_path}")
            except Exception as exc:  # noqa: BLE001
                self.get_logger().error(
                    f"falha ao carregar voz {self.voice_path}: {exc}. Modo stub."
                )
        elif PiperVoice is None:
            self.get_logger().warning(
                "piper não disponível (venv não ativo?). Modo stub (só loga)."
            )

        # Fila de fala + worker em thread separada (toca em ordem)
        self._q: queue.Queue[str] = queue.Queue()
        self._worker = threading.Thread(target=self._speak_loop, daemon=True)
        self._worker.start()

        self.speaking_pub = self.create_publisher(Bool, "/speaking", 10)
        self.sub = self.create_subscription(String, "/tts_text", self.on_text, 10)
        self.get_logger().info(
            f"tts_node pronto (voz={'ok' if self.voice else 'stub'}, "
            f"device={self.audio_device or 'default'})."
        )

    def on_text(self, msg: String):
        text = msg.data.strip()
        if text:
            self._q.put(text)

    def _speak_loop(self):
        """Consome a fila e sintetiza/toca cada frase em ordem."""
        while True:
            text = self._q.get()
            try:
                self._speak_one(text)
            except Exception as exc:  # noqa: BLE001
                self.get_logger().error(f"erro no TTS: {exc}")
            finally:
                self._q.task_done()

    def _speak_one(self, text: str):
        self.get_logger().info(f"falar: {text!r}")
        if self.voice is None:
            return  # stub

        self._set_speaking(True)
        try:
            with tempfile.NamedTemporaryFile(suffix=".wav", delete=True) as wav:
                with wave.open(wav.name, "wb") as wf:
                    self.voice.synthesize_wav(text, wf)
                cmd = [self.player]
                if self.audio_device:
                    cmd += ["-D", self.audio_device]
                cmd.append(wav.name)
                subprocess.run(cmd, capture_output=True, timeout=30)
        finally:
            # Só sinaliza fim quando a fila esvaziar (não entre frases do mesmo turno)
            if self._q.empty():
                self._set_speaking(False)

    def _set_speaking(self, value: bool):
        self.speaking_pub.publish(Bool(data=value))


def main(args=None):
    rclpy.init(args=args)
    node = TtsNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
