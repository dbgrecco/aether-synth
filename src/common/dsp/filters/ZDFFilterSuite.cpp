/*
 * Surge XT Next-Gen - Aether Audio Engine
 * Implementation of Zero-Delay Feedback (ZDF) & Formant Vowel Filter Suite
 */

#include "ZDFFilterSuite.h"
#include <cmath>

namespace Surge
{
namespace DSP
{

static const VowelFormant VOWEL_TABLE[5] = {
    {800.0f, 1150.0f, 2900.0f}, // A
    {350.0f, 2000.0f, 2800.0f}, // E
    {270.0f, 2300.0f, 3000.0f}, // I
    {450.0f, 800.0f,  2800.0f}, // O
    {320.0f, 800.0f,  2400.0f}  // U
};

ZDFFilterSuite::ZDFFilterSuite() {}
ZDFFilterSuite::~ZDFFilterSuite() {}

void ZDFFilterSuite::init(double sampleRate)
{
    sr = sampleRate;
    s1 = s2 = s3 = s4 = 0.0f;
    for (int i = 0; i < 3; ++i)
    {
        formS1[i] = formS2[i] = 0.0f;
    }
}

void ZDFFilterSuite::setFilterType(ZDFFilterType type)
{
    currentType = type;
}

void ZDFFilterSuite::setCutoff(float cutoffHz)
{
    cutoff = juce::jlimit(20.0f, 20000.0f, cutoffHz);
}

void ZDFFilterSuite::setResonance(float res)
{
    resonance = juce::jlimit(0.0f, 0.99f, res);
}

void ZDFFilterSuite::setVowelMorph(float morph)
{
    vowelPosition = juce::jlimit(0.0f, 1.0f, morph);
}

float ZDFFilterSuite::processMoogZDF(float input)
{
    float g = std::tan(juce::MathConstants<float>::pi * cutoff / static_cast<float>(sr));
    float k = 4.0f * resonance;

    // Zero-Delay Feedback solver approximation
    float G = g / (1.0f + g);
    float S = (s1 * G * G * G + s2 * G * G + s3 * G + s4) / (1.0f + g);
    float u = (input - k * S) / (1.0f + k * G * G * G * G);

    // 4-stage One-Pole filters
    float v1 = (u - s1) * G;
    float y1 = v1 + s1;
    s1 = y1 + v1;

    float v2 = (y1 - s2) * G;
    float y2 = v2 + s2;
    s2 = y2 + v2;

    float v3 = (y2 - s3) * G;
    float y3 = v3 + s3;
    s3 = y3 + v3;

    float v4 = (y3 - s4) * G;
    float y4 = v4 + s4;
    s4 = y4 + v4;

    return y4;
}

float ZDFFilterSuite::processVowelFormant(float input)
{
    // Interpolate formants across vowels A-E-I-O-U
    float scaled = vowelPosition * 4.0f;
    int idx = static_cast<int>(scaled);
    float frac = scaled - idx;

    int idxNext = std::min(4, idx + 1);

    float f1 = VOWEL_TABLE[idx].f1 * (1.0f - frac) + VOWEL_TABLE[idxNext].f1 * frac;
    float f2 = VOWEL_TABLE[idx].f2 * (1.0f - frac) + VOWEL_TABLE[idxNext].f2 * frac;
    float f3 = VOWEL_TABLE[idx].f3 * (1.0f - frac) + VOWEL_TABLE[idxNext].f3 * frac;

    float freqs[3] = {f1, f2, f3};
    float outSum = 0.0f;

    // Process 3 bandpass formants
    for (int i = 0; i < 3; ++i)
    {
        float w0 = 2.0f * juce::MathConstants<float>::pi * freqs[i] / static_cast<float>(sr);
        float alpha = std::sin(w0) / (2.0f * 5.0f); // Q = 5.0

        float b0 = alpha;
        float a0 = 1.0f + alpha;
        float a1 = -2.0f * std::cos(w0);
        float a2 = 1.0f - alpha;

        float y = (b0 * input - a1 * formS1[i] - a2 * formS2[i]) / a0;
        formS2[i] = formS1[i];
        formS1[i] = y;

        outSum += y * 0.33f;
    }

    return outSum;
}

float ZDFFilterSuite::processSample(float input)
{
    switch (currentType)
    {
        case ZDFFilterType::MoogTransistor4Pole:
        case ZDFFilterType::DiodeLadder:
            return processMoogZDF(input);
        case ZDFFilterType::FormantVowel:
            return processVowelFormant(input);
        default:
            return input;
    }
}

} // namespace DSP
} // namespace Surge
