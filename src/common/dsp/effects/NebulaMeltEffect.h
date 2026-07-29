/*
 * Aether Synth - Next-Gen Hybrid Synthesizer
 * Nebula Melt Lo-Fi & Vintage Tape Processor Effect
 */

#ifndef SURGE_SRC_COMMON_DSP_EFFECTS_NEBULAMELTEFFECT_H
#define SURGE_SRC_COMMON_DSP_EFFECTS_NEBULAMELTEFFECT_H

#include "Effect.h"
#include "dsp/filters/BiquadFilter.h"
#include "DSPUtils.h"
#include <vembertech/lipol.h>

class NebulaMeltEffect : public Effect
{
  public:
    NebulaMeltEffect(SurgeStorage *storage, FxStorage *fxdata, pdata *pd);
    virtual ~NebulaMeltEffect();

    virtual const char *get_effectname() override { return "Nebula Melt"; }
    virtual void init() override;
    virtual void process(float *dataL, float *dataR) override;
    virtual void suspend() override;
    
    virtual void init_ctrltypes() override;
    virtual void init_default_values() override;
    virtual const char *group_label(int id) override;
    virtual int group_label_ypos(int id) override;
    virtual int get_ringout_decay() override { return 1600; }

    enum nebula_melt_params
    {
        melt_model = 0,     // 0: Clean, 1: SP1200, 2: MPC60, 3: SK1, 4: Custom Crush
        melt_bit_depth,     // Bit Depth (1 to 16)
        melt_downsample,    // Sample Rate Reduction (0.0 to 1.0)
        melt_wow_depth,     // Wow Pitch Drift Depth
        melt_wow_rate,      // Wow Speed
        melt_flutter_depth, // Flutter Jitter
        melt_saturation,    // Tape Saturation / Drive
        melt_warmth,        // Lowpass Warmth Cutoff
        melt_noise_level,   // Tape Hiss / Vinyl Crackle
        melt_width,         // Stereo Chorus / Width
        melt_mix,           // Dry / Wet Mix

        melt_num_params
    };

    enum nebula_melt_models
    {
        model_clean = 0,
        model_sp1200,
        model_mpc60,
        model_sk1,
        model_custom_crush,

        num_models
    };

  private:
    void process_sample(float inL, float inR, float &outL, float &outR);

    // Resampling & Bitcrush state
    float holdL = 0.f, holdR = 0.f;
    float step_accum = 0.f;

    // Wow & Flutter delay buffer (stereo ring buffer)
    static constexpr int DELAY_SIZE = 4096;
    float delayBufferL[DELAY_SIZE];
    float delayBufferR[DELAY_SIZE];
    int writePos = 0;

    // LFO phases
    float wowPhase = 0.f;
    float flutterPhase = 0.f;
    uint32_t noiseState = 123456789;

    // Warmth Filter
    BiquadFilter warmthFilterL, warmthFilterR;
    lipol_ps_blocksz mixSmooth alignas(16);
};

#endif // SURGE_SRC_COMMON_DSP_EFFECTS_NEBULAMELTEFFECT_H
