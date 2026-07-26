/*
 * Surge XT Next-Gen - Aether Audio Engine
 * AI Audio-to-Synth Matcher & Spectral Analysis Utilities
 */

#ifndef SURGE_SRC_COMMON_DSP_UTILITIES_AUDIOTOSYNTHMATCHER_H
#define SURGE_SRC_COMMON_DSP_UTILITIES_AUDIOTOSYNTHMATCHER_H

#include "juce_audio_basics/juce_audio_basics.h"
#include <vector>
#include <string>

namespace Surge
{
namespace DSP
{

struct SynthMatchResult
{
    std::string suggestedOscType{"Wavetable"};
    int detectedFundamentalHz{440};
    float estimatedBrightness{0.5f};
    float estimatedAttackMs{10.0f};
    float estimatedReleaseMs{400.0f};
    std::vector<std::vector<float>> extractedWavetable;
};

class AudioToSynthMatcher
{
  public:
    AudioToSynthMatcher();
    ~AudioToSynthMatcher();

    SynthMatchResult analyzeAudioBuffer(const float *samples, size_t numSamples, double sampleRate);
    void convertToWavetable(const float *samples, size_t numSamples, std::vector<std::vector<float>> &outFrames);

  private:
    float computeRMS(const float *samples, size_t numSamples);
    float estimateSpectralCentroid(const float *samples, size_t numSamples, double sampleRate);
};

} // namespace DSP
} // namespace Surge

#endif // SURGE_SRC_COMMON_DSP_UTILITIES_AUDIOTOSYNTHMATCHER_H
