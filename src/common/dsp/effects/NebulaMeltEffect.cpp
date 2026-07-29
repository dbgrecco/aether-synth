/*
 * Aether Synth - Next-Gen Hybrid Synthesizer
 * Nebula Melt Lo-Fi & Vintage Tape Processor Effect
 */

#include "NebulaMeltEffect.h"
#include "SurgeParamConfig.h"
#include "globals.h"
#include <cmath>
#include <algorithm>

namespace
{
// Helper pseudo-random noise generator
inline float next_random(uint32_t &state)
{
    state = state * 1664525u + 1013904223u;
    return (float)state / (float)UINT32_MAX * 2.0f - 1.0f;
}

// Bit quantization helper
inline float quantize_sample(float input, float bits)
{
    if (bits >= 16.0f)
        return input;
    if (bits <= 1.0f)
        bits = 1.0f;
    float levels = std::pow(2.0f, bits) - 1.0f;
    return std::round(input * levels) / levels;
}
} // namespace

NebulaMeltEffect::NebulaMeltEffect(SurgeStorage *storage, FxStorage *fxdata, pdata *pd)
    : Effect(storage, fxdata, pd), warmthFilterL(storage), warmthFilterR(storage)
{
    mixSmooth.set_blocksize(BLOCK_SIZE);
}

NebulaMeltEffect::~NebulaMeltEffect() {}

void NebulaMeltEffect::init()
{
    holdL = 0.f;
    holdR = 0.f;
    step_accum = 0.f;
    writePos = 0;
    std::fill(delayBufferL, delayBufferL + DELAY_SIZE, 0.f);
    std::fill(delayBufferR, delayBufferR + DELAY_SIZE, 0.f);
    wowPhase = 0.f;
    flutterPhase = 0.f;
    noiseState = 123456789;
    warmthFilterL.suspend();
    warmthFilterR.suspend();
}

void NebulaMeltEffect::suspend()
{
    init();
}

const char *NebulaMeltEffect::group_label(int id)
{
    switch (id)
    {
    case 0:
        return "Resampler & DAC";
    case 1:
        return "Tape Drift";
    case 2:
        return "Melt & Noise";
    case 3:
        return "Output";
    }
    return nullptr;
}

int NebulaMeltEffect::group_label_ypos(int id)
{
    switch (id)
    {
    case 0:
        return 1;
    case 1:
        return 7;
    case 2:
        return 15;
    case 3:
        return 23;
    }
    return 0;
}

void NebulaMeltEffect::init_ctrltypes()
{
    Effect::init_ctrltypes();

    fxdata->p[melt_model].set_name("HW Model");
    fxdata->p[melt_model].set_type(ct_enum);
    fxdata->p[melt_model].val.i = model_clean;

    fxdata->p[melt_bit_depth].set_name("Bit Depth");
    fxdata->p[melt_bit_depth].set_type(ct_percent);

    fxdata->p[melt_downsample].set_name("Downsample");
    fxdata->p[melt_downsample].set_type(ct_percent);

    fxdata->p[melt_wow_depth].set_name("Wow Depth");
    fxdata->p[melt_wow_depth].set_type(ct_percent);

    fxdata->p[melt_wow_rate].set_name("Wow Rate");
    fxdata->p[melt_wow_rate].set_type(ct_percent);

    fxdata->p[melt_flutter_depth].set_name("Flutter");
    fxdata->p[melt_flutter_depth].set_type(ct_percent);

    fxdata->p[melt_saturation].set_name("Melt Drive");
    fxdata->p[melt_saturation].set_type(ct_percent);

    fxdata->p[melt_warmth].set_name("Warmth LP");
    fxdata->p[melt_warmth].set_type(ct_freq_audible);

    fxdata->p[melt_noise_level].set_name("Noise Level");
    fxdata->p[melt_noise_level].set_type(ct_percent);

    fxdata->p[melt_width].set_name("Stereo Width");
    fxdata->p[melt_width].set_type(ct_percent);

    fxdata->p[melt_mix].set_name("Dry/Wet Mix");
    fxdata->p[melt_mix].set_type(ct_percent);

    // Grid layout positioning
    fxdata->p[melt_model].posy_offset = 1;
    fxdata->p[melt_bit_depth].posy_offset = 1;
    fxdata->p[melt_downsample].posy_offset = 1;

    fxdata->p[melt_wow_depth].posy_offset = 7;
    fxdata->p[melt_wow_rate].posy_offset = 7;
    fxdata->p[melt_flutter_depth].posy_offset = 7;

    fxdata->p[melt_saturation].posy_offset = 15;
    fxdata->p[melt_warmth].posy_offset = 15;
    fxdata->p[melt_noise_level].posy_offset = 15;

    fxdata->p[melt_width].posy_offset = 23;
    fxdata->p[melt_mix].posy_offset = 23;
}

void NebulaMeltEffect::init_default_values()
{
    fxdata->p[melt_model].val.i = model_clean;
    fxdata->p[melt_bit_depth].val.f = 1.0f; // 100% -> 16 bits
    fxdata->p[melt_downsample].val.f = 0.0f;
    fxdata->p[melt_wow_depth].val.f = 0.25f;
    fxdata->p[melt_wow_rate].val.f = 0.3f;
    fxdata->p[melt_flutter_depth].val.f = 0.15f;
    fxdata->p[melt_saturation].val.f = 0.3f;
    fxdata->p[melt_warmth].val.f = 14000.0f;
    fxdata->p[melt_noise_level].val.f = 0.05f;
    fxdata->p[melt_width].val.f = 0.5f;
    fxdata->p[melt_mix].val.f = 1.0f;
}

