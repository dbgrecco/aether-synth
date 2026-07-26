/*
 * Surge XT Next-Gen - Aether Audio Engine
 * Implementation of Granular Synthesis Oscillator Module
 */

#include "GranularOscillator.h"
#include "SurgeStorage.h"
#include <cmath>

namespace Surge
{
namespace DSP
{

GranularOscillator::GranularOscillator(SurgeStorage *storage, SurgeSample *sample, float *pitch, float *mod, bool is_sub)
    : Oscillator(storage, sample, pitch, mod, is_sub)
{
    grainPool.resize(32);
    // Fill sample buffer with synthetic sine/harmonics buffer for testing
    sampleBuffer.resize(44100);
    for (size_t i = 0; i < sampleBuffer.size(); ++i)
    {
        float t = static_cast<float>(i) / 44100.0f;
        sampleBuffer[i] = std::sin(2.0f * juce::MathConstants<float>::pi * 440.0f * t) * 0.5f +
                          std::sin(2.0f * juce::MathConstants<float>::pi * 880.0f * t) * 0.25f;
    }
}

GranularOscillator::~GranularOscillator() {}

void GranularOscillator::init(float pitch, bool is_sub)
{
    for (auto &grain : grainPool)
    {
        grain.active = false;
    }
    timeSinceLastGrain = 0.0f;
}

void GranularOscillator::setGrainDensity(float grainsPerSec)
{
    grainDensityGps = std::max(1.0f, grainsPerSec);
}

void GranularOscillator::setGrainSizeMs(float sizeMs)
{
    grainSizeMs = std::max(5.0f, sizeMs);
}

void GranularOscillator::setPositionSpray(float spray)
{
    positionSprayAmount = std::max(0.0f, spray);
}

void GranularOscillator::setPitchJitter(float jitterCents)
{
    pitchJitterAmountCents = std::max(0.0f, jitterCents);
}

float GranularOscillator::calculateHannWindow(float age, float maxAge)
{
    if (maxAge <= 0.0f) return 0.0f;
    float phase = age / maxAge;
    return 0.5f * (1.0f - std::cos(2.0f * juce::MathConstants<float>::pi * phase));
}

void GranularOscillator::spawnNewGrain()
{
    for (auto &grain : grainPool)
    {
        if (!grain.active)
        {
            grain.active = true;
            grain.currentAgeSamples = 0.0f;
            
            float sr = storage->samplerate;
            grain.grainLengthSamples = (grainSizeMs / 1000.0f) * sr;

            // Random position spray
            std::uniform_real_distribution<float> distPos(0.0f, 1.0f);
            float startNorm = std::min(0.95f, distPos(randomEngine) * positionSprayAmount);
            grain.samplePosition = startNorm * static_cast<float>(sampleBuffer.size());

            // Pitch jitter rate
            std::uniform_real_distribution<float> distJitter(-pitchJitterAmountCents, pitchJitterAmountCents);
            float centsOffset = distJitter(randomEngine);
            grain.playbackRate = std::pow(2.0f, centsOffset / 1200.0f);

            // Random stereo panning
            std::uniform_real_distribution<float> distPan(0.0f, 1.0f);
            float pan = distPan(randomEngine);
            grain.panLeft = std::cos(pan * juce::MathConstants<float>::halfPi);
            grain.panRight = std::sin(pan * juce::MathConstants<float>::halfPi);

            break;
        }
    }
}

void GranularOscillator::process_block(float pitch, float detune, float custom_phase)
{
    float sr = storage->samplerate;
    float samplesPerBlock = BLOCK_SIZE;

    // Check grain spawning trigger
    float grainIntervalSamples = sr / grainDensityGps;
    timeSinceLastGrain += samplesPerBlock;
    if (timeSinceLastGrain >= grainIntervalSamples)
    {
        spawnNewGrain();
        timeSinceLastGrain -= grainIntervalSamples;
    }

    // Clear output buffers
    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        outputL[i] = 0.0f;
        outputR[i] = 0.0f;
    }

    // Process active grains
    for (auto &grain : grainPool)
    {
        if (!grain.active)
            continue;

        for (int i = 0; i < BLOCK_SIZE; ++i)
        {
            if (grain.currentAgeSamples >= grain.grainLengthSamples)
            {
                grain.active = false;
                break;
            }

            int sampleIdx = static_cast<int>(grain.samplePosition) % sampleBuffer.size();
            float sampleVal = sampleBuffer[sampleIdx];
            float win = calculateHannWindow(grain.currentAgeSamples, grain.grainLengthSamples);

            float outVal = sampleVal * win;
            outputL[i] += outVal * grain.panLeft;
            outputR[i] += outVal * grain.panRight;

            grain.samplePosition += grain.playbackRate;
            grain.currentAgeSamples += 1.0f;
        }
    }
}

} // namespace DSP
} // namespace Surge
