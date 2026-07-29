# Plano de Expansão Aether Synth: Módulos Baseados no Fathom Pro

Este documento traz o planejamento técnico detalhado para a implementação da próxima fase de expansão do **Aether Synth**, inspirada nos recursos de ponta e bibliotecas de wavetables do sintetizador **Fathom Pro (v2.3.2)**.

---

## 🧭 Visão Geral das 3 Adições

As 3 adições foram estruturadas para não poluir nem confundir a interface, atuando em camadas complementares da engine:

1. **`Quantum Curve`** (Módulo DSP de Efeito): Waveshaping baseado em curvas Bézier.
2. **`Vocalis Formant Warp`** (Módulo DSP de Efeito): Filtro ressonante de formantes vocais humanos.
3. **`SQ-80 Heritage Pack`** (Expansão de Wavetables): Tabelas de onda de 8-bit inspiradas no Ensoniq SQ-80.

---

## 🛠️ Especificação Detalhada de Implementação

### 🎨 1. Módulo DSP: `Quantum Curve` (`QuantumCurveEffect`)

#### Conceito
Remodelador de onda (*waveshaper*) de alta precisão alimentado por matemática de curvas Bézier. Transforma formas de onda senoidais/dente-de-serra em perfis arredondados ou pontiagudos (*Katana Edge*), gerando harmônicos ricos sem aliasing digital.

#### Arquivos a Criar/Modificar
- **[NEW] `src/common/dsp/effects/QuantumCurveEffect.h`**
- **[NEW] `src/common/dsp/effects/QuantumCurveEffect.cpp`**
- **[MODIFY] `src/common/SurgeStorage.h`**: Adicionar `fxt_quantum_curve`, `"Quantum Curve"`, `"Q-CURVE"`.
- **[MODIFY] `src/common/dsp/Effect.cpp`**: Registrar `case fxt_quantum_curve` no `spawn_effect`.
- **[MODIFY] `src/common/CMakeLists.txt`**: Incluir fontes na compilação.

#### Lista de Parâmetros
| Parâmetro | Tipo | Descrição |
|---|---|---|
| `qc_tension` | `ct_percent` | Tensão da curva Bézier (0% = linear, 100% = dobra acentuada) |
| `qc_asymmetry` | `ct_percent_bipolar` | Deformação assimétrica entre o ciclo positivo e negativo |
| `qc_pinch` | `ct_percent` | Achatamento harmônico dos vértices (estilo Katana) |
| `qc_drive` | `ct_percent` | Ganho de entrada pré-shaper |
| `qc_mix` | `ct_percent` | Mixagem Dry/Wet |

---

### 🗣️ 2. Módulo DSP: `Vocalis Formant Warp` (`VocalisWarpEffect`)

#### Conceito
Filtro ressonante de formantes com 3 picos de frequência variáveis em paralelo. Simula o trato vocal humano e permite transições contínuas entre vogais (A - E - I - O - U), inspirando-se nas tabelas `SQ80 Formant` e `Voice` do Fathom Pro.

#### Arquivos a Criar/Modificar
- **[NEW] `src/common/dsp/effects/VocalisWarpEffect.h`**
- **[NEW] `src/common/dsp/effects/VocalisWarpEffect.cpp`**
- **[MODIFY] `src/common/SurgeStorage.h`**: Adicionar `fxt_vocalis_warp`, `"Vocalis Warp"`, `"VOCAL"`.
- **[MODIFY] `src/common/dsp/Effect.cpp`**: Registrar `case fxt_vocalis_warp` no `spawn_effect`.
- **[MODIFY] `src/common/CMakeLists.txt`**: Incluir fontes na compilação.

#### Lista de Parâmetros
| Parâmetro | Tipo | Descrição |
|---|---|---|
| `vw_vowel` | `ct_percent` | Varredura contínua entre as vogais A, E, I, O, U |
| `vw_throat` | `ct_percent_bipolar` | Tamanho da cavidade vocal (deslocamento de frequência dos formantes) |
| `vw_resonance` | `ct_percent` | Fator Q de ressonância dos filtros de formante |
| `vw_gender` | `ct_percent_bipolar` | Ajuste de timbre Masculino (-50%) / Feminino (+50%) |
| `vw_mix` | `ct_percent` | Mixagem Dry/Wet |

---

### 🌊 3. Expansão de Wavetables: `SQ-80 Heritage Pack`

#### Conceito
Presets de tabelas de onda programadas em scripts LuaJIT para geração nativa de formas de onda clássicas do sintetizador Ensoniq SQ-80 de 1987.

#### Arquivos a Criar na pasta `resources/data/wavetables/Aether/`
- **`SQ80 Heritage Aliens.wt.lua`**: Texturas alienígenas ricas em inharmônicos.
- **`SQ80 Heritage Formant.wt.lua`**: Varredura vocal clássica dos anos 80.
- **`SQ80 Heritage Brass.wt.lua`**: Metais sintetizados analógico-digitais de 8-bit.
- **`SQ80 Heritage Reed.wt.lua`**: Palhetas e sopros vintage.

---

## 🌐 4. Atualizações na Landing Page Web (`website/`)

1. Adicionar os novos cards nas funcionalidades (`website/index.html`):
   - **Quantum Curve**: Waveshaper Bézier de alta precisão.
   - **Vocalis Formant Warp**: Filtro e sintetizador de vogais humanas.
   - **SQ-80 Heritage Pack**: Wavetables de 8-bit dos anos 80.
2. Adicionar os seletores interativos na Web Audio API em `website/app.js`.

---

## 📊 Roteiro para a Próxima Sessão

1. Criar os arquivos de C++ do `QuantumCurveEffect`.
2. Criar os arquivos de C++ do `VocalisWarpEffect`.
3. Adicionar as tabelas `.wt.lua` na pasta de recursos.
4. Atualizar a Landing Page Web.
5. Executar `git add`, `git commit` e `git push` para sincronizar no GitHub.