void NebulaMeltEffect::process(float *dataL, float *dataR)
{
    const float samplerate = storage->samplerate;
    const float samplerate_inv = storage->samplerate_inv;

    // Resolve parameters
    int model = fxdata->p[melt_model].val.i;
    float bit_depth = 16.0f;
    float ds_factor = 1.0f;

    switch (model)
    {
    case model_sp1200:
        bit_depth = 12.0f;
        ds_factor = std::clamp(26040.0f * samplerate_inv, 0.05f, 1.0f);
        break;
    case model_mpc60:
        bit_depth = 12.0f;
        ds_factor = std::clamp(40000.0f * samplerate_inv, 0.05f, 1.0f);
        break;
    case model_sk1:
        bit_depth = 8.0f;
        ds_factor = std::clamp(9375.0f * samplerate_inv, 0.02f, 1.0f);
        break;
    case model_custom_crush:
        bit_depth = 1.0f + (*pd_float[melt_bit_depth]) * 15.0f;
        ds_factor = std::clamp(1.0f - (*pd_float[melt_downsample]) * 0.9f, 0.05f, 1.0f);
        break;
    case model_clean:
    default:
        bit_depth = 1.0f + (*pd_float[melt_bit_depth]) * 15.0f;
        ds_factor = std::clamp(1.0f - (*pd_float[melt_downsample]) * 0.9f, 0.05f, 1.0f);
        break;
    }

    const float wow_depth = *pd_float[melt_wow_depth];
    const float wow_rate = 0.1f + (*pd_float[melt_wow_rate]) * 2.5f;
    const float flutter_depth = *pd_float[melt_flutter_depth];
    const float saturation = *pd_float[melt_saturation];
    const float warmth_freq = std::clamp(*pd_float[melt_warmth], 500.0f, 20000.0f);
    const float noise_level = *pd_float[melt_noise_level];
    const float width_val = *pd_float[melt_width];
    const float target_mix = *pd_float[melt_mix];

    mixSmooth.set_target(target_mix);

    // Configure Warmth Filter
    warmthFilterL.coeff_LP(warmthFilterL.calc_omega(warmth_freq * samplerate_inv), 0.707f);
    warmthFilterR.coeff_LP(warmthFilterR.calc_omega(warmth_freq * samplerate_inv), 0.707f);

    const float wow_phase_inc = wow_rate * samplerate_inv * (float)M_PI * 2.0f;
    const float step_size = 1.0f / std::max(ds_factor, 0.01f);

    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        float inL = dataL[i];
        float inR = dataR[i];

        // 1. Resampling & Downsample
        step_accum += 1.0f;
        if (step_accum >= step_size)
        {
            step_accum -= step_size;
            holdL = quantize_sample(inL, bit_depth);
            holdR = quantize_sample(inR, bit_depth);
        }

        // 2. Pitch Drift (Wow & Flutter)
        wowPhase += wow_phase_inc;
        if (wowPhase > (float)M_PI * 2.0f)
            wowPhase -= (float)M_PI * 2.0f;

        flutterPhase += 0.07f;
        float flutter_rand = next_random(noiseState);
        float delayMod = std::sin(wowPhase) * wow_depth * 80.0f + flutter_rand * flutter_depth * 15.0f;

        // Write to ring buffer
        delayBufferL[writePos] = holdL;
        delayBufferR[writePos] = holdR;

        // Read from ring buffer with fractional delay
        float baseDelay = 128.0f;
        float readDelay = baseDelay + delayMod;
        readDelay = std::clamp(readDelay, 1.0f, (float)(DELAY_SIZE - 2));

        float readPos = (float)writePos - readDelay;
        if (readPos < 0.0f)
            readPos += DELAY_SIZE;

        int rIndex0 = (int)readPos;
        int rIndex1 = (rIndex0 + 1) % DELAY_SIZE;
        float frac = readPos - (float)rIndex0;

        float wetL = delayBufferL[rIndex0] * (1.0f - frac) + delayBufferL[rIndex1] * frac;
        float wetR = delayBufferR[rIndex0] * (1.0f - frac) + delayBufferR[rIndex1] * frac;

        writePos = (writePos + 1) % DELAY_SIZE;

        // 3. Melt Saturation (Soft Clip)
        float satDrive = 1.0f + saturation * 3.0f;
        wetL = std::tanh(wetL * satDrive);
        wetR = std::tanh(wetR * satDrive);

        // 4. Warmth Filter
        wetL = warmthFilterL.process_single(wetL);
        wetR = warmthFilterR.process_single(wetR);

        // 5. Tape Hiss & Mechanical Noise Injection
        if (noise_level > 0.001f)
        {
            float noiseRaw = next_random(noiseState) * 0.05f * noise_level;
            // Vinyl crackle pops
            if (next_random(noiseState) > 0.997f)
            {
                noiseRaw += next_random(noiseState) * 0.2f * noise_level;
            }
            wetL += noiseRaw;
            wetR += noiseRaw;
        }

        // 6. Stereo Width (Mid-Side Width)
        float mid = (wetL + wetR) * 0.5f;
        float side = (wetL - wetR) * 0.5f * (width_val * 2.0f);
        wetL = mid + side;
        wetR = mid - side;

        // 7. Dry/Wet Crossfade
        float currentMix = mixSmooth.v;
        mixSmooth.step();

        dataL[i] = inL * (1.0f - currentMix) + wetL * currentMix;
        dataR[i] = inR * (1.0f - currentMix) + wetR * currentMix;
    }
}
