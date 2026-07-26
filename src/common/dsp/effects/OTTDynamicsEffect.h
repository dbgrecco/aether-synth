/*
 * Surge XT Next-Gen - Aether Audio Engine
 * OTT Multiband Compressor & Upward/Downward Dynamics Processor
 */

#ifndef SURGE_SRC_COMMON_DSP_EFFECTS_OTTDYNAMICSEFFECT_H
#define SURGE_SRC_COMMON_DSP_EFFECTS_OTTDYNAMICSEFFECT_H

#include "Effect.h"
#include <vector>

namespace Surge
{
namespace DSP
{

struct BandDynamics
{
    float thresholdDb{-18.0f};
    float ratioUpward{4.0f};
    float ratioDownward{8.0f};
    float attackMs{10.0f};
    float releaseMs{100.0f};
    float makeupGainDb{0.0f};
    float currentEnvelope{0.0f};
};

class OTTDynamicsEffect : public Effect
{
  public:
    OTTDynamicsEffect(SurgeStorage *storage, SurgeFxUnit *fxdata, psp_type type);
    ~OTTDynamicsEffect() override;

    void init() override;
    void process(float *inoutL, float *inoutR) override;

    void setLowCrossHz(float hz);
    void setHighCrossHz(float hz);
    void setDepth(float depth);

  private:
    BandDynamics lowBand;
    BandDynamics midBand;
    BandDynamics highBand;

    float lowCrossoverHz{250.0f};
    float highCrossoverHz{2500.0f};
    float depthAmount{1.0f};

    // Filter memory state for 3-band crossover
    float lpLowL{0.0f}, lpLowR{0.0f};
    float hpHighL{0.0f}, hpHighR{0.0f};

    float processBandDynamics(BandDynamics &band, float inputSample, float sr);
};

} // namespace DSP
} // namespace Surge

#endif // SURGE_SRC_COMMON_DSP_EFFECTS_OTTDYNAMICSEFFECT_H
