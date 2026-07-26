/*
 * Surge XT Next-Gen - Aether Audio Engine
 * Implementation of OTT Multiband Compressor & Upward/Downward Dynamics Processor
 */

#include "OTTDynamicsEffect.h"
#include "SurgeStorage.h"
#include <cmath>

namespace Surge
{
namespace DSP
{

OTTDynamicsEffect::OTTDynamicsEffect(SurgeStorage *storage, SurgeFxUnit *fxdata, psp_type type)
    : Effect(storage, fxdata, type)
{
    lowBand.thresholdDb = -20.0f;
    lowBand.ratioUpward = 3.5f;
    lowBand.ratioDownward = 6.0f;
    
    midBand.thresholdDb = -18.0f;
    midBand.ratioUpward = 4.0f;
    midBand.ratioDownward = 8.0f;

    highBand.thresholdDb = -15.0f;
    highBand.ratioUpward = 4.5f;
    highBand.ratioDownward = 10.0f;
}

OTTDynamicsEffect::~OTTDynamicsEffect() {}

void OTTDynamicsEffect::init()
{
    lpLowL = lpLowR = 0.0f;
    hpHighL = hpHighR = 0.0f;
    lowBand.currentEnvelope = 0.0f;
    midBand.currentEnvelope = 0.0f;
    highBand.currentEnvelope = 0.0f;
}

void OTTDynamicsEffect::setLowCrossHz(float hz)
{
    lowCrossoverHz = juce::jlimit(50.0f, 1000.0f, hz);
}

void OTTDynamicsEffect::setHighCrossHz(float hz)
{
    highCrossoverHz = juce::jlimit(1000.0f, 12000.0f, hz);
}

void OTTDynamicsEffect::setDepth(float depth)
{
    depthAmount = juce::jlimit(0.0f, 1.0f, depth);
}

float OTTDynamicsEffect::processBandDynamics(BandDynamics &band, float inputSample, float sr)
{
    float absIn = std::abs(inputSample);

    float alphaAttack = std::exp(-1.0f / ((band.attackMs * 0.001f) * sr));
    float alphaRelease = std::exp(-1.0f / ((band.releaseMs * 0.001f) * sr));

    if (absIn > band.currentEnvelope)
        band.currentEnvelope = alphaAttack * band.currentEnvelope + (1.0f - alphaAttack) * absIn;
    else
        band.currentEnvelope = alphaRelease * band.currentEnvelope + (1.0f - alphaRelease) * absIn;

    float envDb = 20.0f * std::log10(std::max(1e-5f, band.currentEnvelope));
    float gainDb = 0.0f;

    if (envDb > band.thresholdDb)
    {
        // Downward Compression
        float overDb = envDb - band.thresholdDb;
        gainDb = -overDb * (1.0f - 1.0f / band.ratioDownward);
    }
    else
    {
        // Upward Compression
        float underDb = band.thresholdDb - envDb;
        gainDb = underDb * (1.0f - 1.0f / band.ratioUpward);
        gainDb = std::min(18.0f, gainDb); // Limit max upward boost to 18dB
    }

    float totalGain = std::pow(10.0f, (gainDb + band.makeupGainDb) / 20.0f);
    float processed = inputSample * totalGain;

    return inputSample * (1.0f - depthAmount) + processed * depthAmount;
}

void OTTDynamicsEffect::process(float *inoutL, float *inoutR)
{
    float sr = storage->samplerate;
    float coeffLow = std::min(0.99f, 2.0f * juce::MathConstants<float>::pi * lowCrossoverHz / sr);
    float coeffHigh = std::min(0.99f, 2.0f * juce::MathConstants<float>::pi * highCrossoverHz / sr);

    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        float inL = inoutL[i];
        float inR = inoutR[i];

        // Simple 3-band crossover filter
        lpLowL += coeffLow * (inL - lpLowL);
        lpLowR += coeffLow * (inR - lpLowR);

        hpHighL += coeffHigh * (inL - hpHighL);
        hpHighR += coeffHigh * (inR - hpHighR);

        float lowL = lpLowL;
        float lowR = lpLowR;
        float highL = inL - hpHighL;
        float highR = inR - hpHighR;
        float midL = inL - lowL - highL;
        float midR = inR - lowR - highR;

        // Process dynamics per band
        float outLowL = processBandDynamics(lowBand, lowL, sr);
        float outLowR = processBandDynamics(lowBand, lowR, sr);
        float outMidL = processBandDynamics(midBand, midL, sr);
        float outMidR = processBandDynamics(midBand, midR, sr);
        float outHighL = processBandDynamics(highBand, highL, sr);
        float outHighR = processBandDynamics(highBand, highR, sr);

        inoutL[i] = outLowL + outMidL + outHighL;
        inoutR[i] = outLowR + outMidR + outHighR;
    }
}

} // namespace DSP
} // namespace Surge
