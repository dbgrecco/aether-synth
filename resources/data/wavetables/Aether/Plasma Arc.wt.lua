-- Aether Synth Wavetable: "Plasma Arc"
-- Formula: Onda quadrada com PWM via modulação cosseno + harmônico de arco
-- Character: Eletrônico, hard-edged, com energia de arco de plasma
-- Author: @dbgrecco | Aether Synth Project
--
-- PWM controlled by cosine envelope across frames
-- Frame 0: narrow pulse (10% duty) | Frame N: square (50%) + arc harmonics

function generate(phase, frame)
    local p = phase  -- 0..1
    local t = frame / nFrames  -- 0..1

    -- PWM: duty cycle from 0.08 to 0.50 via cosine easing
    local duty = 0.08 + (0.5 - 0.08) * (1 - math.cos(t * math.pi)) / 2

    -- Square/pulse oscillator with bandlimited approximation
    local pulse = 0.0
    local n_harmonics = 32
    for k = 1, n_harmonics do
        local coeff = math.sin(k * math.pi * duty) / (k * math.pi / 2)
        pulse = pulse + coeff * math.cos(p * math.pi * 2 * k)
    end

    -- Arc discharge artifact: high-frequency sizzle at frame edges
    local arc_freq = 7 + t * 11  -- 7 to 18 sub-harmonics
    local arc = math.sin(p * math.pi * 2 * arc_freq) * 0.15 * (1 - math.abs(t - 0.5) * 2)

    -- Voltage transient spike: asymmetric attack per cycle
    local spike = math.exp(-math.fmod(p * 8, 1) * 6) * 0.1 * t

    return (pulse * 0.75 + arc + spike) * 0.8
end

nFrames = 150
nSamples = 2048
