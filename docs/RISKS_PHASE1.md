# Análise de Riscos — Fase 1 (dar os sentidos ao Baymax)

Baseado em inspeção real do Host (não suposições). Objetivo: antecipar erros e
delays antes de implementar.

## Estado do Host (confirmado)

| Recurso | Estado | Implicação |
|---|---|---|
| Saída de áudio | HDMI (card 0) + analógico ALC269VC (card 1) | Precisa escolher a saída certa; HDMI só funciona com monitor |
| Microfone | ALC269VC analógico (card 1) + DMIC array acp63 (card 2) | 2 opções; a DMIC pode exigir firmware/driver específico |
| Servidor de som | **NENHUM** (sem pipewire/pulse) | Captura e playback vão disputar o device ALSA cru |
| Webcam | **NENHUMA** (`/dev/video*` ausente) | vision_node não testável até ter câmera |
| Build | cmake 3.28, g++ 13.3, make OK | whisper.cpp/Piper compilam em CPU |
| Vulkan dev | headers e `glslc` **ausentes** | whisper.cpp com Vulkan exigiria sudo (libvulkan-dev, glslc) |
| RAM | 20 GiB disponível (modelo GPU ocupa ~2,2 GB) | Folga boa; cuidado só se somar whisper+piper+visão grandes |
| GPU | iGPU ocupada pelo LLM (Ollama/Vulkan) | STT/visão na GPU disputariam a MESMA iGPU com o LLM |

---

## Passo 1 — whisper.cpp (STT)

### Riscos
- **R1.1 — Disputa de GPU:** o LLM já usa a iGPU 780M via Vulkan. Rodar whisper
  TAMBÉM na GPU faz os dois competirem pela mesma iGPU → engasgo quando o Baymax
  ouve e pensa ao mesmo tempo. **Mitigação:** manter whisper em CPU (AVX-512 dá
  conta do modelo `small`); a CPU está ociosa enquanto o LLM está na GPU. Boa divisão
  de trabalho: ouvir=CPU, pensar=GPU.
- **R1.2 — whisper Vulkan precisa de sudo:** faltam headers Vulkan e `glslc`.
  Compilar whisper com GPU sairia do "sem sudo". **Decisão:** compilar whisper só com
  CPU/AVX-512. Mantém a promessa de sem-sudo e evita a disputa de GPU (R1.1).
- **R1.3 — Latência do STT:** o modelo `small` transcreve ~1–3s de fala em tempo
  aceitável em CPU, mas NÃO é instantâneo. Esse tempo SOMA à latência total
  (ouvir → transcrever → pensar → falar). **Mitigação:** usar `base` se precisar
  mais rápido; medir e decidir. Processar só quando o VAD detecta fim de fala.
- **R1.4 — Qualidade PT-BR:** whisper multilíngue erra mais em PT que em inglês,
  especialmente com ruído. **Mitigação:** forçar `language=pt`, e no futuro avaliar
  fine-tunes PT-BR.

### Delays esperados
- Compilação: ~2–5 min (uma vez).
- Download do modelo `small` (~466 MB) ou `base` (~142 MB): precisa de internet (setup).
- Runtime: ~0,5–2s por transcrição (CPU), dependendo do tamanho da fala.

---

## Passo 2 — Piper (TTS)

### Riscos
- **R2.1 — Sem servidor de som:** sem pipewire/pulse, o `aplay` fala direto no ALSA.
  Se o mic (arecord) e o Piper (aplay) usarem o device cru ao mesmo tempo, pode dar
  `device busy`. **Mitigação:** instalar PipeWire (precisa sudo) OU serializar
  (não ouvir enquanto fala — o que até evita o Baymax se ouvir, ver R3.3).
- **R2.2 — Escolher a saída errada:** se o default for HDMI e não houver monitor,
  o Baymax "fala" no vazio. **Mitigação:** fixar a saída analógica (card 1) por config.
- **R2.3 — Voz PT-BR:** as vozes Piper PT-BR existem mas variam em qualidade/naturalidade.
  **Mitigação:** testar algumas e escolher; é trocar um arquivo .onnx.

### Delays esperados
- Download binário Piper + voz PT-BR (~60–100 MB): internet (setup).
- Runtime: Piper é rápido (~0,2–0,5s para uma frase curta em CPU). Baixo risco.

