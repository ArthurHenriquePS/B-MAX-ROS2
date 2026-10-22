# Performance & Otimização de Latência — Baymax Brain

## Diagnóstico (medido, não estimado)

O "~3,5s" inicial era enganoso: incluía o **cold start** (carregar o modelo do disco
pra RAM). Separando os componentes com `scripts/bench_llm.py` e `scripts/measure_coldstart.py`:

| Componente | Custo medido |
|---|---|
| Cold start (load disco→RAM) | **~2,04 s** |
| Geração quente (resposta completa, persona) | ~0,9–1,2 s |
| Geração quente (resposta curta) | ~0,2 s |
| Velocidade de geração (CPU, AVX-512/zen4) | ~29–38 tokens/s |

Conclusão: o gargalo não era o LLM em si, e sim (1) o cold start e (2) esperar a
resposta inteira antes de começar a falar.

## Otimizações aplicadas (brain_node)

Todas via parâmetros ROS, reversíveis, sem sudo:

| Otimização | Parâmetro | Efeito |
|---|---|---|
| Modelo sempre quente | `keep_alive: -1` | Elimina cold start recorrente (~2s) |
| Warm-up no boot | `warmup: true` | 1ª interação já sai quente (carrega no boot) |
| Streaming por frase | `stream: true` | Baymax começa a falar na 1ª frase, não espera o fim |
| Resposta curta | `num_predict: 80` | Menos tokens = resposta mais rápida (combina com a persona) |
| Threads físicos | `num_thread: 8` | Usa os 8 núcleos físicos do Ryzen 8845HS |

### Resultados (medidos end-to-end via ROS)

| Cenário | Antes | Depois |
|---|---|---|
| 1ª frase (modelo quente) | ~3,5 s | **~0,4–0,8 s** |
| 1ª interação pós-boot (com warm-up) | ~2,8 s (cold) | **0,82 s** (quente) |
| Resposta completa (quente) | ~3,5 s | ~1,2 s (já falando desde ~0,4s) |

> `keep_alive: -1` mantém ~2,2 GB de RAM ocupados permanentemente pelo modelo.
> Com 23 GiB no Host, é folgado.

### Bug encontrado no caminho
`keep_alive` precisa ser **inteiro** (`-1`) na API do Ollama; passar string `"-1"`
retorna HTTP 400. Corrigido declarando o parâmetro ROS como int. O teste real pegou
isso (o fallback por regras mascarou, mas o LLM não rodava).

## Rodar o modelo na iGPU (Radeon 780M) — CONCLUÍDO ✅

Objetivo: acelerar a **geração** (os ~1,2s restantes) usando a iGPU. **Resultado: funcionou.**

### Resultado medido (CPU vs iGPU, mesmo benchmark)

| Métrica | CPU (zen4/AVX-512) | iGPU (Vulkan/RADV) | Ganho |
|---|---|---|---|
| Velocidade de geração | 29–38 tok/s | **40–49 tok/s** | ~30–40% |
| Resposta curta (total) | 216 ms | **154 ms** | 29% |
| Persona Baymax completa | 1234 ms | **702 ms** | **43%** |
| Warm-up (carga do modelo) | ~2040 ms | **~1170 ms** | 43% |

End-to-end pelo ROS (modelo quente): 1ª frase em **~350 ms**, resposta completa ~1,2–1,4s.
O modelo roda com **37/37 camadas na GPU** (`ollama ps` mostra `100% GPU`).

### O que foi preciso (o pulo do gato)
Ativar Vulkan sozinho NÃO basta: o Ollama **descarta iGPUs por padrão**
(`dropping integrated GPU; to enable, set OLLAMA_IGPU_ENABLE=1`). Precisa das DUAS flags.

Override em `/etc/systemd/system/ollama.service.d/vulkan.conf`:
```ini
[Service]
Environment="OLLAMA_VULKAN=1"
Environment="OLLAMA_IGPU_ENABLE=1"
```
Depois: `sudo systemctl daemon-reload && sudo systemctl restart ollama`.

### Confirmação de que roda na GPU
- `ollama ps` → coluna PROCESSOR = `100% GPU`.
- log: `offloaded 37/37 layers to GPU`, `using device Vulkan0 (AMD Radeon 780M ... RADV PHOENIX)`.
- ROCm foi (corretamente) descartado: `no rocblas support for gfx1103`.

### Reversão (se precisar voltar pra CPU)
Remover `/etc/systemd/system/ollama.service.d/vulkan.conf` (ou só a flag IGPU),
`daemon-reload` + `restart ollama`. A CPU com AVX-512/zen4 continua sendo um bom fallback.

---

## (Histórico) Plano original da iGPU

