#!/usr/bin/env python3
"""Teste de bancada do protocolo serial (sem ROS).

Roda com: python3 scripts/test_serial_protocol.py
Valida construção de comandos, faixas e parsing de respostas.
"""
import sys
from pathlib import Path

# Permite importar baymax_common sem instalar o pacote
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "src" / "baymax_common"))

from baymax_common import serial_protocol as sp  # noqa: E402

failures = []


def check(desc, got, expected):
    if got != expected:
        failures.append(f"[FALHOU] {desc}\n   esperado: {expected!r}\n   obtido:   {got!r}")
    else:
        print(f"[ok] {desc} -> {got!r}")


def check_raises(desc, fn):
    try:
        fn()
    except sp.ProtocolError:
        print(f"[ok] {desc} (levantou ProtocolError como esperado)")
    else:
        failures.append(f"[FALHOU] {desc}: deveria ter levantado ProtocolError")


print("=== Construtores de comando ===")
check("head_pan(120)", sp.head_pan(120), "HPAN:120\n")
check("head_tilt(90)", sp.head_tilt(90), "HTLT:90\n")
check("head_gesture(nod)", sp.head_gesture("nod"), "HGES:nod\n")
check("arm_shoulder(45)", sp.arm_shoulder(45), "ASHO:45\n")
check("arm_elbow(100)", sp.arm_elbow(100), "AELB:100\n")
check("arm_grip(100)", sp.arm_grip(100), "AGRP:100\n")
check("arm_gesture(wave)", sp.arm_gesture("wave"), "AGES:wave\n")
check("ping()", sp.ping(), "PING:\n")
check("stop()", sp.stop(), "STOP:\n")
check("set_torque(True)", sp.set_torque(True), "TRQ:1\n")
check("set_torque(False)", sp.set_torque(False), "TRQ:0\n")

print("\n=== Validação de faixa ===")
check_raises("head_pan(200) fora de faixa", lambda: sp.head_pan(200))
check_raises("head_pan(-5) fora de faixa", lambda: sp.head_pan(-5))
check_raises("arm_grip(150) fora de faixa", lambda: sp.arm_grip(150))
check_raises("head_gesture inválido", lambda: sp.head_gesture("dance"))
check_raises("arm_gesture inválido", lambda: sp.arm_gesture("nod"))

print("\n=== Parsing de respostas ===")
r = sp.parse_reply("ACK:HPAN\n")
check("parse ACK tipo", r.type, sp.ReplyType.ACK)
check("parse ACK args", r.args, ["HPAN"])
check("parse ACK ok", r.ok, True)

r = sp.parse_reply("ERR:ASHO,range")
check("parse ERR tipo", r.type, sp.ReplyType.ERR)
check("parse ERR args", r.args, ["ASHO", "range"])
check("parse ERR ok", r.ok, False)

r = sp.parse_reply("  PONG:  ")
check("parse PONG tipo", r.type, sp.ReplyType.PONG)

r = sp.parse_reply("STS:hpan=90;htlt=90")
check("parse STS tipo", r.type, sp.ReplyType.STS)

r = sp.parse_reply("RDY:head")
check("parse RDY tipo", r.type, sp.ReplyType.RDY)
check("parse RDY args", r.args, ["head"])

r = sp.parse_reply("lixo sem dois pontos")
check("parse desconhecido", r.type, sp.ReplyType.UNKNOWN)

print("\n=== Encoding ===")
check("encode", sp.encode("HPAN:120\n"), b"HPAN:120\n")

print()
if failures:
    print("\n".join(failures))
    print(f"\n{len(failures)} teste(s) falharam.")
    sys.exit(1)
print("Todos os testes do protocolo passaram. ✅")
