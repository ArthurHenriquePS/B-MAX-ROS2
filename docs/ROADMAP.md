# Roadmap — Baymax Brain

## Fase 0 — Fundação (atual)
- [x] Análise de hardware do Host
- [x] Definição da stack de IA (Qwen2.5 3B, whisper.cpp, Piper, visão)
- [x] Estrutura do workspace ROS2 + documentação de arquitetura
- [x] Protocolo serial simples (Arduino/ESP32) + teste de bancada
- [x] Instalar ROS2 Jazzy + dependências (colcon, pyserial, rosdep)
- [x] Scaffold dos nós ROS2 (todos os pacotes + launch + params)
- [x] Build do workspace (7 pacotes, colcon OK com g++ 13.3)
- [x] Instalar stack base: build-essential, áudio, Ollama 0.40.2, Qwen2.5 3B (1.9 GB)
- [x] Launch de bringup + validação do grafo (6 nós, 6 tópicos)
- [x] Teste end-to-end: fala -> brain(LLM) -> resposta falada + ponte de movimento

### Validação end-to-end (realizada)
- `/speech_text` -> brain_node -> Qwen2.5 3B (local) -> `/tts_text`: resposta empática OK.
- `/motion_cmd` -> motor_bridge -> protocolo serial: `HGES:nod`, `AGES:wave`, `HPAN:120` OK.

### Otimização de latência (CONCLUÍDA) — ver docs/PERFORMANCE.md
- Diagnóstico: o "~3,5s" era dominado por cold start (~2s), não pelo LLM.
- Aplicado: keep_alive=-1, warm-up no boot, streaming por frase, num_predict=80, 8 threads.
- Resultado: 1ª frase em ~0,4–0,8s (antes ~3,5s); 1ª interação pós-boot em 0,82s.
- Bug corrigido: keep_alive precisa ser int (-1), não string (HTTP 400).

### Falta para o Baymax "ganhar vida" (fase 1)
- whisper.cpp (STT real) + Piper (TTS real, voz PT-BR) — compilar/baixar em models/.
- Captura de áudio real no mic_node + VAD.
- Visão real no vision_node (OpenCV + MediaPipe/FER).
- Firmware Arduino (braço) + ESP32 (cabeça) implementando o protocolo.

## Fase 1 — Integração funcional
- [x] brain_node com persona Baymax + chamada Ollama (feito na fase 0)
- [x] tts_node integrado ao Piper (voz PT-BR, API persistente, /speaking p/ half-duplex)
- [x] stt_node integrado ao whisper.cpp (pywhispercpp, modelo base, CPU)
- [x] mic_node com captura real + VAD (código completo; teste acústico pende de hardware)
- [ ] vision_node com webcam + classificação de emoção (bloqueado: sem webcam)
- [ ] motor_bridge_node falando com MCU real (bloqueado: sem MCUs)
- [ ] Firmware Arduino (braço) + ESP32 (cabeça) implementando o protocolo

### Passo 1 (Piper/TTS) — CONCLUÍDO
- Piper via pip (OHF-Voice/piper1-gpl) em venv isolado `.venv-speech` (--system-site-packages).
- Voz `pt_BR-faber-medium` em models/. API Python persistente: load 1x (~866ms),
  síntese 18–55ms/frase (RTF 0.28) — TTS deixou de ser gargalo.
- tts_node: fila de frases em ordem, device analógico plughw:1,0, publica /speaking.
- Nós de fala rodam sob o venv via `prefix` no launch (venv vê rclpy + piper).
- Teste integrado: brain(GPU) -> tts falando PT-BR, 1ª frase em ~360ms. OK.
- Requereu 1 sudo: `apt install python3.12-venv` (Ubuntu 24.04 é PEP 668/externally-managed).

### Passo 2 (whisper.cpp/STT) — CONCLUÍDO
- pywhispercpp no venv (bindings do whisper.cpp; faster-whisper descartado — depende de CUDA).
- Modelos em models/whisper/. ESCOLHIDO: **base** em CPU.
  - base:  ~330ms p/ 3.3s de fala (RTF 0.10), qualidade PT-BR ok.
  - small: ~1140ms (RTF 0.34), 3.5x mais lento SEM ganho real nas frases curtas → descartado.
- Decisão GPU: NÃO vale agora. STT em CPU (RTF 0.10) não é gargalo; iGPU fica dedicada ao LLM.
  (Reavaliar só se migrar p/ modelo maior. Vulkan daria ~10x mas exige sudo + disputa de GPU.)
- Arquitetura: mic_node grava WAV 16kHz mono de cada fala (VAD) e publica o caminho em
  /utterance_wav; stt_node transcreve e publica /speech_text (que o brain já escuta).
- REQUISITO achado no teste: whisper exige 16000 Hz mono (Piper gera 22050 — reamostrar).
- Teste: /utterance_wav -> stt_node (whisper base) -> /speech_text transcrito. OK.

### Passo 3 (mic_node + VAD) — CÓDIGO COMPLETO, teste acústico pendente de hardware
- Libs no venv: sounddevice 0.5.6, webrtcvad. Capta via PortAudio, reamostra p/ 16kHz.
- mic_node: VAD segmenta falas, grava WAV 16kHz, publica /utterance_wav (-> stt_node).
- Parâmetro `duplex`: "half" (anti-eco via /speaking, sem sudo) ou "full" (interrompível).
- PipeWire instalado + ativo (systemd --user) para suportar full-duplex.
- VALIDADO: sintaxe, VAD+segmentação com áudio real (fala 3960ms detectada), nó abre
  o device e respeita /speaking. Dois microfones vistos: ALC269VC (hw:1,0) e DMIC (hw:2,0).
- NÃO VALIDÁVEL neste host: captura de voz real (mic mudo, RMS=0.0) e eco acústico.
  O comparativo full vs half só no hardware final → scripts/test_duplex.sh.
- Requereu sudo: `apt install pipewire pipewire-pulse pipewire-alsa wireplumber`.

## Fase 2 — Otimização e robustez
- [x] Otimização de latência em CPU (keep_alive, warm-up, streaming) — docs/PERFORMANCE.md
- [x] Backend Vulkan (iGPU Radeon 780M) para o LLM — CONCLUÍDO. 100% GPU, 37/37 camadas.
      Ganho ~30–43% na geração. Chave: OLLAMA_VULKAN=1 + OLLAMA_IGPU_ENABLE=1. Ver docs/PERFORMANCE.md.
- [ ] Wake word ("Hey Baymax")
- [ ] Avaliar NPU Ryzen AI (XDNA)
- [ ] Gerenciamento de estado/humor do Baymax
- [ ] Watchdog e reconexão serial automática

## Fase 3 — Comportamento
- [ ] Mapeamento emoção detectada → resposta empática
- [ ] Gestos coordenados (fala + movimento sincronizados)
- [ ] Memória de conversa curta (contexto entre turnos)
