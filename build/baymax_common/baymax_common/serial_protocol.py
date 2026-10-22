"""Protocolo serial do Baymax (v1).

Fonte única da verdade para construir e validar mensagens trocadas entre o Host
(cérebro ROS2) e os MCUs (ESP32 = cabeça, Arduino = braço direito).

Protocolo ASCII baseado em linha, terminado em '\\n'. Ver docs/SERIAL_PROTOCOL.md.

Este módulo é puro Python (sem dependências ROS) para ser testável isoladamente e
reutilizável tanto pelo nó ROS2 quanto por scripts de bancada.
"""
from __future__ import annotations

from dataclasses import dataclass
from enum import Enum

# ---------------------------------------------------------------------------
# Parâmetros físicos
# ---------------------------------------------------------------------------
BAUD_RATE = 115200
LINE_TERMINATOR = "\n"
ENCODING = "ascii"
WATCHDOG_MS = 3000

ANGLE_MIN = 0
ANGLE_MAX = 180
GRIP_MIN = 0
GRIP_MAX = 100

# Portas padrão (recomenda-se symlink udev estável; ver docs)
DEFAULT_HEAD_PORT = "/dev/ttyUSB0"   # ESP32
DEFAULT_ARM_PORT = "/dev/ttyUSB1"    # Arduino


class Mcu(str, Enum):
    HEAD = "head"   # ESP32
    ARM = "arm"     # Arduino


# Gestos válidos por MCU
HEAD_GESTURES = {"nod", "shake", "tilt", "center"}
ARM_GESTURES = {"wave", "rest", "point", "fistbump"}


class ProtocolError(ValueError):
    """Erro ao construir ou validar uma mensagem do protocolo."""


# ---------------------------------------------------------------------------
# Helpers de validação
# ---------------------------------------------------------------------------
def _clamp(value: int, lo: int, hi: int) -> int:
    return max(lo, min(hi, value))


def _check_angle(angle: int) -> int:
    if not isinstance(angle, int):
        raise ProtocolError(f"ângulo deve ser inteiro, recebido {angle!r}")
    if angle < ANGLE_MIN or angle > ANGLE_MAX:
        raise ProtocolError(
            f"ângulo {angle} fora da faixa [{ANGLE_MIN},{ANGLE_MAX}]"
        )
    return angle


# ---------------------------------------------------------------------------
# Construtores de comando (Host -> MCU). Retornam a linha JÁ com terminador.
# ---------------------------------------------------------------------------
def _frame(cmd: str, *args) -> str:
    body = ",".join(str(a) for a in args)
    return f"{cmd}:{body}{LINE_TERMINATOR}"


def head_pan(angle: int) -> str:
    """Girar cabeça na horizontal (ESP32). 90 = centro."""
    return _frame("HPAN", _check_angle(angle))


def head_tilt(angle: int) -> str:
    """Inclinar cabeça na vertical (ESP32). 90 = centro."""
    return _frame("HTLT", _check_angle(angle))


def head_gesture(name: str) -> str:
    """Gesto pré-programado da cabeça (ESP32)."""
    if name not in HEAD_GESTURES:
        raise ProtocolError(
            f"gesto de cabeça inválido {name!r}; válidos: {sorted(HEAD_GESTURES)}"
        )
    return _frame("HGES", name)


def arm_shoulder(angle: int) -> str:
    """Ângulo do ombro direito (Arduino)."""
    return _frame("ASHO", _check_angle(angle))


def arm_elbow(angle: int) -> str:
    """Ângulo do cotovelo direito (Arduino)."""
    return _frame("AELB", _check_angle(angle))


def arm_grip(value: int) -> str:
    """Garra: 0 = aberta, 100 = fechada (Arduino)."""
    if value < GRIP_MIN or value > GRIP_MAX:
        raise ProtocolError(
            f"garra {value} fora da faixa [{GRIP_MIN},{GRIP_MAX}]"
        )
    return _frame("AGRP", value)


def arm_gesture(name: str) -> str:
    """Gesto pré-programado do braço (Arduino)."""
    if name not in ARM_GESTURES:
        raise ProtocolError(
            f"gesto de braço inválido {name!r}; válidos: {sorted(ARM_GESTURES)}"
        )
    return _frame("AGES", name)


def ping() -> str:
    return _frame("PING")


def stop() -> str:
    return _frame("STOP")


def set_torque(enabled: bool) -> str:
    return _frame("TRQ", 1 if enabled else 0)


# ---------------------------------------------------------------------------
# Parsing de respostas (MCU -> Host)
# ---------------------------------------------------------------------------
class ReplyType(str, Enum):
    ACK = "ACK"
    ERR = "ERR"
    PONG = "PONG"
    STS = "STS"
    RDY = "RDY"
    UNKNOWN = "UNKNOWN"


@dataclass
class Reply:
    type: ReplyType
    raw: str
    args: list[str]

    @property
    def ok(self) -> bool:
        return self.type in (ReplyType.ACK, ReplyType.PONG, ReplyType.RDY, ReplyType.STS)


def parse_reply(line: str) -> Reply:
    """Interpreta uma linha vinda do MCU. Tolerante a espaços e CR/LF."""
    clean = line.strip()
    if ":" not in clean:
        return Reply(ReplyType.UNKNOWN, clean, [])
    head, _, body = clean.partition(":")
    head = head.strip().upper()
    args = [a.strip() for a in body.split(",")] if body.strip() else []
    try:
        rtype = ReplyType(head)
    except ValueError:
        rtype = ReplyType.UNKNOWN
    return Reply(rtype, clean, args)


def encode(line: str) -> bytes:
    """Converte uma linha de comando para bytes prontos para a serial."""
    return line.encode(ENCODING)
