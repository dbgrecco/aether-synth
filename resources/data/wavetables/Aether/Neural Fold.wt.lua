-- Aether Synth Wavetable: "Neural Fold"
-- Formula: Sigmoid neural activation + cosine distortion folding
-- Character: IA-glitchy, digital, com dobramento de onda progressivo
-- Author: @dbgrecco | Aether Synth Project
--
-- Inspired by neural network activation functions applied to audio
-- Frame 0: pure sigmoid | Frame N: deeply folded cosine cascade

function sigmoid(x, gain)
    return 2 / (1 + math.exp(-gain * x)) - 1
end

function wavefold(x, amount)
    -- Cosine-based wave folding (smoother than hard clipping)
    local folded = x * amount
    if math.abs(folded) > 1 then
        -- Fold back using cosine for smooth discontinuities
        folded = math.cos((math.abs(folded) - 1) * math.pi * 0.5) * math.sign(folded or 1)
    end
    return folded
end

function generate(phase, frame)
    local p = phase * 2 - 1  -- -1 to 1
    local t = frame / nFrames  -- 0..1

    -- Base: sigmoid-shaped sine
    local base = math.sin(p * math.pi)
    local sig  = sigmoid(base, 2 + t * 8)

    -- Progressive folding amount: 1.0 (no fold) to 4.5 (heavy fold)
    local fold_amount = 1.0 + t * 3.5

    -- Apply cosine folding
    local folded = math.cos(sig * fold_amount * math.pi) * 0.7

    -- Add harmonically related fold artifact for richness
    local harmonic_fold = math.cos(sig * fold_amount * 2.0 * math.pi + t * math.pi) * 0.2 * t

    return (folded + harmonic_fold) * 0.85
end

nFrames = 256
nSamples = 2048
