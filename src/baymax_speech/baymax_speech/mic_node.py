"""mic_node — captura do microfone + VAD, segmenta falas e publica WAV p/ o STT.

Fluxo: captura contínua (sounddevice) → VAD (webrtcvad) detecta início/fim de fala →
grava um WAV mono 16kHz do trecho (utterance) → publica o caminho em /utterance_wav.
O stt_node transcreve esse WAV.

Modos de duplex (parâmetro `duplex`):
  - "half": enquanto o Baymax fala (/speaking == true), a captura é IGNORADA.
            Evita o robô ouvir a própria voz. Simples e robusto, sem servidor de som.
  - "full": captura sempre, mesmo enquanto fala. Permite interromper o Baymax, mas
            exige mixagem/eco adequados (idealmente PipeWire + cancelamento de eco).

Áudio: whisper exige 16000 Hz mono. O device pode operar em 44100/48000; capturamos
na taxa nativa e reamostramos para 16000 (decimação simples via numpy).

Parâmetros:
  device (int)        índice do dispositivo de entrada (sounddevice). -1 = default.
  capture_rate (int)  taxa nativa de captura (default 48000)
  vad_aggressiveness  0..3 (default 2; maior = corta mais ruído)
  silence_ms (int)    silêncio que encerra a fala (default 700)
  min_speech_ms (int) duração mínima de fala p/ valer (default 300)
  duplex (str)        "half" ou "full" (default "half")
  out_dir (str)       onde gravar os WAVs temporários (default /tmp/baymax_utt)
"""
from __future__ import annotations

import os
import time
import wave

import numpy as np
import rclpy
from rclpy.node import Node
from std_msgs.msg import Bool, String

try:
    import sounddevice as sd
    import webrtcvad
except ImportError:  # pragma: no cover
    sd = None
    webrtcvad = None

WHISPER_RATE = 16000
FRAME_MS = 30  # webrtcvad aceita 10/20/30 ms


class MicNode(Node):
    def __init__(self):
        super().__init__("mic_node")
        self.declare_parameter("device", -1)
        self.declare_parameter("capture_rate", 48000)
        self.declare_parameter("vad_aggressiveness", 2)
        self.declare_parameter("silence_ms", 700)
        self.declare_parameter("min_speech_ms", 300)
        self.declare_parameter("duplex", "half")
        self.declare_parameter("out_dir", "/tmp/baymax_utt")

        self.device = int(self.get_parameter("device").value)
        self.capture_rate = int(self.get_parameter("capture_rate").value)
        self.vad_aggr = int(self.get_parameter("vad_aggressiveness").value)
        self.silence_ms = int(self.get_parameter("silence_ms").value)
        self.min_speech_ms = int(self.get_parameter("min_speech_ms").value)
        self.duplex = str(self.get_parameter("duplex").value).lower()
        self.out_dir = self.get_parameter("out_dir").value
        os.makedirs(self.out_dir, exist_ok=True)

        self.pub = self.create_publisher(String, "/utterance_wav", 10)
        self.create_subscription(Bool, "/speaking", self.on_speaking, 10)

        self._baymax_speaking = False
        self._voiced = bytearray()      # PCM 16kHz acumulado da fala atual
        self._silence_run = 0.0         # ms de silêncio seguido
        self._in_speech = False
        self._utt_count = 0

        if sd is None or webrtcvad is None:
            self.get_logger().warning(
                "sounddevice/webrtcvad indisponível (venv?). mic_node em stub."
            )
            return

        self.vad = webrtcvad.Vad(self.vad_aggr)
        self._decim = max(1, self.capture_rate // WHISPER_RATE)
        self._frame_samples = int(self.capture_rate * FRAME_MS / 1000)

        try:
            self.stream = sd.InputStream(
                device=(self.device if self.device >= 0 else None),
                channels=1,
                samplerate=self.capture_rate,
                dtype="int16",
                blocksize=self._frame_samples,
                callback=self._on_audio_block,
            )
            self.stream.start()
            self.get_logger().info(
                f"mic_node capturando (device={self.device}, "
                f"rate={self.capture_rate}->16k, duplex={self.duplex})."
            )
        except Exception as exc:  # noqa: BLE001
            self.get_logger().error(f"falha ao abrir microfone: {exc}. Stub.")

    def on_speaking(self, msg: Bool):
        self._baymax_speaking = bool(msg.data)
        if self.duplex == "half" and self._baymax_speaking:
            # Descarta qualquer fala em formação (não transcrever a voz do Baymax)
            self._reset_utterance()

    def _reset_utterance(self):
        self._voiced = bytearray()
        self._silence_run = 0.0
        self._in_speech = False

    def _on_audio_block(self, indata, frames, time_info, status):  # noqa: ARG002
        # Callback do sounddevice (thread de áudio). Mantém leve.
        if self.duplex == "half" and self._baymax_speaking:
            return  # anti-eco: ignora captura enquanto o Baymax fala

        pcm_native = indata[:, 0] if indata.ndim > 1 else indata
        # Reamostra p/ 16kHz por decimação simples
        pcm16k = pcm_native[:: self._decim].astype(np.int16)
        frame_bytes = pcm16k.tobytes()

        # webrtcvad precisa de frames de 10/20/30 ms exatos a 16kHz
        want = int(WHISPER_RATE * FRAME_MS / 1000)  # 480 amostras
        if len(pcm16k) < want:
            return
        frame = pcm16k[:want].tobytes()

        try:
            is_speech = self.vad.is_speech(frame, WHISPER_RATE)
        except Exception:  # noqa: BLE001
            return

        if is_speech:
            self._voiced.extend(frame_bytes)
            self._silence_run = 0.0
            self._in_speech = True
        elif self._in_speech:
            self._voiced.extend(frame_bytes)
            self._silence_run += FRAME_MS
            if self._silence_run >= self.silence_ms:
                self._finish_utterance()

    def _finish_utterance(self):
        voiced = bytes(self._voiced)
        self._reset_utterance()
        dur_ms = (len(voiced) / 2) / WHISPER_RATE * 1000
        if dur_ms < self.min_speech_ms:
            return  # muito curto, provável ruído
        path = os.path.join(self.out_dir, f"utt_{self._utt_count:04d}.wav")
        self._utt_count += 1
        try:
            with wave.open(path, "wb") as wf:
                wf.setnchannels(1)
                wf.setsampwidth(2)
                wf.setframerate(WHISPER_RATE)
                wf.writeframes(voiced)
        except Exception as exc:  # noqa: BLE001
            self.get_logger().error(f"falha ao gravar WAV: {exc}")
            return
        self.get_logger().info(f"fala detectada ({dur_ms:.0f}ms) -> {path}")
        self.pub.publish(String(data=path))

    def destroy_node(self):
        try:
            if hasattr(self, "stream"):
                self.stream.stop()
                self.stream.close()
        except Exception:  # noqa: BLE001
            pass
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = MicNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
