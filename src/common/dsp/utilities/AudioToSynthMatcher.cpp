/*
 * Surge XT Next-Gen - Aether Audio Engine
 * Implementation of AI Audio-to-Synth Matcher & Spectral Analysis Utilities
 */

#include "AudioToSynthMatcher.h"
#include <cmath>
#include <algorithm>

namespace Surge
{
namespace DSP
{

AudioToSynthMatcher::AudioToSynthMatcher() {}
AudioToSynthMatcher::~AudioToSynthMatcher() {}

float AudioToSynthMatcher::computeRMS(const float *samples, size_t numSamples)
{
    if (numSamples == 0) return 0.0f;
    float sumSq = 0.0f;
    for (size_t i = 0; i < numSamples; ++i)
    {
        sumSq += samples[i] * samples[i];
    }
    return std::sqrt(sumSq / static_cast<float>(numSamples));
}

float AudioToSynthMatcher::estimateSpectralCentroid(const float *samples, size_t numSamples, double sampleRate)
{
    if (numSamples == 0) return 1000.0f;
    // Simple zero-crossing / brightness estimation heuristic
    size_t zeroCrossings = 0;
    for (size_t i = 1; i < numSamples; ++i)
    {
        if ((samples[i - 1] < 0.0f && samples[i] >= 0.0f) || (samples[i - 1] >= 0.0f && samples[i] < 0.0f))
        {
            zeroCrossings++;
        }
    }
    float approxFreq = (static_cast<float>(zeroCrossings) * static_cast<float>(sampleRate)) / (2.0f * numSamples);
    return std::min(20000.0f, approxFreq);
}

SynthMatchResult AudioToSynthMatcher::analyzeAudioBuffer(const float *samples, size_t numSamples, double sampleRate)
{
    SynthMatchResult result;
    if (numSamples == 0 || samples == nullptr)
        return result;

    float centroid = estimateSpectralCentroid(samples, numSamples, sampleRate);
    result.estimatedBrightness = std::min(1.0f, centroid / 8000.0f);

    if (centroid > 3000.0f)
    {
        result.suggestedOscType = "Wavetable Bright / FM";
    }
    else if (centroid < 500.0f)
    {
        result.suggestedOscType = "Sub / Warm Sine";
    }
    else
    {
        result.suggestedOscType = "Classic Saw / Pulse";
    }

    convertToWavetable(samples, numSamples, result.extractedWavetable);
    return result;
}

void AudioToSynthMatcher::convertToWavetable(const float *samples, size_t numSamples, std::vector<std::vector<float>> &outFrames)
{
    size_t frameLength = 256;
    size_t numFrames = 16;
    outFrames.resize(numFrames);

    for (size_t f = 0; f < numFrames; ++f)
    {
        outFrames[f].resize(frameLength, 0.0f);
        size_t startOffset = f * (numSamples / (numFrames + 1));

        for (size_t i = 0; i < frameLength; ++i)
        {
            if (startOffset + i < numSamples)
            {
                outFrames[f][i] = samples[startOffset + i];
            }
        }
    }
}

} // namespace DSP
} // namespace Surge
