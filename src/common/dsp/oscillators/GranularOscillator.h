/*
 * Surge XT Next-Gen - Aether Audio Engine
 * Granular Synthesis Oscillator Module
 */

#ifndef SURGE_SRC_COMMON_DSP_OSCILLATORS_GRANULAROSCILLATOR_H
#define SURGE_SRC_COMMON_DSP_OSCILLATORS_GRANULAROSCILLATOR_H

#include "Oscillator.h"
#include <vector>
#include <random>

namespace Surge
{
namespace DSP
{

struct AudioGrain
{
    float samplePosition{0.0f};
    float playbackRate{1.0f};
    float grainLengthSamples{1000.0f};
    float currentAgeSamples{0.0f};
    float windowAmplitude{1.0f};
    float panLeft{0.707f};
    float panRight{0.707f};
    bool active{false};
};

class GranularOscillator : public Oscillator
{
  public:
    GranularOscillator(SurgeStorage *storage, SurgeSample *sample, float *pitch, float *mod, bool is_sub);
    ~GranularOscillator() override;

    void init(float pitch, bool is_sub = false) override;
    void process_block(float pitch, float detune = 0.0f, float custom_phase = 0.0f) override;

    void setGrainDensity(float grainsPerSec);
    void setGrainSizeMs(float sizeMs);
    void setPositionSpray(float spray);
    void setPitchJitter(float jitterCents);

  private:
    std::vector<AudioGrain> grainPool;
    std::vector<float> sampleBuffer;
    
    float grainDensityGps{20.0f};
    float grainSizeMs{50.0f};
    float positionSprayAmount{0.1f};
    float pitchJitterAmountCents{0.0f};

    float timeSinceLastGrain{0.0f};
    std::mt19937 randomEngine;

    void spawnNewGrain();
    float calculateHannWindow(float age, float maxAge);
};

} // namespace DSP
} // namespace Surge

#endif // SURGE_SRC_COMMON_DSP_OSCILLATORS_GRANULAROSCILLATOR_H