### Terreno (investigado)
- iGPU: Phoenix3 `[1002:1900]` = Radeon 780M, RDNA3, **gfx1103**.
- **ROCm**: a gfx1103 NÃO é oficialmente suportada; o runner ROCm trava na descoberta
  da GPU no 780M. ⇒ descartado.
- **Vulkan (Mesa RADV)**: caminho recomendado. Driver já instalado
  (`mesa-vulkan-drivers`, `libvulkan1`). O Ollama 0.40.2 já traz o backend embutido
  (`/usr/local/lib/ollama/vulkan/libggml-vulkan.so`) e as flags `OLLAMA_VULKAN`,
  `GGML_VK_VISIBLE_DEVICES`.

### Passos (requerem sudo + relogin de grupo)
```bash
# 1. Diagnóstico + acesso à GPU
sudo apt install -y vulkan-tools
sudo usermod -aG render,video $USER   # exige logout/login para valer

# 2. Confirmar que o Vulkan vê a 780M
vulkaninfo --summary | grep -E "deviceName|driverName"

# 3. Ativar Vulkan no serviço do Ollama
sudo systemctl edit ollama
#   [Service]
#   Environment="OLLAMA_VULKAN=1"
sudo systemctl daemon-reload && sudo systemctl restart ollama

# 4. Confirmar offload na GPU
journalctl -u ollama | grep -i vulkan     # deve mostrar backend Vulkan carregado
ollama ps                                 # PROCESSOR deve indicar GPU
```

### Riscos (confirmados na pesquisa da comunidade)
- Regressões no path Vulkan já foram reportadas em algumas versões do Ollama;
  a 0.40.2 é mais nova, mas só o teste confirma estabilidade no nosso Host.
- A iGPU usa RAM compartilhada (UMA/GTT) como VRAM — ok para o Qwen 3B.
- **Totalmente reversível**: remover `OLLAMA_VULKAN=1` e reiniciar volta pra CPU
  (que já está rápida com AVX-512/zen4).

### Método de validação
Medir CPU vs Vulkan lado a lado com `scripts/bench_llm.py` (tokens/s e latência).
Só manter o Vulkan se houver ganho real e estabilidade; caso contrário, voltar à CPU.

---

## Latência do fluxo de voz completo (medido encadeado)

Medição **encadeada real** (não soma de medições isoladas), injetando um WAV de fala
de 3,3s em `/utterance_wav` e cronometrando até a 1ª frase sair para o TTS.
Script: medição ad-hoc via probe nos tópicos `/speech_text` e `/tts_text`.

### Decomposição (modelo quente, LLM na iGPU, STT na CPU)

| Etapa | Latência medida | Onde roda |
|---|---|---|
| STT (áudio 3,3s → texto) | **~334 ms** | CPU (whisper base, RTF 0.10) |
| brain / LLM (texto → 1ª frase) | **~379 ms** | iGPU (Qwen2.5 3B, streaming) |
| TTS (texto → áudio sintetizado) | ~18–55 ms/frase | CPU (Piper, API persistente) |
| **TOTAL até a 1ª frase ser falada** | **~713 ms** | — |

> O TTS por frase (18–55ms) é desprezível e acontece em paralelo ao streaming do LLM:
> o Baymax começa a falar a 1ª frase enquanto o resto da resposta ainda é gerado.

### O que NÃO está incluído (e por quê)

Estas parcelas dependem do hardware/uso e NÃO foram medidas neste host:

- **Captura + VAD (tempo de "ouvir"):** o mic_node só publica a fala DEPOIS de detectar
  o fim dela (silêncio de ~700ms, parâmetro `silence_ms`). Esse tempo é dominado pela
  duração da própria fala do usuário + a janela de silêncio — não é "processamento".
  Não medível aqui (mic mudo, RMS=0.0).
- **Latência acústica (buffers de áudio ALSA/PipeWire, playback real):** depende do
  hardware de áudio final. Tipicamente dezenas de ms.
- **Cold start:** eliminado em operação (keep_alive=-1 + warm-up no boot). Só a 1ª
  resposta após reiniciar o Ollama pagaria ~1–2s de load.

### Leitura prática

- Do fim da fala do usuário até o Baymax **começar a responder**: **~0,7 s** (medido).
- É tempo de conversa natural. O gargalo está distribuído: ~metade STT, ~metade LLM.
- A janela de silêncio do VAD (700ms) soma à percepção de "demora a responder"; é
  ajustável (`silence_ms`) — trade-off entre responder rápido e cortar a fala do usuário.

### Comparação com o ponto de partida

| Momento | Latência percebida |
|---|---|
| Início do projeto (LLM cold, sem streaming) | ~3,5 s |
| Após otimização do LLM (GPU + streaming + warm-up) | ~0,35 s (só o pensar) |
| Fluxo de voz completo (STT + LLM, encadeado) | **~0,7 s** |
