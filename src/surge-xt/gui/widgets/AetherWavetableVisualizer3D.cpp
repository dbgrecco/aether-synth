/*
 * Surge XT Next-Gen - Aether Synthesizer Architecture
 * Implementation of 3D Wavetable & Spectrum Visualizer Component
 */

#include "AetherWavetableVisualizer3D.h"
#include <cmath>

namespace Surge
{
namespace GUI
{
namespace Widgets
{

AetherWavetableVisualizer3D::AetherWavetableVisualizer3D()
{
    setOpaque(false);
    // Initialize default dummy wavetable frames for visualization
    wavetableFrames.resize(16);
    for (size_t f = 0; f < 16; ++f)
    {
        wavetableFrames[f].resize(128);
        float harmonic = 1.0f + static_cast<float>(f) * 0.5f;
        for (size_t i = 0; i < 128; ++i)
        {
            float phase = (static_cast<float>(i) / 128.0f) * 2.0f * juce::MathConstants<float>::pi;
            wavetableFrames[f][i] = std::sin(phase * harmonic) * (1.0f / harmonic);
        }
    }
}

AetherWavetableVisualizer3D::~AetherWavetableVisualizer3D() {}

void AetherWavetableVisualizer3D::setWavetableData(const std::vector<std::vector<float>> &frames)
{
    wavetableFrames = frames;
    repaint();
}

void AetherWavetableVisualizer3D::setMorphPosition(float pos)
{
    morphPosition = juce::jlimit(0.0f, 1.0f, pos);
    repaint();
}

void AetherWavetableVisualizer3D::setRotation(float pitch, float yaw)
{
    cameraPitch = pitch;
    cameraYaw = yaw;
    repaint();
}

void AetherWavetableVisualizer3D::mouseDrag(const juce::MouseEvent &e)
{
    auto delta = e.getOffsetFromDragStart();
    cameraYaw += delta.x * 0.2f;
    cameraPitch = juce::jlimit(5.0f, 85.0f, cameraPitch + delta.y * 0.2f);
    repaint();
}

void AetherWavetableVisualizer3D::resized()
{
    repaint();
}

void AetherWavetableVisualizer3D::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds().toFloat().reduced(4.0f);

    // Draw modern glassmorphic background container
    g.setColour(juce::Colour(0xFF141721).withAlpha(0.85f));
    g.fillRoundedRectangle(bounds, 8.0f);

    g.setColour(juce::Colour(0xFF2E364F));
    g.drawRoundedRectangle(bounds, 8.0f, 1.5f);

    renderWaterfall3D(g, bounds);
}

void AetherWavetableVisualizer3D::renderWaterfall3D(juce::Graphics &g, const juce::Rectangle<float> &bounds)
{
    if (wavetableFrames.empty())
        return;

    size_t numFrames = wavetableFrames.size();
    float cx = bounds.getCentreX();
    float cy = bounds.getCentreY() + bounds.getHeight() * 0.1f;
    float scaleX = bounds.getWidth() * 0.35f;
    float scaleY = bounds.getHeight() * 0.25f;

    float radPitch = juce::DegreesToRadians(cameraPitch);
    float radYaw = juce::DegreesToRadians(cameraYaw);

    float cosYaw = std::cos(radYaw);
    float sinYaw = std::sin(radYaw);
    float sinPitch = std::sin(radPitch);

    int highlightIndex = static_cast<int>(morphPosition * (numFrames - 1));

    for (int f = static_cast<int>(numFrames) - 1; f >= 0; --f)
    {
        const auto &frame = wavetableFrames[f];
        size_t samples = frame.size();
        if (samples == 0)
            continue;

        juce::Path p;
        float zNorm = (static_cast<float>(f) / static_cast<float>(numFrames - 1)) - 0.5f;

        bool isHighlight = (f == highlightIndex);
        juce::Colour frameColour = isHighlight 
            ? juce::Colour(0xFF00E5FF) 
            : juce::Colour::fromHSV(0.55f + f * 0.02f, 0.7f, 0.8f, 0.4f + f * 0.03f);

        for (size_t i = 0; i < samples; ++i)
        {
            float xNorm = (static_cast<float>(i) / static_cast<float>(samples - 1)) - 0.5f;
            float yNorm = frame[i];

            // 3D projection formula
            float x3d = xNorm * cosYaw - zNorm * sinYaw;
            float z3d = xNorm * sinYaw + zNorm * cosYaw;
            float y3d = yNorm * 0.4f - z3d * sinPitch;

            float screenX = cx + x3d * scaleX;
            float screenY = cy - y3d * scaleY;

            if (i == 0)
                p.startNewSubPath(screenX, screenY);
            else
                p.lineTo(screenX, screenY);
        }

        g.setColour(frameColour);
        g.strokePath(p, juce::PathStrokeType(isHighlight ? 2.5f : 1.2f));

        if (isHighlight)
        {
            g.setColour(juce::Colour(0x3000E5FF));
            g.fillPath(p);
        }
    }
}

} // namespace Widgets
} // namespace GUI
} // namespace Surge