---

## Passo 3 — mic_node (captura + VAD)

### Riscos
- **R3.1 — Device errado:** 2 microfones (analógico vs DMIC array). O DMIC (acp63)
  em laptops AMD recentes às vezes exige firmware/SOF específico e pode vir mudo.
  **Mitigação:** tornar o device um parâmetro ROS; testar os dois e fixar o que funciona.
- **R3.2 — Sem servidor de som (ALSA cru):** acesso exclusivo ao device. Ver R2.1.
- **R3.3 — Baymax se ouvir (feedback/eco):** se o mic captura enquanto o Piper fala,
  o Baymax transcreve a própria voz e entra em loop. **Mitigação:** "half-duplex" —
  pausar a captura enquanto fala (barato e robusto); futuro: cancelamento de eco.
- **R3.4 — VAD falso-positivo/negativo:** ruído dispara transcrição à toa, ou fala
  baixa não dispara. **Mitigação:** VAD com limiar ajustável (webrtcvad ou silero);
  parâmetros expostos no nó.

### Delays esperados
- `sounddevice`/`webrtcvad` via pip: rápido (sem sudo, se pip permitir user install).
- Runtime: captura é contínua; VAD é leve. Latência vem do "esperar o fim da fala".

---

## Passo 4 — vision_node (webcam + expressões)

### Riscos
- **R4.1 — SEM WEBCAM:** não há `/dev/video*`. **Bloqueio total de teste real** até
  conectar uma câmera USB. **Mitigação:** implementar com fonte plugável (câmera OU
  arquivo de vídeo OU imagem de teste) para desenvolver sem hardware; validar de
  verdade só quando a câmera chegar.
- **R4.2 — Disputa de GPU:** MediaPipe/ONNX podem usar a iGPU → conflito com o LLM
  (R1.1). **Mitigação:** rodar visão em CPU (modelos FER são leves) ou em baixa taxa (1–2 Hz).
- **R4.3 — Custo de CPU contínuo:** visão roda o tempo todo, diferente do LLM que é
  sob demanda. Pode roubar CPU do whisper. **Mitigação:** limitar FPS, processar 1 a
  cada N frames, resolução baixa.
- **R4.4 — Dependência pesada:** MediaPipe + OpenCV são grandes e às vezes brigam com
  versões de numpy/python. **Mitigação:** usar venv isolado; fixar versões.

### Delays esperados
- Instalação OpenCV/MediaPipe: ~2–4 min + download. numpy conflitos possíveis.
- Runtime: FER leve em CPU ~10–30 ms/frame; controlado pelo FPS escolhido.

---

## Passo 5 — Firmware Arduino (braço) + ESP32 (cabeça)

### Riscos
- **R5.1 — Hardware ausente/não testado:** sem os MCUs plugados, só dá para validar
  o protocolo em simulação (já feito). **Mitigação:** firmware + teste de loopback;
  validação real quando o hardware chegar.
- **R5.2 — Portas seriais instáveis:** `/dev/ttyUSB0/1` podem trocar de número ao
  reconectar. **Mitigação:** regra udev por serial do chip (já documentada em
  SERIAL_PROTOCOL.md) → nomes fixos `/dev/baymax_head`, `/dev/baymax_arm`.
- **R5.3 — Permissão serial:** usuário precisa estar no grupo `dialout` para abrir a
  serial sem sudo. **Verificar:** `groups` deve conter `dialout` (hoje NÃO contém).
- **R5.4 — Alimentação dos servos:** servos puxam corrente; alimentar pela USB do MCU
  queima/reinicia. **Mitigação (hardware):** fonte externa para servos, GND comum.
- **R5.5 — Bloqueio da serial:** leitura serial síncrona pode travar o nó ROS.
  **Mitigação:** timeout curto (já está 0,1s no motor_bridge) + thread de leitura.

### Delays esperados
- Escrever/flashar firmware: rápido, mas depende do hardware em mãos.

---

## Riscos transversais (afetam vários passos)

- **RT.1 — Contenção da iGPU:** só o LLM deve usar a GPU. STT e visão em CPU.
  É a regra de ouro para não criar engasgos.
