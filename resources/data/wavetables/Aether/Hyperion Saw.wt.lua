-- Aether Synth Wavetable: "Hyperion Saw"
-- Formula: Dente de serra com harmônicos em proporções Fibonacci
-- Character: Analógico-perfeito, rico em harmônicos naturalmente espaçados
-- Author: @dbgrecco | Aether Synth Project
--
-- Fibonacci ratios for harmonic spacing: 1, 1, 2, 3, 5, 8, 13, 21...
-- Each frame morphs the balance between pure saw and Fibonacci-spaced partials

function generate(phase, frame)
    local p = phase  -- 0..1
    local t = frame / nFrames  -- morphing parameter 0..1

    -- Classic bandlimited sawtooth approximation (N harmonics)
    local saw = 0.0
    local fib = {1, 1, 2, 3, 5, 8, 13, 21, 34}
    local n_harm = math.floor(8 + t * 24)  -- 8 to 32 harmonics

    for k = 1, n_harm do
        -- Morph from integer harmonics (pure saw) to Fibonacci harmonics
        local h_int = k
        local h_fib = fib[math.min(k, #fib)] * k / fib[math.min(k, #fib)]
        local harmonic = h_int * (1 - t) + h_fib * t

        -- Small golden ratio detune per frame for analog warmth
        local phi = 1.6180339887
        local detune = 1 + (t * 0.003 * math.sin(k * phi))

        saw = saw + (1 / k) * math.sin(p * math.pi * 2 * harmonic * detune)
    end

    -- Normalize to prevent clipping
    return (2 / math.pi) * saw * 0.7
end

nFrames = 128
nSamples = 2048
