-- Aether Synth Wavetable: "Lunar Bell"
-- Formula: Parciais inarmônicas em proporções da razão áurea
-- Character: Percussivo, bell-like, etéreo — como sinos lunares
-- Author: @dbgrecco | Aether Synth Project
--
-- Golden ratio partials: f_n = f0 * phi^n  where phi = 1.6180339887
-- Creates inharmonic bell-like spectrum unlike any traditional waveform
-- Frame position controls partial density and decay shape

function generate(phase, frame)
    local p = phase
    local t = frame / nFrames  -- 0..1
    local phi = 1.6180339887

    -- Number of inharmonic partials (6 to 20 across frames)
    local n_partials = math.floor(6 + t * 14)

    local result = 0.0
    for k = 0, n_partials - 1 do
        -- Golden ratio frequency ratio
        local ratio = phi ^ (k * 0.618)  -- 0.618 = 1/phi (conjugate)

        -- Amplitude envelope: higher partials decay faster
        local amp = (1 / (k + 1)) * math.exp(-k * 0.4 * (1 - t * 0.5))

        -- Phase offset for each partial (creates shimmer spread)
        local phase_offset = k * 0.13 * math.pi

        result = result + amp * math.sin(p * math.pi * 2 * ratio + phase_offset)
    end

    -- Normalize and apply subtle saturation
    local norm = result * 0.5
    return norm / (1 + math.abs(norm) * 0.2)
end

nFrames = 200
nSamples = 2048
