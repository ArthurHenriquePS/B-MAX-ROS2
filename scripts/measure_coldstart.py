#!/usr/bin/env python3
"""Mede o custo de cold start do modelo (carga disco -> RAM)."""
import json
import urllib.request

URL = "http://127.0.0.1:11434/api/generate"


def call(payload):
    data = json.dumps(payload).encode()
    req = urllib.request.Request(URL, data=data, headers={"Content-Type": "application/json"})
    with urllib.request.urlopen(req, timeout=120) as r:
        return json.loads(r.read().decode())


# cold start: modelo foi descarregado antes de chamar este script
d = call({"model": "qwen2.5:3b", "prompt": "oi", "stream": False,
          "options": {"num_predict": 5}})
print("=== COLD START (primeira chamada) ===")
print(f"load_duration:  {d.get('load_duration',0)/1e9:.2f} s  (carregar modelo p/ RAM)")
print(f"total_duration: {d.get('total_duration',0)/1e9:.2f} s")

# segunda chamada: já quente
d2 = call({"model": "qwen2.5:3b", "prompt": "oi de novo", "stream": False,
           "options": {"num_predict": 5}})
print("\n=== QUENTE (segunda chamada) ===")
print(f"load_duration:  {d2.get('load_duration',0)/1e9:.2f} s")
print(f"total_duration: {d2.get('total_duration',0)/1e9:.2f} s")

print(f"\n>>> Custo do cold start: ~{(d.get('load_duration',0))/1e9:.2f} s")