- **RT.2 — Áudio full-duplex sem servidor de som:** o maior risco sistêmico.
  Recomendação: instalar PipeWire (sudo) OU adotar half-duplex (ouvir≠falar).
- **RT.3 — Internet só no setup:** downloads de modelos/binários precisam de rede uma
  vez. Depois tudo offline (regra nº 1 do projeto).
- **RT.4 — pip sem sudo:** instalar libs Python pode exigir `--user` ou venv.
  Preferir venv no workspace para não poluir o sistema.
- **RT.5 — Ordem de latência total:** ouvir(VAD espera fim) + transcrever(STT) +
  pensar(LLM) + falar(TTS). Cada etapa soma. O LLM já está rápido (~0,35s 1ª frase);
  o novo gargalo passa a ser o STT. Medir ponta a ponta.

---

## Passo 6 — Dashboard no projetor (HDMI) — NOVO

Contexto: o Baymax terá um projetor (HDMI) mostrando um dashboard de diagnóstico em
tempo real. Estado atual do Host: **headless** (sem servidor gráfico; `XDG_SESSION_TYPE=tty`),
HDMI desconectado, iGPU já com ~2,1 GB VRAM usados pelo LLM.

### Esclarecimento importante (corrige suposição)
O dashboard NÃO disputa compute com o LLM de forma relevante. A iGPU faz duas coisas
distintas: **compute** (inferência do LLM via Vulkan) e **scanout/render 2D** (desenhar
na tela e mandar pro HDMI). Um dashboard usa principalmente scanout + 2D leve — carga
pequena. A APU roda os dois em paralelo tranquilamente.

### Riscos
- **R6.1 — Falta ambiente gráfico:** Host é headless hoje. Dashboard exige instalar
  um compositor/kiosk (sudo). É software novo, não existe ainda.
- **R6.2 — CPU é o ponto de contenção, não a GPU:** dashboard pesado (ex. navegador)
  come CPU, que é onde rodam STT e visão. **Mitigação:** app nativo leve (Qt/pygame),
  evitar navegador; limitar taxa de atualização do dashboard.
- **R6.3 — VRAM compartilhada:** framebuffer do projetor sai da RAM (como a VRAM do LLM).
  Em 1080p é pequeno; em 4K cresce. Contabilizar. Folga boa nos 23 GB.
- **R6.4 — EDID/resolução do projetor:** projetores reportam EDID irregular, overscan,
  modo errado ("corta a imagem" / tela preta). **Mitigação:** testar com o projetor real;
  possível forçar modo/resolução.
- **R6.5 — Boot em kiosk:** para subir direto no dashboard sem desktop completo,
  precisa configurar autostart/kiosk. Trabalho de integração.

### Delays / dependências
- Instalar ambiente gráfico mínimo + app: sudo + download.
- Definições pendentes do usuário: tipo de dashboard (web/nativo/simples), kiosk vs
  desktop, resolução do projetor. **Projetor ainda não especificado — passo fica por último.**

### Regra de GPU — REVISADA
Antes: "só o LLM usa a GPU". Agora:
> A GPU roda o LLM (compute pesado, sob demanda) + o scanout do dashboard (leve, contínuo).
> STT e visão ficam na CPU. O ponto de contenção a vigiar é **CPU e VRAM compartilhada**,
> não o compute da GPU.

---

## Recomendação de ordem (menor risco primeiro)

1. **Piper (TTS)** — baixo risco, retorno imediato (Baymax ganha voz). Resolve saída de áudio.
2. **whisper.cpp (STT) em CPU** — sem sudo, sem disputa de GPU. Mede latência real.
3. **mic_node + VAD half-duplex** — fecha o loop de voz; resolve R3.3 por design.
4. **Firmware + motor real** — quando o hardware (MCUs/servos) estiver em mãos.
5. **vision_node** — bloqueado por R4.1 (sem webcam) até ter câmera.
6. **Dashboard/projetor** — por último; projetor e detalhes ainda não especificados.

Decisões pendentes para o usuário:
- Áudio: instalar **PipeWire** (sudo, full-duplex) ou **half-duplex** (sem sudo)?
- Dashboard: tipo (nativo leve / web / simples), kiosk vs desktop, resolução — quando houver projetor.
