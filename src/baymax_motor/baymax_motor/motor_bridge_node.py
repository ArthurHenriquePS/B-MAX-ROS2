"""motor_bridge_node — ponte entre ROS2 e os MCUs (ESP32=cabeça, Arduino=braço).

Assina /motion_cmd (baymax_interfaces/MotionCommand), traduz cada comando para o
protocolo serial simples (ver baymax_common.serial_protocol) e envia pela UART ao
MCU correto. Lê respostas (ACK/ERR/...) e loga.

Parâmetros ROS:
  head_port (str)   porta serial da cabeça/ESP32   (default /dev/ttyUSB0)
  arm_port  (str)   porta serial do braço/Arduino  (default /dev/ttyUSB1)
  baud      (int)   baud rate (default 115200)
  simulate  (bool)  se True, não abre serial — apenas loga o que enviaria.
                    Útil para validar o grafo ROS sem hardware.
"""
from __future__ import annotations

import rclpy
from rclpy.node import Node

from baymax_common import serial_protocol as sp
from baymax_interfaces.msg import MotionCommand

try:
    import serial  # pyserial
except ImportError:  # pragma: no cover
    serial = None


class SerialLink:
    """Encapsula uma conexão serial a um MCU, com modo simulação."""

    def __init__(self, node: Node, role: sp.Mcu, port: str, baud: int, simulate: bool):
        self.node = node
        self.role = role
        self.port = port
        self.baud = baud
        self.simulate = simulate
        self.conn = None
        if not simulate:
            self._open()

    def _open(self):
        if serial is None:
            self.node.get_logger().error(
                "pyserial não disponível; forçando modo simulação para %s" % self.role.value
            )
            self.simulate = True
            return
        try:
            self.conn = serial.Serial(self.port, self.baud, timeout=0.1)
            self.node.get_logger().info(
                f"[{self.role.value}] conectado em {self.port} @ {self.baud}"
            )
        except Exception as exc:  # noqa: BLE001
            self.node.get_logger().warning(
                f"[{self.role.value}] falha ao abrir {self.port}: {exc}. "
                f"Operando em simulação até reconectar."
            )
            self.conn = None

    def send(self, line: str):
        if self.simulate or self.conn is None:
            self.node.get_logger().info(f"[SIM {self.role.value}] -> {line!r}")
            if self.conn is None and not self.simulate:
                self._open()  # tenta reconectar na próxima
            return
        try:
            self.conn.write(sp.encode(line))
            reply = self.conn.readline().decode(sp.ENCODING, errors="replace")
            if reply.strip():
                parsed = sp.parse_reply(reply)
                if parsed.type == sp.ReplyType.ERR:
                    self.node.get_logger().error(
                        f"[{self.role.value}] MCU rejeitou {line!r}: {parsed.args}"
                    )
                else:
                    self.node.get_logger().debug(
                        f"[{self.role.value}] <- {parsed.raw}"
                    )
        except Exception as exc:  # noqa: BLE001
            self.node.get_logger().warning(
                f"[{self.role.value}] erro serial: {exc}; fechando p/ reconectar."
            )
            self._safe_close()

    def _safe_close(self):
        try:
            if self.conn:
                self.conn.close()
        except Exception:  # noqa: BLE001
            pass
        self.conn = None

    def close(self):
        self._safe_close()


class MotorBridgeNode(Node):
    def __init__(self):
        super().__init__("motor_bridge_node")

        self.declare_parameter("head_port", sp.DEFAULT_HEAD_PORT)
        self.declare_parameter("arm_port", sp.DEFAULT_ARM_PORT)
        self.declare_parameter("baud", sp.BAUD_RATE)
        self.declare_parameter("simulate", True)

        head_port = self.get_parameter("head_port").value
        arm_port = self.get_parameter("arm_port").value
        baud = int(self.get_parameter("baud").value)
        simulate = bool(self.get_parameter("simulate").value)

        self.head = SerialLink(self, sp.Mcu.HEAD, head_port, baud, simulate)
        self.arm = SerialLink(self, sp.Mcu.ARM, arm_port, baud, simulate)

        self.sub = self.create_subscription(
            MotionCommand, "/motion_cmd", self.on_motion_cmd, 10
        )
        self.get_logger().info(
            f"motor_bridge_node pronto (simulate={simulate}). "
            f"cabeça={head_port}, braço={arm_port}"
        )

    def on_motion_cmd(self, msg: MotionCommand):
        """Traduz MotionCommand -> linha de protocolo e envia ao MCU correto."""
        try:
            link, line = self._translate(msg)
        except sp.ProtocolError as exc:
            self.get_logger().error(f"comando inválido {msg.kind}/{msg.value}: {exc}")
            return
        if link is None:
            self.get_logger().error(f"alvo/comando desconhecido: {msg.target}/{msg.kind}")
            return
        link.send(line)

    def _translate(self, msg: MotionCommand):
        t = msg.target.lower()
        k = msg.kind.lower()
        v = int(msg.value)

        # Comandos comuns a ambos os MCUs
        if k == "stop":
            link = self.head if t == "head" else self.arm
            return link, sp.stop()
        if k == "torque":
            link = self.head if t == "head" else self.arm
            return link, sp.set_torque(bool(v))

        if t == "head":
            if k == "pan":
                return self.head, sp.head_pan(v)
            if k == "tilt":
                return self.head, sp.head_tilt(v)
            if k == "head_gesture":
                return self.head, sp.head_gesture(msg.name)
        elif t == "arm":
            if k == "shoulder":
                return self.arm, sp.arm_shoulder(v)
            if k == "elbow":
                return self.arm, sp.arm_elbow(v)
            if k == "grip":
                return self.arm, sp.arm_grip(v)
            if k == "arm_gesture":
                return self.arm, sp.arm_gesture(msg.name)
        return None, ""

    def destroy_node(self):
        self.head.close()
        self.arm.close()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = MotorBridgeNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
