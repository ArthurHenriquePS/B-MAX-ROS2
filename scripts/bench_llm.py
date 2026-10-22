#!/usr/bin/env python3
"""Benchmark de latência do LLM local (Ollama).

Mede e separa os componentes da latência: carga do modelo, avaliação do prompt
e geração dos tokens. Roda vários prompts e reporta métricas.

Uso: python3 scripts/bench_llm.py [modelo]
"""
import json
import sys
import time
import urllib.request

MODEL = sys.argv[1] if len(sys.argv) > 1 else "qwen2.5:3b"
URL = "http://127.0.0.1:11434/api/generate"


def ms(ns):
    return f"{ns / 1e6:7.0f} ms"


def call(prompt, num_predict=None, system=None):
    options = {"temperature": 0.6}
    if num_predict is not None:
        options["num_predict"] = num_predict
    payload = {"model": MODEL, "prompt": prompt, "stream": False, "options": options}
    if system is not None:
        payload["system"] = system
    data = json.dumps(payload).encode()
    req = urllib.request.Request(URL, data=data, headers={"Content-Type": "application/json"})
    t0 = time.time()
    with urllib.request.urlopen(req, timeout=120) as r:
        d = json.loads(r.read().decode())
    wall = time.time() - t0
    return d, wall


def report(tag, d, wall):
    ec = d.get("eval_count", 0)
    ed = d.get("eval_duration", 1)
    pc = d.get("prompt_eval_count", 0)
    tps = ec / (ed / 1e9) if ed else 0
    print(f"\n=== {tag} ===")
    print("resposta:        ", repr(d.get("response", "")[:90]))
    print("wall-clock:      ", f"{wall*1000:7.0f} ms  (medido pelo cliente)")
    print("total_duration:  ", ms(d.get("total_duration", 0)))
    print("load_duration:   ", ms(d.get("load_duration", 0)), " (carregar modelo p/ RAM)")
    print("prompt_eval:     ", ms(d.get("prompt_eval_duration", 0)), f" ({pc} tokens de entrada)")
    print("eval (geracao):  ", ms(d.get("eval_duration", 0)), f" ({ec} tokens gerados)")
    print("velocidade:      ", f"{tps:7.1f} tokens/s")
    return tps, ec


if __name__ == "__main__":
    print(f"Modelo: {MODEL}")

    # 1) Aquecer (ignora load_duration nas próximas)
    print("\n>>> Aquecendo o modelo...")
    call("oi", num_predict=5)

    # 2) Prompt curto, resposta curta
    d, w = call("Diga ola em uma unica frase curta.", num_predict=40)
    report("Prompt curto / resposta curta", d, w)

    # 3) Prompt realista (persona Baymax completa)
    persona = (
        "Você é Baymax, um robô-enfermeiro gentil e calmo. Fale em português do Brasil, "
        "frases curtas e empáticas."
    )
    d, w = call(
        "[A pessoa parece: sad]\nPessoa: Oi Baymax, estou me sentindo triste hoje.\nBaymax:",
        system=persona,
    )
    report("Persona Baymax (sem limite de tokens)", d, w)

    # 4) Persona Baymax com num_predict limitado (resposta curta forçada)
    d, w = call(
        "[A pessoa parece: sad]\nPessoa: Oi Baymax, estou me sentindo triste hoje.\nBaymax:",
        system=persona,
        num_predict=48,
    )
    report("Persona Baymax (num_predict=48)", d, w)
