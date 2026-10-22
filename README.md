# Baymax Brain 🤖

Cérebro de IA 100% local e offline para o Baymax (Big Hero 6), orquestrado por **ROS2 Jazzy**.
Roda inteiramente no Host — sem API, sem internet — e controla cabeça e braço direito
via **UART** (Arduino + ESP32) usando um protocolo serial simples.

## Hardware do Host (cérebro)

| Componente | Detalhe |
|---|---|
| CPU | AMD Ryzen 7 8845HS — 8C/16T, até 5.1 GHz, AVX-512 / VNNI / BF16 |
| iGPU | Radeon 780M (RDNA3) — aceleração via Vulkan (opcional, fase 2) |
| NPU | AMD IPU (XDNA) — presente, stack Linux imaturo (não usado por ora) |
| RAM | 23 GiB + 8 GiB swap |
| Disco | 98 GB (~82 GB livres) |
| SO | Ubuntu 24.04.5 LTS, kernel 6.8, x86_64 |

**Estratégia de inferência:** CPU-first com modelos quantizados (aproveita AVX-512/VNNI).
GPU dedicada ausente; iGPU via Vulkan fica como otimização de fase 2.

## Stack de IA (tudo local)

| Função | Ferramenta | Modelo inicial |
|---|---|---|
| LLM (raciocínio/conversa) | Ollama + llama.cpp | **Qwen2.5 3B Instruct (Q4_K_M)** |
| STT (voz → texto) | whisper.cpp | `small` (multilíngue, PT-BR) |
| TTS (texto → voz) | Piper | voz PT-BR |
| Visão (expressões faciais) | MediaPipe / ONNX FER | Face landmarks + classificador leve |

## Arquitetura ROS2

Cada capacidade é um **nó** independente comunicando por **tópicos**. Isso roda em paralelo
nos 16 threads e isola falhas (se o LLM está pensando, a visão continua; se um motor trava,
o resto não cai).

```
[Mic] → mic_node ──/audio_raw──→ stt_node (whisper.cpp)
                                      │
                                 /speech_text
                                      ▼
[Webcam] → vision_node ──/emotion──→ brain_node (Qwen2.5 3B)
                                      │
                         ┌────────────┼───────────────┐
                    /tts_text    /motion_cmd      /emotion_state
                         ▼            ▼
                    tts_node     motor_bridge_node
                    (Piper)      (pyserial → UART)
                         │            │
                   [Alto-falante]   /dev/ttyUSB* ──UART──→ Arduino (braço) + ESP32 (cabeça)
```

### Nós

| Pacote | Nó | Entrada | Saída | Responsabilidade |
|---|---|---|---|---|
| baymax_speech | `mic_node` | microfone | `/audio_raw` | Captura áudio do mic |
| baymax_speech | `stt_node` | `/audio_raw` | `/speech_text` | Transcreve fala (whisper.cpp) |
| baymax_speech | `tts_node` | `/tts_text` | alto-falante | Sintetiza voz (Piper) |
| baymax_vision | `vision_node` | webcam | `/emotion` | Detecta expressão facial |
| baymax_brain | `brain_node` | `/speech_text`, `/emotion` | `/tts_text`, `/motion_cmd`, `/emotion_state` | Decide resposta e ações (LLM) |
| baymax_motor | `motor_bridge_node` | `/motion_cmd` | UART | Traduz comandos → serial p/ MCUs |

### Pacotes

- `baymax_interfaces` — mensagens customizadas (`.msg`)
- `baymax_common` — código Python compartilhado (protocolo serial, utilidades)
- `baymax_brain` — nó do LLM
- `baymax_speech` — mic, STT, TTS
- `baymax_vision` — visão/expressões
- `baymax_motor` — ponte serial UART
- `baymax_bringup` — launch files e configs

## Divisão de MCUs

| MCU | Controla | Conexão |
|---|---|---|
| **ESP32** | Cabeça (pan/tilt) | UART (ex. `/dev/ttyUSB0`) |
| **Arduino** | Braço direito | UART (ex. `/dev/ttyUSB1`) |

Protocolo serial simples ASCII — ver [`docs/SERIAL_PROTOCOL.md`](docs/SERIAL_PROTOCOL.md).

## Como construir

```bash
cd ~/baymax_ws
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## Como rodar

```bash
ros2 launch baymax_bringup baymax.launch.py
```

## Status do projeto

Ver `docs/ROADMAP.md`. Fase atual: scaffolding + instalação da stack.
