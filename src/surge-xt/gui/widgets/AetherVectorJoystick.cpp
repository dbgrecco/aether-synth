/*
 * Surge XT Next-Gen - Aether Synthesizer Architecture
 * Implementation of Korg Vector Joystick 2D Pitch & Modulation Controller
 */

#include "AetherVectorJoystick.h"
#include <cmath>

namespace Surge
{
namespace GUI
{
namespace Widgets
{

AetherVectorJoystick::AetherVectorJoystick()
{
    setOpaque(false);
}

AetherVectorJoystick::~AetherVectorJoystick() {}

void AetherVectorJoystick::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds().toFloat().reduced(4.0f);
    float radius = std::min(bounds.getWidth(), bounds.getHeight()) * 0.5f - 8.0f;
    auto center = bounds.getCentre();

    // Metallic circular joystick housing
    g.setColour(juce::Colour(0xFF0C101A));
    g.fillEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f);

    g.setColour(juce::Colour(0xFF00E5FF).withAlpha(0.6f));
    g.drawEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f, 2.0f);

    // Crosshair axes
    g.setColour(juce::Colour(0x3000E5FF));
    g.drawHorizontalLine(static_cast<int>(center.y), center.x - radius + 4.0f, center.x + radius - 4.0f);
    g.drawVerticalLine(static_cast<int>(center.x), center.y - radius + 4.0f, center.y + radius - 4.0f);

    // Labels
    g.setFont(juce::Font(9.0f, juce::Font::bold));
    g.setColour(juce::Colours::lightgrey);
    g.drawText("+MOD", static_cast<int>(center.x - 20), static_cast<int>(center.y - radius + 2), 40, 12, juce::Justification::centred);
    g.drawText("-PITCH", static_cast<int>(center.x - radius + 2), static_cast<int>(center.y - 6), 40, 12, juce::Justification::left);
    g.drawText("+PITCH", static_cast<int>(center.x + radius - 42), static_cast<int>(center.y - 6), 40, 12, juce::Justification::right);

    // Calculate joystick handle position
    float handleX = center.x + currentPitchX * (radius - 12.0f);
    float handleY = center.y - currentModY * (radius - 12.0f);

    // Vector line connecting center to stick
    g.setColour(juce::Colour(0x6000E5FF));
    g.drawLine(center.x, center.y, handleX, handleY, 2.0f);

    // Glowing Joystick Handle Head
    float handleSize = 24.0f;
    juce::Rectangle<float> handleCircle(handleX - handleSize * 0.5f, handleY - handleSize * 0.5f, handleSize, handleSize);

    g.setColour(juce::Colour(0xFF7C4DFF));
    g.fillEllipse(handleCircle);

    g.setColour(juce::Colour(0xFF00E5FF));
    g.drawEllipse(handleCircle, 2.0f);

    g.setColour(juce::Colours::white);
    g.fillEllipse(handleX - 4.0f, handleY - 4.0f, 8.0f, 8.0f);
}

void AetherVectorJoystick::updateFromPosition(const juce::Point<float> &pos)
{
    auto bounds = getLocalBounds().toFloat().reduced(4.0f);
    float radius = std::min(bounds.getWidth(), bounds.getHeight()) * 0.5f - 8.0f;
    auto center = bounds.getCentre();

    float dx = pos.x - center.x;
    float dy = pos.y - center.y;

    float dist = std::sqrt(dx * dx + dy * dy);
    float maxDist = radius - 12.0f;

    if (dist > maxDist && maxDist > 0.0f)
    {
        dx = (dx / dist) * maxDist;
        dy = (dy / dist) * maxDist;
    }

    currentPitchX = juce::jlimit(-1.0f, 1.0f, dx / maxDist);
    currentModY = juce::jlimit(-1.0f, 1.0f, -dy / maxDist);

    repaint();
}

void AetherVectorJoystick::mouseDown(const juce::MouseEvent &e)
{
    updateFromPosition(e.position);
}

void AetherVectorJoystick::mouseDrag(const juce::MouseEvent &e)
{
    updateFromPosition(e.position);
}

void AetherVectorJoystick::mouseUp(const juce::MouseEvent &e)
{
    // Pitch Bend X springs back to center (0.0), Mod Y stays
    currentPitchX = 0.0f;
    repaint();
}

void AetherVectorJoystick::resized()
{
    repaint();
}

} // namespace Widgets
} // namespace GUI
} // namespace Surge
