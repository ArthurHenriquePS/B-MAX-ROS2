"""vision_node — lê a webcam e publica a emoção detectada em /emotion.

Pipeline: V4L2 (MJPG) -> MediaPipe FaceLandmaker (blendshapes) ->
regras de emoção -> EMA + histerese -> publica na mudança (+ heartbeat).

Parâmetros:
  camera_index (int)  índice da webcam (default 0)
  width, height (int) resolução de captura (default 640x480)
  process_hz (float) frequência de inferência (default 10.0)
  rate_hz (float) heartbeat de publicação (default 2.0)
  enabled (bool) Falsee = stub 'neutral' (default False)
  model_path (str) caminho do face_landmarker.task
  ema_alpha (float) suavização dos scores, 0-1 (default 0.4)
  hold_frames (int) leituras consecutivas p/trocar de emomção (defaut 3)
  min_score (float) score mínimo para sair de 'neutral' (default 0.35)
  no_face_timeout (float) segundos sem rosto atée publicar 'none' (default  1.0)
"""
from __future__ import annotations

import threading
import time
from pathlib import Path

import cv2
import numpy as np
import rclpy
from rclpy.node import Node

import mediapipe as mp 
from mediapipe.tasks import python as mp_python
from mediapipe.tasks.python import vision as mp_vision

from baymax_interfaces.msg import Emotion

LABELS = ("neutral", "happy", "sad", "surprised", "angry", "pain")

def _avg(b: dict, *names: str) -> float:
    return sum(b.get(n, 0.0) for n in names) / len(names)

def _clip(x:float) -> float:
    return float(min(1.0, max(0.0, x)))

def score_emotions(b: dict) -> np.ndarray:
    """ Convertendo blendshapes em scores [0.1] por emoção

    --> PARA BEATRIZ DO FUTURO: Calibrar quando tiver com acesso a camera na máquina
    com o rosto.
    """
    smile = _avg(b, "mouthSmileLeft", "mouthSmileRight")
    frown = _avg(b, "mouthFrownLeft", "mouthFrownRight")
    brow_down = _avg(b, "browDownLeft", "browDownRight")
    brow_up = b.get("browInnerUp", 0.0)
    brow_outer = _avg(b, "browOuterUpLeft", "browOuterUpRight")
    squint = _avg(b, "eyeSquintLeft", "eyeSquintRight")
    wide = _avg(b, "eyeWideLeft", "eyeWideRight")
    blink = _avg(b, "eyeBlinkLeft", "eyeBlinkRight")
    jaw = b.get("jawOpen", 0.0)
    sneer = _avg(b, "noseSneerLeft", "noseSneerRight")
    press = _avg(b, "mouthPressLeft", "mouthPressRight")
    stretch = _avg(b, "mouthStretchLeft", "mouthStretchRight")

    happy = _clip(smile * 1.8)
    sad = _clip(frown * 1.5 + brow_up  * 1.0 - smile)
    surprised = _clip(jaw * 1.2 + wide * 1.0 + (brow_outer) * 0.5 - brow_down)
    angry = _clip(brow_down * 1.4 + sneer * 0.8 + press * 0.8 - smile)
    pain = _clip(squint * 1.0 + brow_down * 0.6 + stretch * 0.8 + blink * 0.2 - smile)

    return np.array([0.0, happy, sad, surprised, angry, pain], dtype=np.float32)

