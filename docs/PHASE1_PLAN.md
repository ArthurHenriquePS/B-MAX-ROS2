# Plano da Fase 1 — Dar vida ao Baymax

Status: **aguardando aprovação do usuário para começar.** Nada implementado ainda.
Análise de riscos detalhada em `docs/RISKS_PHASE1.md`.

## Princípios que guiam a fase 1

1. **Divisão de hardware:** LLM na iGPU (compute); STT e visão na CPU; dashboard usa
   scanout leve da GPU. Ponto de contenção a vigiar = CPU + VRAM compartilhada.
2. **Sem sudo quando possível:** compilar/baixar no workspace. Marcar claramente o que
   exige sudo (áudio/gráfico) para o usuário aprovar.
3. **Testável sem o hardware final:** cada nó com modo stub/plugável, para desenvolver
   antes de webcam/MCUs/projetor chegarem. Validação real quando o hardware existir.
4. **Medir cada etapa:** latência ponta a ponta após cada integração (não confiar em build).

## Estado do hardware (confirmado no Host)

| Recurso | Estado | Impacto no plano |
|---|---|---|
| Áudio saída | HDMI + analógico ALC269VC | Fixar saída analógica para o alto-falante |
| Microfone | ALC269VC + array DMIC acp63 | Testar os dois; device como parâmetro |
| Servidor de som | nenhum | Decisão PipeWire vs half-duplex |
| Webcam | nenhuma | vision_node bloqueado p/ teste real |
| GPU | iGPU com LLM (~2,1 GB VRAM) | STT/visão na CPU |
| Serial (MCUs) | nenhum conectado | firmware testável só em loopback/sim |
| Gráfico | headless (tty) | dashboard precisa de ambiente gráfico (novo) |
| grupo dialout | usuário NÃO está | necessário p/ serial sem sudo |

## Passos (ordem de menor risco → maior)

### Passo 1 — Piper (TTS): o Baymax ganha voz
- Baixar binário Piper + voz PT-BR em `models/`.
- Integrar no `tts_node` (já tem o esqueleto chamando piper + aplay).
- Fixar saída de áudio analógica (parâmetro).
- **Entrega:** `ros2 topic pub /tts_text` faz o Baymax falar pelo alto-falante.
- **Risco:** baixo. Sem sudo (download). Cuidado só com device de saída.
- **Mede:** latência do TTS (esperado ~0,2–0,5s/frase).

### Passo 2 — whisper.cpp (STT): o Baymax ouve
- Compilar whisper.cpp **só CPU** (AVX-512), sem Vulkan (evita disputa de GPU + sudo).
- Baixar modelo `small` (ou `base` se precisar mais rápido) em `models/`.
- Integrar no `stt_node`: recebe PCM, transcreve, publica `/speech_text`.
- **Entrega:** áudio gravado vira texto no tópico.
- **Risco:** médio. STT vira o novo gargalo de latência (~0,5–2s). Qualidade PT-BR.
- **Mede:** latência de transcrição por tamanho de fala; decidir small vs base.

### Passo 3 — mic_node + VAD: fecha o loop de voz
- Captura real (sounddevice/arecord) + VAD (webrtcvad/silero) para detectar fim de fala.
- Device de mic como parâmetro (testar analógico vs DMIC).
- **Half-duplex por padrão** (pausa captura enquanto fala) — resolve "Baymax se ouvir".
- **Entrega:** falar → Baymax ouve, pensa, responde por voz. Loop completo.
- **Risco:** médio-alto. Áudio sem servidor de som; feedback/eco; device certo.
- **Decisão pendente:** PipeWire (sudo, full-duplex) vs half-duplex (sem sudo).

### Passo 4 — Firmware MCUs + motor real
- Firmware Arduino (braço) e ESP32 (cabeça) implementando o protocolo (SERIAL_PROTOCOL.md).
- Teste de loopback do protocolo antes do hardware.
- Regra udev para portas fixas; usuário no grupo `dialout`.
- Trocar `simulate:=false` no motor_bridge quando o hardware chegar.
- **Entrega:** `/motion_cmd` move cabeça e braço de verdade.
- **Risco:** depende do hardware em mãos. Alimentação de servos (fonte externa).
- **Bloqueio:** MCUs/servos físicos.

### Passo 5 — vision_node: o Baymax vê expressões
- OpenCV + MediaPipe/FER em CPU, taxa baixa (1–2 Hz).
- Fonte plugável (câmera / vídeo / imagem de teste) para dev sem webcam.
- Publica `/emotion`; o brain_node já consome.
- **Entrega:** expressão facial detectada influencia a resposta.
- **Risco:** médio. Dependências pesadas (venv isolado). Custo de CPU contínuo.
- **Bloqueio:** sem webcam, teste real impossível (R4.1).

### Passo 6 — Dashboard no projetor (HDMI)
- Novo pacote `baymax_dashboard` assinando tópicos (`/emotion_state`, `/speech_text`, etc.).
- Ambiente gráfico mínimo (kiosk) — provavelmente app nativo leve (Qt/pygame).
- **Entrega:** projetor mostra diagnóstico em tempo real.
- **Risco:** precisa de ambiente gráfico (novo, sudo). EDID/resolução do projetor.
- **Bloqueio:** projetor e definições (tipo/kiosk/resolução) ainda não especificados.

## Impacto na latência total (ponta a ponta)

Hoje (só cérebro): LLM 1ª frase ~0,35s na GPU.
Com voz completa, a cadeia vira:
```
ouvir (VAD espera fim da fala) + transcrever (STT ~0,5–2s CPU)
  + pensar (LLM ~0,35s 1ª frase GPU) + falar (TTS ~0,2–0,5s/frase)
```
O novo gargalo passa a ser o **STT**. Por isso o passo 2 inclui medir e, se preciso,
trocar `small` por `base`.

## Decisões pendentes do usuário (antes/durante a fase 1)
1. **Áudio:** PipeWire (sudo, full-duplex) ou half-duplex (sem sudo)? — afeta passo 3.
2. **Dashboard:** tipo, kiosk vs desktop, resolução do projetor — afeta passo 6 (sem pressa).
3. **Hardware:** quando chegam webcam (passo 5), MCUs/servos (passo 4), projetor (passo 6)?

## O que NÃO está no caminho crítico
- Passos 1, 2, 3 (voz) podem ser feitos JÁ, sem hardware novo (só mic/alto-falante do Host).
- Passos 4, 5, 6 dependem de hardware que ainda não está presente.
