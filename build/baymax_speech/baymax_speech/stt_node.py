"""stt_node — transcreve fala em texto com whisper.cpp (pywhispercpp), em CPU.

Arquitetura: o mic_node (passo 3) segmenta a fala por VAD e grava um WAV mono 16kHz
de cada trecho (utterance), publicando o CAMINHO do arquivo em /utterance_wav
(std_msgs/String). O stt_node transcreve esse arquivo e publica o texto em /speech_text.

Por que caminho de arquivo e não áudio cru no tópico: evita serializar PCM pesado no
DDS, desacopla captura de transcrição, e o whisper lê WAV direto.

Decisão de modelo (medido neste Host, CPU Ryzen 8845HS):
  base:  ~330ms p/ 3.3s de fala (RTF 0.10) — ESCOLHIDO (rápido, qualidade ok p/ PT-BR)
  small: ~1140ms (RTF 0.34), 3.5x mais lento sem ganho real nas frases curtas.
STT em CPU não é gargalo (RTF 0.10); a iGPU fica dedicada ao LLM.

Requisito: áudio DEVE ser 16000 Hz mono (exigência do whisper). O mic_node captura
nessa taxa; utilitários externos devem reamostrar antes.

Parâmetros:
  model (str)      nome ou caminho do modelo whisper (default "base")
  models_dir (str) diretório de cache dos modelos (default models/whisper)
  language (str)   idioma (default pt)
  n_threads (int)  threads do whisper (default 8 = núcleos físicos)
"""
from __future__ import annotations

import os

import rclpy
from rclpy.node import Node
from std_msgs.msg import String

try:
    from pywhispercpp.model import Model
except ImportError:  # pragma: no cover
    Model = None


class SttNode(Node):
    def __init__(self):
        super().__init__("stt_node")
        self.declare_parameter("model", "base")
        self.declare_parameter("models_dir", "models/whisper")
        self.declare_parameter("language", "pt")
        self.declare_parameter("n_threads", 8)

        self.model_name = self.get_parameter("model").value
        self.models_dir = self.get_parameter("models_dir").value
        self.language = self.get_parameter("language").value
        self.n_threads = int(self.get_parameter("n_threads").value)

        self.model = None
        if Model is not None:
            try:
                self.model = Model(
                    self.model_name,
                    models_dir=self.models_dir,
                    n_threads=self.n_threads,
                    language=self.language,
                    print_realtime=False,
                    print_progress=False,
                )
                self.get_logger().info(
                    f"whisper carregado: modelo={self.model_name} "
                    f"lang={self.language} threads={self.n_threads}"
                )
            except Exception as exc:  # noqa: BLE001
                self.get_logger().error(
                    f"falha ao carregar whisper ({self.model_name}): {exc}. Modo stub."
                )
        else:
            self.get_logger().warning(
                "pywhispercpp indisponível (venv não ativo?). Modo stub."
            )

        self.pub = self.create_publisher(String, "/speech_text", 10)
        # Caminho de um WAV 16kHz mono de uma fala segmentada pelo mic_node/VAD
        self.sub = self.create_subscription(
            String, "/utterance_wav", self.on_utterance, 10
        )
        self.get_logger().info("stt_node pronto.")

    def on_utterance(self, msg: String):
        wav_path = msg.data.strip()
        if not wav_path:
            return
        if not os.path.isfile(wav_path):
            self.get_logger().error(f"WAV não encontrado: {wav_path}")
            return
        if self.model is None:
            self.get_logger().info(f"[stub] transcreveria {wav_path}")
            return
        try:
            segments = self.model.transcribe(wav_path)
            text = " ".join(s.text for s in segments).strip()
        except Exception as exc:  # noqa: BLE001
            self.get_logger().error(f"erro ao transcrever {wav_path}: {exc}")
            return
        if text:
            self.get_logger().info(f"transcrito: {text!r}")
            self.pub.publish(String(data=text))
        else:
            self.get_logger().debug("transcrição vazia (silêncio?)")


def main(args=None):
    rclpy.init(args=args)
    node = SttNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
