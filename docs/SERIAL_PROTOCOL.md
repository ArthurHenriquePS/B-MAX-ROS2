# Protocolo Serial — Baymax ⇄ MCUs

Protocolo **ASCII, baseado em linha**, propositalmente simples para ser fácil de debugar
(dá para digitar à mão num monitor serial) e trivial de implementar em Arduino/ESP32.

## Camada física

| Parâmetro | Valor |
|---|---|
| Interface | UART |
| Baud rate | **115200** |
| Data bits | 8 |
| Paridade | Nenhuma |
| Stop bits | 1 |
| Fluxo | Nenhum |
| Fim de linha | `\n` (LF) |
| Encoding | ASCII |

Dois MCUs, cada um em sua própria porta serial:

| MCU | Papel | Porta (exemplo) |
|---|---|---|
| ESP32 | Cabeça (pan + tilt) | `/dev/ttyUSB0` |
| Arduino | Braço direito (ombro + cotovelo + garra) | `/dev/ttyUSB1` |

> As portas reais podem variar. Recomenda-se fixar por `udev` (regra por serial do chip FTDI/CP210x)
> para que ESP32 e Arduino sempre apareçam com nomes estáveis (ex. `/dev/baymax_head`,
> `/dev/baymax_arm`). Ver seção "udev" abaixo.

## Formato de mensagem

Uma mensagem por linha, terminada em `\n`:

```
<CMD>:<ARG1>[,<ARG2>...]\n
```

- `CMD` — comando de 1+ letras maiúsculas.
- Argumentos separados por vírgula.
- Ângulos em **graus inteiros**. Velocidade/duração quando aplicável.
- Espaços são ignorados.

### Comandos: Host → MCU

| Comando | Formato | Alvo | Significado |
|---|---|---|---|
| Pan da cabeça | `HPAN:<ang>` | ESP32 | Girar cabeça horizontal. `ang` 0–180 (90 = centro) |
| Tilt da cabeça | `HTLT:<ang>` | ESP32 | Inclinar cabeça vertical. `ang` 0–180 (90 = centro) |
| Gesto da cabeça | `HGES:<nome>` | ESP32 | Gesto pré-programado: `nod`, `shake`, `tilt`, `center` |
| Ombro | `ASHO:<ang>` | Arduino | Ângulo do ombro direito. 0–180 |
| Cotovelo | `AELB:<ang>` | Arduino | Ângulo do cotovelo direito. 0–180 |
| Garra | `AGRP:<val>` | Arduino | Garra: 0 = aberta, 100 = fechada |
| Gesto do braço | `AGES:<nome>` | Arduino | Gesto: `wave`, `rest`, `point`, `fistbump` |
| Ping | `PING:` | ambos | Teste de conexão (espera `PONG`) |
| Parada | `STOP:` | ambos | Parar movimento / posição segura |
| Habilitar torque | `TRQ:<0\|1>` | ambos | 0 = relaxa servos, 1 = segura posição |

### Respostas: MCU → Host

| Resposta | Formato | Significado |
|---|---|---|
| Confirmação | `ACK:<CMD>` | Comando recebido e aceito |
| Erro | `ERR:<CMD>,<motivo>` | Comando inválido/fora de faixa |
| Pong | `PONG:` | Resposta ao `PING` |
| Status | `STS:<campo>=<valor>[;...]` | Telemetria opcional (ex. `STS:hpan=90;htlt=90`) |
| Pronto (boot) | `RDY:<papel>` | MCU inicializou. `papel` = `head` ou `arm` |

### Exemplos

```
Host → ESP32 : HPAN:120\n
ESP32 → Host : ACK:HPAN\n

Host → ESP32 : HGES:nod\n
ESP32 → Host : ACK:HGES\n

Host → Arduino: AGES:wave\n
Arduino → Host: ACK:AGES\n

Host → Arduino: ASHO:200\n          (fora de faixa)
Arduino → Host: ERR:ASHO,range\n

Host → ambos  : PING:\n
MCU  → Host   : PONG:\n
```

## Regras de segurança

- Todo ângulo é **clampeado** à faixa `[0,180]` no firmware antes de mover o servo.
- `STOP:` sempre leva a uma posição segura conhecida e tem prioridade.
- Se o Host não enviar nada por `WATCHDOG_MS` (padrão 3000 ms), o firmware pode opcionalmente
  relaxar torque ou ir para posição de repouso (evita servo forçando indefinidamente).
- O Host valida faixa **antes** de enviar; o firmware valida **de novo** (defesa em profundidade).

## Regra udev sugerida (portas estáveis)

Crie `/etc/udev/rules.d/99-baymax.rules` (ajuste os `idVendor`/`idProduct`/serial reais com `udevadm info`):

```
# ESP32 (CP210x) → /dev/baymax_head
SUBSYSTEM=="tty", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", SYMLINK+="baymax_head"
# Arduino Uno (ATmega16u2) → /dev/baymax_arm
SUBSYSTEM=="tty", ATTRS{idVendor}=="2341", SYMLINK+="baymax_arm"
```

Depois: `sudo udevadm control --reload-rules && sudo udevadm trigger`.

## Versão

Protocolo v1. Mudanças futuras devem incrementar a versão e, se possível, serem
retrocompatíveis (adicionar comandos, não quebrar os existentes).
