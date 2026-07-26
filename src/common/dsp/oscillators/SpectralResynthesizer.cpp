/*
 * Surge XT Next-Gen - Aether Audio Engine
 * Implementation of Spectral Additive Resynthesizer Oscillator Engine
 */

#include "SpectralResynthesizer.h"
#include "SurgeStorage.h"
#include <cmath>

namespace Surge
{
namespace DSP
{

SpectralResynthesizer::SpectralResynthesizer(SurgeStorage *storage, SurgeSample *sample, float *pitch, float *mod, bool is_sub)
    : Oscillator(storage, sample, pitch, mod, is_sub)
{
    partialsGrid.resize(64); // 64 primary audible partials
    updateHarmonicSpectrum();
}

SpectralResynthesizer::~SpectralResynthesizer() {}

void SpectralResynthesizer::init(float pitch, bool is_sub)
{
    for (size_t i = 0; i < partialsGrid.size(); ++i)
    {
        partialsGrid[i].phase = 0.0f;
    }
}

void SpectralResynthesizer::setSpectralBrightness(float brightness)
{
    brightnessFactor = juce::jlimit(0.1f, 3.0f, brightness);
    updateHarmonicSpectrum();
}

void SpectralResynthesizer::setOddEvenBalance(float balance)
{
    oddEvenRatio = juce::jlimit(0.0f, 1.0f, balance);
    updateHarmonicSpectrum();
}

void SpectralResynthesizer::setSpectralTilt(float tiltDbPerOct)
{
    spectralTiltDb = tiltDbPerOct;
    updateHarmonicSpectrum();
}

void SpectralResynthesizer::updateHarmonicSpectrum()
{
    for (size_t i = 0; i < partialsGrid.size(); ++i)
    {
        float h = static_cast<float>(i + 1);
        partialsGrid[i].harmonicRatio = h;

        bool isOdd = (static_cast<int>(h) % 2 != 0);
        float balanceScale = isOdd ? (1.0f - oddEvenRatio) : oddEvenRatio;

        // Spectral roll-off attenuation formula
        float octaves = std::log2(h);
        float tiltFactor = std::pow(10.0f, (spectralTiltDb * octaves) / 20.0f);

        partialsGrid[i].amplitude = (1.0f / (h * brightnessFactor)) * balanceScale * tiltFactor;
    }
}

void SpectralResynthesizer::process_block(float pitch, float detune, float custom_phase)
{
    float sr = storage->samplerate;
    float fundamentalHz = pitch_to_frequency(pitch + detune);
    float nyquist = sr * 0.49f;

    // Clear output buffers
    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        outputL[i] = 0.0f;
        outputR[i] = 0.0f;
    }

    // Synthesize partials
    for (auto &partial : partialsGrid)
    {
        float freqHz = fundamentalHz * partial.harmonicRatio;
        if (freqHz >= nyquist || partial.amplitude <= 1e-4f)
            continue;

        float phaseInc = (2.0f * juce::MathConstants<float>::pi * freqHz) / sr;

        for (int i = 0; i < BLOCK_SIZE; ++i)
        {
            float val = std::sin(partial.phase) * partial.amplitude;
            outputL[i] += val;
            outputR[i] += val;

            partial.phase += phaseInc;
            if (partial.phase >= 2.0f * juce::MathConstants<float>::pi)
            {
                partial.phase -= 2.0f * juce::MathConstants<float>::pi;
            }
        }
    }
}

} // namespace DSP
} // namespace Surge
