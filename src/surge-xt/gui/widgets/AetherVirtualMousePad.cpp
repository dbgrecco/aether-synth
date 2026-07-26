/*
 * Surge XT Next-Gen - Aether Synthesizer Architecture
 * Implementation of Virtual Mouse XY Pitch Bend & Modulation Controller Pad
 */

#include "AetherVirtualMousePad.h"

namespace Surge
{
namespace GUI
{
namespace Widgets
{

AetherVirtualMousePad::AetherVirtualMousePad()
{
    setOpaque(false);
}

AetherVirtualMousePad::~AetherVirtualMousePad() {}

void AetherVirtualMousePad::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds().toFloat().reduced(4.0f);

    // Glassmorphic XY Pad Container
    g.setColour(juce::Colour(0xFF0F131D).withAlpha(0.85f));
    g.fillRoundedRectangle(bounds, 8.0f);

    g.setColour(juce::Colour(0xFF1E2638));
    g.drawRoundedRectangle(bounds, 8.0f, 1.5f);

    // Draw Crosshair Guides
    g.setColour(juce::Colour(0x3000E5FF));
    g.drawHorizontalLine(static_cast<int>(bounds.getY() + (1.0f - currentY) * bounds.getHeight()), bounds.getX(), bounds.getRight());
    g.drawVerticalLine(static_cast<int>(bounds.getX() + currentX * bounds.getWidth()), bounds.getY(), bounds.getBottom());

    // Draw Active Puck Handle
    float handleX = bounds.getX() + currentX * bounds.getWidth();
    float handleY = bounds.getBottom() - currentY * bounds.getHeight();

    g.setColour(juce::Colour(0xFF00E5FF));
    g.fillEllipse(handleX - 8.0f, handleY - 8.0f, 16.0f, 16.0f);

    g.setColour(juce::Colours::white);
    g.drawEllipse(handleX - 8.0f, handleY - 8.0f, 16.0f, 16.0f, 2.0f);

    // Labels
    g.setFont(juce::Font(11.0f, juce::Font::bold));
    g.setColour(juce::Colours::lightgrey);
    g.drawText("PITCH BEND (X)", bounds.getX() + 8, bounds.getBottom() - 20, 120, 16, juce::Justification::left);
    g.drawText("MOD WHEEL (Y)", bounds.getRight() - 110, bounds.getY() + 6, 100, 16, juce::Justification::right);
}

void AetherVirtualMousePad::mouseDrag(const juce::MouseEvent &e)
{
    auto bounds = getLocalBounds().toFloat().reduced(4.0f);
    currentX = juce::jlimit(0.0f, 1.0f, (e.position.x - bounds.getX()) / bounds.getWidth());
    currentY = juce::jlimit(0.0f, 1.0f, (bounds.getBottom() - e.position.y) / bounds.getHeight());
    repaint();
}

void AetherVirtualMousePad::mouseUp(const juce::MouseEvent &e)
{
    // Auto-spring Pitch Bend (X) back to center (0.5), leave Mod Wheel (Y) as set
    currentX = 0.5f;
    repaint();
}

void AetherVirtualMousePad::resized()
{
    repaint();
}

} // namespace Widgets
} // namespace GUI
} // namespace Surge
