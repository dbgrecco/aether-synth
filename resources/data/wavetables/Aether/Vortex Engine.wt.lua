-- Aether Synth Wavetable: "Vortex Engine"
-- Formula: Espiral de frequência logarítmica + rotação de fase vetorial
-- Character: Cinético, tenso, como um motor em aceleração constante
-- Author: @dbgrecco | Aether Synth Project
--
-- Logarithmic frequency spiral: each frame rotates the spectral phase by a
-- fixed angle, creating a "spinning" timbral evolution
-- Inspired by rotating magnetic field patterns in induction motors

function generate(phase, frame)
    local p = phase
    local t = frame / nFrames  -- 0..1

    -- Spectral rotation angle (full revolution = 256 frames)
    local rotation = t * math.pi * 2

    local result = 0.0
    local n_partials = 20

    for k = 1, n_partials do
        -- Logarithmic frequency spacing (approximates critical band spacing)
        local log_freq = math.log(k + 1) / math.log(2)  -- log2(k+1)
        local freq = 1 + log_freq * k * 0.5

        -- Phase rotation per harmonic
        local rotated_phase = p * freq + rotation * k * 0.08

        -- Amplitude: inverse log for bright character
        local amp = 1 / (1 + math.log(k))

        -- Vectorial phase offset (90° stagger between partials)
        local vector_phase = k * math.pi * 0.5
        local s = math.sin(rotated_phase * math.pi * 2 + vector_phase)

        result = result + amp * s
    end

    -- Dynamic normalization with vortex saturation
    local vortex = result * 0.35
    return math.tanh(vortex * (1 + t * 0.8)) * 0.85
end

nFrames = 256
nSamples = 2048
