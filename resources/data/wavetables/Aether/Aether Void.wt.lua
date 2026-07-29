-- Aether Synth Wavetable: "Aether Void"
-- Formula: Chebyshev polynomial T8 com modulação de fase quântica
-- Character: Metálico, etéreo, com harmônicos superiores proeminentes
-- Author: @dbgrecco | Aether Synth Project
--
-- Chebychev T8(x) = 128x^8 - 256x^6 + 160x^4 - 32x^2 + 1
-- Modulated by quantum-phase offset across wavetable frames

function generate(phase, frame)
    local x = math.cos(phase * math.pi * 2)
    -- Chebyshev T8 evaluation
    local x2 = x * x
    local x4 = x2 * x2
    local x6 = x4 * x2
    local x8 = x4 * x4
    local cheb = 128*x8 - 256*x6 + 160*x4 - 32*x2 + 1

    -- Quantum phase offset: each frame introduces a harmonic phase warp
    local quantum = frame / nFrames
    local phase_warp = math.sin(phase * math.pi * 2 * 5 + quantum * math.pi * 3)
    local modulation = 0.3 * math.sin(phase * math.pi * 2 * 13 * (1 + quantum * 0.7))

    return (cheb * 0.6 + phase_warp * 0.25 + modulation * 0.15) * 0.85
end

nFrames = 256
nSamples = 2048
