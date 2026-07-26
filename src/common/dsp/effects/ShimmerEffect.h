/*
 * Surge XT Next-Gen - Aether Audio Engine
 * Shimmer Reverb Effect Engine (High-Octave Pitch Shifted Reverb)
 */

#ifndef SURGE_SRC_COMMON_DSP_EFFECTS_SHIMMEREFFECT_H
#define SURGE_SRC_COMMON_DSP_EFFECTS_SHIMMEREFFECT_H

#include "Effect.h"
#include <vector>

namespace Surge
{
namespace DSP
{

class ShimmerEffect : public Effect
{
  public:
    ShimmerEffect(SurgeStorage *storage, SurgeFxUnit *fxdata, psp_type type);
    ~ShimmerEffect() override;

    void init() override;
    void process(float *inoutL, float *inoutR) override;

    void setDecay(float decaySeconds);
    void setPitchShiftSemi(float semitones);
    void setShimmerMix(float mix);

  private:
    std::vector<float> delayLineL;
    std::vector<float> delayLineR;
    size_t writePosL{0};
    size_t writePosR{0};

    float decayFactor{0.85f};
    float pitchShiftFactor{2.0f}; // 1 octave pitch shift up by default
    float mixAmount{0.5f};

    float pitchShiftPhaseL{0.0f};
    float pitchShiftPhaseR{0.0f};
};

} // namespace DSP
} // namespace Surge

#endif // SURGE_SRC_COMMON_DSP_EFFECTS_SHIMMEREFFECT_H
