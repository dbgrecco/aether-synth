/*
 * Surge XT Next-Gen - Aether Audio Engine
 * Spectral Additive Resynthesizer Oscillator Engine (512 Partial Grid)
 */

#ifndef SURGE_SRC_COMMON_DSP_OSCILLATORS_SPECTRALRESYNTHESIZER_H
#define SURGE_SRC_COMMON_DSP_OSCILLATORS_SPECTRALRESYNTHESIZER_H

#include "Oscillator.h"
#include <vector>

namespace Surge
{
namespace DSP
{

struct AdditivePartial
{
    float harmonicRatio{1.0f};
    float amplitude{1.0f};
    float phase{0.0f};
};

class SpectralResynthesizer : public Oscillator
{
  public:
    SpectralResynthesizer(SurgeStorage *storage, SurgeSample *sample, float *pitch, float *mod, bool is_sub);
    ~SpectralResynthesizer() override;

    void init(float pitch, bool is_sub = false) override;
    void process_block(float pitch, float detune = 0.0f, float custom_phase = 0.0f) override;

    void setSpectralBrightness(float brightness);
    void setOddEvenBalance(float balance);
    void setSpectralTilt(float tiltDbPerOct);

  private:
    std::vector<AdditivePartial> partialsGrid;
    float brightnessFactor{1.0f};
    float oddEvenRatio{0.5f};
    float spectralTiltDb{-3.0f};

    void updateHarmonicSpectrum();
};

} // namespace DSP
} // namespace Surge

#endif // SURGE_SRC_COMMON_DSP_OSCILLATORS_SPECTRALRESYNTHESIZER_H
