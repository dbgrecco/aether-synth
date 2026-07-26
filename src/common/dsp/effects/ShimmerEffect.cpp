/*
 * Surge XT Next-Gen - Aether Audio Engine
 * Implementation of Shimmer Reverb Effect Engine
 */

#include "ShimmerEffect.h"
#include "SurgeStorage.h"
#include <cmath>

namespace Surge
{
namespace DSP
{

ShimmerEffect::ShimmerEffect(SurgeStorage *storage, SurgeFxUnit *fxdata, psp_type type)
    : Effect(storage, fxdata, type)
{
    delayLineL.resize(44100, 0.0f);
    delayLineR.resize(44100, 0.0f);
}

ShimmerEffect::~ShimmerEffect() {}

void ShimmerEffect::init()
{
    std::fill(delayLineL.begin(), delayLineL.end(), 0.0f);
    std::fill(delayLineR.begin(), delayLineR.end(), 0.0f);
    writePosL = 0;
    writePosR = 0;
    pitchShiftPhaseL = 0.0f;
    pitchShiftPhaseR = 0.0f;
}

void ShimmerEffect::setDecay(float decaySeconds)
{
    float sr = storage->samplerate;
    decayFactor = std::pow(0.001f, 1.0f / (decaySeconds * sr));
}

void ShimmerEffect::setPitchShiftSemi(float semitones)
{
    pitchShiftFactor = std::pow(2.0f, semitones / 12.0f);
}

void ShimmerEffect::setShimmerMix(float mix)
{
    mixAmount = juce::jlimit(0.0f, 1.0f, mix);
}

void ShimmerEffect::process(float *inoutL, float *inoutR)
{
    size_t delaySize = delayLineL.size();

    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        float dryL = inoutL[i];
        float dryR = inoutR[i];

        // Pitch shifted feedback tap reading
        size_t readPosL = (writePosL + static_cast<size_t>(pitchShiftPhaseL)) % delaySize;
        size_t readPosR = (writePosR + static_cast<size_t>(pitchShiftPhaseR)) % delaySize;

        float wetL = delayLineL[readPosL];
        float wetR = delayLineR[readPosR];

        // Write input + pitch-shifted feedback to delay lines
        delayLineL[writePosL] = dryL + wetL * decayFactor;
        delayLineR[writePosR] = dryR + wetR * decayFactor;

        writePosL = (writePosL + 1) % delaySize;
        writePosR = (writePosR + 1) % delaySize;

        pitchShiftPhaseL += (pitchShiftFactor - 1.0f);
        pitchShiftPhaseR += (pitchShiftFactor - 1.0f);

        if (pitchShiftPhaseL >= delaySize) pitchShiftPhaseL -= delaySize;
        if (pitchShiftPhaseR >= delaySize) pitchShiftPhaseR -= delaySize;

        // Output mix
        inoutL[i] = dryL * (1.0f - mixAmount) + wetL * mixAmount;
        inoutR[i] = dryR * (1.0f - mixAmount) + wetR * mixAmount;
    }
}

} // namespace DSP
} // namespace Surge
