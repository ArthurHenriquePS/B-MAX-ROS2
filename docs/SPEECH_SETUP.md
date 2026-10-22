# Setup de Fala (TTS/STT/Mic) — Baymax

## Ambiente Python isolado (venv)

O Ubuntu 24.04 é **externally-managed** (PEP 668): não dá para `pip install` no sistema
nem `pip --user` sem quebrar proteções. Solução: **venv isolado** no workspace.

```bash
# requer python3.12-venv (1 sudo, uma vez)
sudo apt install -y python3.12-venv
cd ~/baymax_ws
python3 -m venv --system-site-packages .venv-speech   # vê o rclpy do ROS
. .venv-speech/bin/activate
pip install piper-tts
```

`--system-site-packages` é essencial: o venv enxerga o `rclpy`/ROS do sistema E as libs
de fala instaladas via pip (piper, futuramente whisper bindings, sounddevice, webrtcvad).

## Piper (TTS)

- Pacote: `piper-tts` (fork oficial atual: OHF-Voice/piper1-gpl; o rhasspy/piper foi
  arquivado em out/2025).
- Voz PT-BR baixada em `models/`:
  ```bash
  python3 -m piper.download_voices pt_BR-faber-medium --data-dir models
  ```
- API Python (usada no tts_node — carrega a voz UMA vez):
  ```python
  from piper import PiperVoice
  voice = PiperVoice.load("models/pt_BR-faber-medium.onnx")
  with wave.open(path, "wb") as wf:
      voice.synthesize_wav(texto, wf)
  ```
- Desempenho medido: load ~866ms (1x no boot); síntese 18–55ms/frase (RTF ~0.28, CPU).

## Rodar nós de fala com o venv

Os nós `tts_node`/`stt_node`/`mic_node` precisam do venv (onde o piper vive). O launch
os invoca com o python do venv via `prefix`:

```python
prefix = "/home/icbaymax/baymax_ws/.venv-speech/bin/python3"
Node(package="baymax_speech", executable="tts_node", prefix=prefix, ...)
```

Argumento de launch `speech_venv` permite mudar o caminho do venv.

## Áudio (ALSA)

- Saída: placa analógica = **card 1** → device `plughw:1,0` (parâmetro `audio_device`
  do tts_node). HDMI é card 0 (só com monitor/projetor).
- Microfone: ALC269VC analógico (card 1) OU array DMIC acp63 (card 2) — a definir no mic.
- **Sem servidor de som** (PipeWire/Pulse). Decisão pendente para full-duplex vs half-duplex.

## Half-duplex (anti-eco)

O tts_node publica `/speaking` (Bool): `true` enquanto fala, `false` ao terminar o turno.
O mic_node (passo 3) deve pausar a captura quando `/speaking == true`, evitando que o
Baymax transcreva a própria voz.