class VisionNode(Node):
    def __init__(self):
        super().__init__("vision_node")
        self.declare_parameter("camera_index", 0)
        self.declare_parameter("width", 640)
        self.declare_parameter("height", 480)
        self.declare_parameter("process_hz", 10.0)
        self.declare_parameter("rate_hz", 2.0)
        self.declare_parameter("enabled", False)
        self.declare_parameter("model_path", str(Path.home() / "baymax_ws/models/face_landmarker.task"))
        self.declare_parameter("ema_alpha", 0.4)
        self.declare_parameter("hold_frames", 3)
        self.declare_parameter("min_score", 0.35)
        self.declare_parameter("no_face_timeout", 1.0)

        p = lambda n: self.get_parameter(n).value
        self.enabled = bool(p("enabled"))
        self.cam_idx = int(p("camera_index"))
        self.width, self.height = int(p("width")), int(p("height"))
        self.process_hz = max(float(p("process_hz")), 1.0)
        self.model_path = str(p("model_path"))
        self.alpha = float(p("ema_alpha"))
        self.hold = max(int(p("hold_frames")), 1)
        self.min_score = float(p("min_score"))
        self.no_face_timeout = float(p("no_face_timeout"))

        self.pub = self.create_publisher(Emotion, "/emotion", 10)
        rate = float(p("rate_hz"))
        self.timer = self.create_timer(1.0 / max(rate, 0.1), self._heartbeat)

        self._lock = threading.Lock()
        self._label, self._conf = "neutral", 1.0
        self._stop = threading.Event()
        self._thread: threading.Thread | None = None

        if self.enabled:
            if not Path(self.model_path).is_file():
                self.get_logger().error(f"modelo não encontrado: {self.model_path}")
                self.enabled = False
            else:
                self._thread = threading.Thread(target=self._worker, daemon=True)
                self._thread.start()
        self.get_logger().info(
            f"vision_node pronto ({'real' if self.enabled else 'stub'})."
        )

    # ---------- publicação ----------
    def _publish(self, label: str, conf: float):
        msg = Emotion()
        msg.label = label
        msg.confidence = float(conf)
        msg.stamp = self.get_clock().now().to_msg()
        self.pub.publish(msg)

    def _heartbeat(self):
        with self._lock:
            label, conf = self._label, self._conf
        self._publish(label, conf)

    # ---------- worker: câmera + inferência ----------
    def _open_camera(self) -> cv2.VideoCapture | None:
        cap = cv2.VideoCapture(self.cam_idx, cv2.CAP_V4L2)
        cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*"MJPG"))  # menos CPU/USB
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, self.width)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, self.height)
        cap.set(cv2.CAP_PROP_FPS, 30)
        cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
        if not cap.isOpened():
            self.get_logger().error(f"não abriu a câmera {self.cam_idx}")
            return None
        return cap

    def _worker(self):
        options = mp_vision.FaceLandmarkerOptions(
            base_options=mp_python.BaseOptions(model_asset_path=self.model_path),
            running_mode=mp_vision.RunningMode.VIDEO,
            num_faces=1,
            output_face_blendshapes=True,
            output_facial_transformation_matrixes=False,
        )
        landmarker = mp_vision.FaceLandmarker.create_from_options(options)
        cap = self._open_camera()
        if cap is None:
            return

        period = 1.0 / self.process_hz
        last_proc = 0.0
        last_face = time.monotonic()
        ema = np.zeros(len(LABELS), dtype=np.float32)
        cand, cand_count = "neutral", 0
        cur = "neutral"
        t_acc, n_acc = 0.0, 0

        while not self._stop.is_set():
            if not cap.grab():
                time.sleep(0.01)
                continue
            now = time.monotonic()
            if now - last_proc < period:
                continue
            last_proc = now
            ok, frame = cap.retrieve()
            if not ok:
                continue

            t0 = time.perf_counter()
            rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            mp_img = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb)
            res = landmarker.detect_for_video(mp_img, int(now * 1000))

            if res.face_blendshapes:
                last_face = now
                b = {c.category_name: c.score for c in res.face_blendshapes[0]}
                scores = score_emotions(b)
                ema = self.alpha * scores + (1.0 - self.alpha) * ema
                top = int(np.argmax(ema[1:])) + 1
                if ema[top] < self.min_score:
                    label, conf = "neutral", float(1.0 - ema[top])
                else:
                    label, conf = LABELS[top], float(ema[top])
            elif now - last_face > self.no_face_timeout:
                label, conf = "none", 1.0
                ema[:] = 0.0
            else:
                continue

            if label == cand:
                cand_count += 1
            else:
                cand, cand_count = label, 1
            if cand_count >= self.hold and label != cur:
                cur = label
                with self._lock:
                    self._label, self._conf = label, conf
                self._publish(label, conf)  # publica na hora, sem esperar o timer
            else:
                with self._lock:
                    if self._label == label:
                        self._conf = conf

            t_acc += time.perf_counter() - t0
            n_acc += 1
            if n_acc >= int(self.process_hz * 5):
                self.get_logger().info(f"inferência média: {1000 * t_acc / n_acc:.1f} ms")
                t_acc, n_acc = 0.0, 0

        cap.release()
        landmarker.close()

    def destroy_node(self):
        self._stop.set()
        if self._thread:
            self._thread.join(timeout=2.0)
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = VisionNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
