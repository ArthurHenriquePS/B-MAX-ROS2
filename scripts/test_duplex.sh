#!/usr/bin/env bash
# Teste comparativo full-duplex vs half-duplex do mic_node.
# RODAR NO HARDWARE FINAL (microfone + alto-falante reais), pois a diferença entre
# os modos é ACÚSTICA (eco do alto-falante vazando no microfone).
#
# Uso:
#   scripts/test_duplex.sh half   # ou: full
#
# O que observar:
#   - half: ao falar com o Baymax e ele responder, o mic NÃO deve captar a voz dele
#           (nenhuma utterance espúria durante /speaking == true). Você NÃO consegue
#           interromper o Baymax no meio da fala.
#   - full: você CONSEGUE interromper falando por cima. Risco: o mic pode captar a voz
#           do Baymax e transcrevê-la (eco). Verificar se gera utterances falsas.
#
# Métrica de gargalo: observar uso de CPU (htop) e latência de resposta nos dois modos.

set -euo pipefail
MODE="${1:-half}"
WS="$HOME/baymax_ws"

source /opt/ros/jazzy/setup.bash
source "$WS/install/setup.bash"

echo ">>> Subindo mic_node em modo duplex=$MODE"
echo ">>> Fale com o Baymax. Observe se há eco (full) ou corte (half)."
echo ">>> Ctrl+C para encerrar."

# Para full-duplex, garantir PipeWire ativo
if [ "$MODE" = "full" ]; then
  systemctl --user start pipewire pipewire-pulse wireplumber 2>/dev/null || true
  echo ">>> PipeWire: $(systemctl --user is-active pipewire 2>/dev/null || echo n/d)"
fi

exec "$WS/.venv-speech/bin/python3" \
  "$WS/install/baymax_speech/lib/baymax_speech/mic_node" \
  --ros-args -p duplex:="$MODE"
