/*
 * Surge XT Next-Gen - Aether Audio Engine
 * Zero-Delay Feedback (ZDF) & Formant Vowel Filter Suite
 */

#ifndef SURGE_SRC_COMMON_DSP_FILTERS_ZDFFILTERSUITE_H
#define SURGE_SRC_COMMON_DSP_FILTERS_ZDFFILTERSUITE_H

#include "juce_audio_basics/juce_audio_basics.h"
#include <vector>

namespace Surge
{
namespace DSP
{

enum class ZDFFilterType
{
    MoogTransistor4Pole,
    DiodeLadder,
    FormantVowel
};

struct VowelFormant
{
    float f1, f2, f3; // Formant frequencies for A, E, I, O, U
};

class ZDFFilterSuite
{
  public:
    ZDFFilterSuite();
    ~ZDFFilterSuite();

    void init(double sampleRate);
    void setFilterType(ZDFFilterType type);
    void setCutoff(float cutoffHz);
    void setResonance(float res);
    void setVowelMorph(float morph); // 0.0 = A, 0.25 = E, 0.5 = I, 0.75 = O, 1.0 = U

    float processSample(float input);

  private:
    double sr{44100.0};
    ZDFFilterType currentType{ZDFFilterType::MoogTransistor4Pole};
    
    float cutoff{1000.0f};
    float resonance{0.5f};
    float vowelPosition{0.0f};

    // Filter internal states (s1, s2, s3, s4)
    float s1{0.0f}, s2{0.0f}, s3{0.0f}, s4{0.0f};

    // Formant Biquads
    float formS1[3]{0.0f}, formS2[3]{0.0f};

    float processMoogZDF(float input);
    float processVowelFormant(float input);
};

} // namespace DSP
} // namespace Surge

#endif // SURGE_SRC_COMMON_DSP_FILTERS_ZDFFILTERSUITE_H
