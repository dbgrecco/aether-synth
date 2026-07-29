-- Aether Synth Wavetable: "Crystal Matrix"
-- Formula: Superposição de ondas triangulares com defasagem matricial
-- Character: Cristalino, brilhante, com shimmer multi-dimensional
-- Author: @dbgrecco | Aether Synth Project
--
-- Matrix of triangle waves offset by irrational phases (e, pi, sqrt(2))
-- Creates a shimmer effect impossible with standard synthesis

local IRRATIONAL = {
    math.exp(1),         -- e  ≈ 2.71828
    math.pi,             -- π  ≈ 3.14159
    math.sqrt(2),        -- √2 ≈ 1.41421
    math.sqrt(3),        -- √3 ≈ 1.73205
    (math.sqrt(5) + 1) / 2,  -- φ ≈ 1.61803
}

function triangle(phase)
    -- Perfect triangle wave (2*|2*(p-floor(p+0.5))|-1)
    local p = phase - math.floor(phase + 0.5)
    return 4 * math.abs(p) - 1
end

function generate(phase, frame)
    local p = phase
    local t = frame / nFrames  -- 0..1

    local result = 0.0
    local n_layers = math.floor(2 + t * 3)  -- 2 to 5 matrix layers

    for i = 1, n_layers do
        local ir   = IRRATIONAL[i]
        -- Frequency: irrational ratio for non-repeating interference
        local freq = ir - math.floor(ir) + i  -- fractional part + integer layer
        local phase_offset = t * ir * 0.25
        local amp  = 1 / i

        result = result + amp * triangle(p * freq + phase_offset)
    end

    -- Matrix-wide modulation: all layers breathe together
    local breathe = 1 + 0.05 * math.sin(t * math.pi * 4)
    return result / n_layers * breathe * 0.88
end

nFrames = 180
nSamples = 2048
