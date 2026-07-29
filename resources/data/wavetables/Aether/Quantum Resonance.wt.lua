-- Aether Synth Wavetable: "Quantum Resonance"
-- Formula: Série de Fourier com batimentos quânticos entre osciladores primos
-- Character: Hipnótico, oscilante, com batimentos que evoluem por frames
-- Author: @dbgrecco | Aether Synth Project
--
-- Prime number harmonics create complex beating patterns
-- Primes: 2, 3, 5, 7, 11, 13 — irrational interference across frames

local primes = {2, 3, 5, 7, 11, 13, 17, 19}

function generate(phase, frame)
    local p = phase
    local t = frame / nFrames  -- 0..1

    local result = 0.0
    local norm   = 0.0

    for i, prime in ipairs(primes) do
        -- Each prime harmonic has a unique phase that evolves with frame
        -- The difference between adjacent primes creates beating
        local phase_drift = t * math.pi * (prime % 3 + 1) * 0.5
        local amp = 1 / math.sqrt(prime)  -- amplitude inversely proportional to sqrt(prime)

        -- Beat frequency between this prime and the next
        local beat_rate = i < #primes and (primes[i+1] - prime) or 1
        local beat_mod  = 0.85 + 0.15 * math.cos(t * math.pi * 2 * beat_rate)

        result = result + amp * beat_mod * math.sin(p * math.pi * 2 * prime + phase_drift)
        norm   = norm + amp
    end

    -- Apply soft saturation for warmth
    local out = result / norm
    return out * 0.9 / (1 + math.abs(out) * 0.12)
end

nFrames = 300
nSamples = 2048
