# Aether Synth

**Sintetizador híbrido next-gen — VST3 / AU / CLAP / LV2 / Standalone**

[![CI Build Status](https://dev.azure.com/surge-synthesizer/surge/_apis/build/status/surge-synthesizer.surge?branchName=main)](https://dev.azure.com/surge-synthesizer/surge/_build/latest?definitionId=2&branchName=main)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-lightgrey)](#downloads)

> Aether Synth é um fork customizado do [Surge XT](https://surge-synthesizer.github.io/) com tema visual próprio, paleta de cores Aether (cyan/roxo), presets exclusivos e uma landing page interativa com sintetizador funcional no navegador.

---

## ✨ O que é diferente do Surge XT original

| Feature | Aether Synth | Surge XT |
|---|---|---|
| Tema visual Aether (cyan/purple) | ✅ | ❌ |
| Paleta de cores customizada em `SkinColors.cpp` | ✅ | ❌ |
| Landing page interativa com Web Audio synth | ✅ | ❌ |
| Visualizador 3D de wavetables no browser | ✅ | ❌ |
| Presets exclusivos (Aether Pack) | ✅ | ❌ |
| Joystick vetorial interativo (web demo) | ✅ | ❌ |
| Branding e identidade visual próprios | ✅ | ❌ |

---

## 🎛️ Funcionalidades

### Motor de Síntese (herdado do Surge XT, estado da arte)
- **15 tipos de osciladores**: Wavetable, Classic, Modern, Granular, Sine, FM2, FM3, S&H, String, Twist, Spectral, Window, Alias, AudioInput
- **Síntese granular** com controle de densidade, spray e jitter
- **Síntese espectral aditiva** com 512 parciais harmônicos
- **Wavetable scripting** via LuaJIT — crie tabelas de ondas programaticamente
- **Filtros ZDF** (Zero Delay Feedback): Moog Ladder, Diode, Comb, Formante Vocal

### Modulação
- **12 LFOs** por cena com formas de onda sincronizadas ao BPM
- **Editor MSEG** (Multi-Segment Envelope Generator) com curvas Bézier
- **Matriz de modulação** com conexões ilimitadas
- **Suporte a MPE** (MIDI Polyphonic Expression)
- **Microafinamento Scala** (SCL/KBM)

### Efeitos
- **Shimmer Reverb** — loop de realimentação com pitch-shifting harmônico
- **Nimbus** (baseado no Clouds do Eurorack)
- **BBD Ensemble** (simulação analógica de bucket-brigade)
- **OTT Dynamics** — compressor multibanda upward/downward
- **Combulator**, **Waveshaper**, **Frequency Shifter**, **Ring Modulator**, **Vocoder**, **Rotary Speaker** e mais

### Compatibilidade
- Formatos: **VST3**, **AU** (macOS), **CLAP**, **LV2**, **Standalone**
- Plataformas: **Windows 10/11**, **macOS 10.13+** (Universal Binary ARM/Intel), **Linux** (x86_64, aarch64, Raspberry Pi 64-bit)
- Suporte a **ASIO** (Windows, build pessoal)
- **Bindings Python** via pybind11 para automação e análise

---

## 🎨 Tema Visual Aether

O Aether Synth utiliza uma paleta de cores customizada definida em [`src/common/SkinColors.cpp`](src/common/SkinColors.cpp):

| Token | Cor | Uso |
|---|---|---|
| Aether Cyan | `#00E5FF` | Waveforms, controles ativos, accent principal |
| Aether Purple | `#7C4DFF` | Seleções, mod sources armados, gradientes |
| Aether Dark | `#090B10` | Background principal |
| Aether Gold | `#FFD700` | Macro controls, valores destacados |

O sistema de skin do Surge XT permite sobrescrever qualquer cor da interface via arquivo `.surge-skin` ou diretamente no código — veja [`src/common/SkinModel.cpp`](src/common/SkinModel.cpp) para referência.

---

## 🌐 Landing Page / Web Demo

A pasta [`website/`](website/) contém uma landing page completamente independente com:

- **Sintetizador funcional no browser** (Web Audio API — sem instalação)
- **Visualizador 3D de wavetables** renderizado em Canvas 2D com projeção perspectiva
- **4 presets de demo**: Cyberpunk Saw Lead, Ambient Shimmer Pad, Deep Acid Sub Bass, Granular Bell Texture
- **Joystick vetorial** tipo Korg para controle de Pitch Bend e Mod Wheel
- **Design glassmorphism** com tema Aether completo

Para rodar localmente:
```bash
cd website
# qualquer servidor estático funciona:
npx serve .
# ou
python -m http.server 8080
```

---

## 🔨 Build do Plugin

### Pré-requisitos

**Windows**: Git, Visual Studio 2017+ (com "Desktop development with C++")

**macOS**: Xcode, Xcode Command Line Tools, CMake (`brew install cmake`)

**Linux**:
```bash
sudo apt install build-essential git cmake libcairo-dev libxkbcommon-x11-dev \
  libxkbcommon-dev libxcb-cursor-dev libxcb-keysyms1-dev libxcb-util-dev \
  libxrandr-dev libxinerama-dev libxcursor-dev libasound2-dev libjack-jackd2-dev
```

### Build padrão

```bash
git clone https://github.com/dbgrecco/aether-synth.git
cd aether-synth
git submodule update --init --recursive
cmake -Bbuild
cmake --build build --config Release --target surge-staged-assets
```

Os binários ficam em `build/surge_xt_products/`.

### Opções úteis

```bash
# Copiar plugin automaticamente após build
cmake -Bbuild -DSURGE_COPY_AFTER_BUILD=True

# Build com LV2
cmake -Bbuild -DSURGE_BUILD_LV2=TRUE

# Build com Python bindings
cmake -Bbuild -DSURGE_BUILD_PYTHON_BINDINGS=ON

# Build com ASIO (Windows, uso pessoal)
cmake -Bbuild -DBUILD_USING_MY_ASIO_LICENSE=True
```

### Fat binary macOS (ARM + Intel)

```bash
cmake -Bbuild -D"CMAKE_OSX_ARCHITECTURES=arm64;x86_64"
```

---

## 📦 Downloads

Visite a [página oficial](https://github.com/dbgrecco/aether-synth/releases) para baixar os binários pré-compilados para Windows (.msi), macOS (.dmg) e Linux (.tar.gz).

---

## 🤝 Contribuindo

Pull requests são bem-vindos! Áreas de interesse:

- **Novos presets** (`.fxp` / formato Surge)
- **Novas wavetables** (scripts Lua via `WavetableScriptEvaluator`)
- **Melhorias no skin** (`SkinColors.cpp`, `SkinModel.cpp`)
- **Novos efeitos** derivados da cadeia em `src/common/dsp/effects/`

Para discussões, abra uma [Issue](https://github.com/dbgrecco/aether-synth/issues) ou visite o [Discord do Surge Synth Team](https://surge-synthesizer.github.io/).

---

## ⚖️ Licença

Aether Synth é licenciado sob **GNU General Public License v3** (GPL-3.0-or-later).

Este projeto é um fork do [Surge XT](https://github.com/surge-synthesizer/surge), criado originalmente por Claes Johanson (Vember Audio) e mantido pela comunidade Surge Synth Team desde 2018. Todo o crédito pela engine de síntese pertence aos autores originais — veja [AUTHORS](AUTHORS) e [LICENSE](LICENSE).

```
Aether Synth — fork e customização por @dbgrecco
Base: Surge XT © 2018-2024 Surge Synth Team (GPL-3.0)
```
