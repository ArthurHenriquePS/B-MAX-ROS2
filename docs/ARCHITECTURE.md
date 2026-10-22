# Arquitetura — Baymax Brain

## Princípio de design

O cérebro do Baymax é um **pipeline de percepção → cognição → ação**, implementado como um
grafo de nós ROS2. Cada etapa é desacoplada por tópicos, o que dá três benefícios no nosso
hardware (Ryzen 8845HS, 16 threads, sem GPU dedicada):

1. **Paralelismo real** — visão, escuta e fala rodam em processos separados nos múltiplos cores.
2. **Isolamento de falha** — um nó travado (ex. serial do motor) não derruba o cérebro.
3. **Latência mascarada** — enquanto o LLM "pensa", a visão e a escuta continuam coletando contexto.

## Fluxo de dados (ciclo de interação)

```
                        ┌───────────────────────────┐
                        │        PERCEPÇÃO          │
                        └───────────────────────────┘
   microfone ─► mic_node ─/audio_raw─► stt_node ─/speech_text─┐
                                                              │
   webcam ───► vision_node ─────────────────/emotion─────────┤
                                                              ▼
                        ┌───────────────────────────┐
                        │   COGNIÇÃO (brain_node)   │
                        │   Qwen2.5 3B via Ollama   │
                        │  junta fala + emoção,     │
                        │  gera resposta + ações     │
                        └───────────────────────────┘
                                      │
             ┌────────────────────────┼────────────────────────┐
         /tts_text                /motion_cmd              /emotion_state
             ▼                        ▼                        ▼
         tts_node               motor_bridge_node        (log/estado interno)
         (Piper)                (pyserial → UART)
             ▼                        ▼
       alto-falante          Arduino (braço) + ESP32 (cabeça)
```

## Contrato de tópicos

| Tópico | Tipo | Publicado por | Assinado por |
|---|---|---|---|
| `/audio_raw` | `std_msgs/ByteMultiArray` (PCM) | mic_node | stt_node |
| `/speech_text` | `std_msgs/String` | stt_node | brain_node |
| `/emotion` | `baymax_interfaces/Emotion` | vision_node | brain_node |
| `/tts_text` | `std_msgs/String` | brain_node | tts_node |
| `/motion_cmd` | `baymax_interfaces/MotionCommand` | brain_node | motor_bridge_node |
| `/emotion_state` | `std_msgs/String` | brain_node | (debug/UI) |

> O áudio pode migrar depois para `audio_common_msgs` se adotarmos esse pacote; por ora
> mantemos simples com tipos padrão para reduzir dependências.

## brain_node — o coração cognitivo

Responsabilidades:
1. Receber `/speech_text` (o que o usuário disse) e `/emotion` (como está).
2. Montar o prompt com uma **persona Baymax** (calmo, cuidadoso, assistente de saúde pessoal).
3. Chamar o LLM local (Qwen2.5 3B via Ollama, endpoint `http://127.0.0.1:11434`).
4. Separar a saída em: fala (`/tts_text`) e intenção de movimento (`/motion_cmd`).

O movimento pode ser derivado de "tags" na resposta do LLM (ex. o modelo emite
`[HEAD:nod]`) ou por regras simples a partir da emoção/contexto. Começamos com regras
determinísticas + algumas tags, para manter previsível.

## Decisões de inferência

- **Por que Qwen2.5 3B:** forte em PT-BR, cabe folgado na RAM em Q4_K_M (~2 GB), latência
  conversacional boa em CPU. Dá pra escalar para 7–8B depois trocando só o parâmetro de modelo.
- **Por que Ollama:** gerencia modelo/quantização, expõe API HTTP local estável, facilita
  trocar de modelo sem mexer no código do nó.
- **Por que whisper.cpp (não faster-whisper):** binário C++ otimizado para AVX, sem dependência
  de CUDA; ideal para CPU AMD.
- **Por que Piper:** TTS neural leve em CPU, vozes PT-BR, latência baixa — combina com o tom do Baymax.

## Fase 2 (otimizações futuras)

- Vulkan backend no llama.cpp/whisper.cpp para usar a Radeon 780M.
- Avaliar NPU Ryzen AI (XDNA) quando o stack Linux amadurecer.
- VAD (voice activity detection) no mic_node para não transcrever silêncio.
- Wake word ("Hey Baymax") antes de ativar o STT completo.
