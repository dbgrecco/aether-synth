/*
 * Surge XT Next-Gen - Aether Synthesizer Architecture
 * Implementation of MSEG 2.0 (Multi-Stage Envelope Generator) 2D Interactive Node Editor Component
 */

#include "AetherMSEGEditor2D.h"
#include <algorithm>

namespace Surge
{
namespace GUI
{
namespace Widgets
{

AetherMSEGEditor2D::AetherMSEGEditor2D()
{
    setOpaque(false);
    // Initialize default envelope curve nodes
    msegNodes = {
        {0.0f, 0.0f, 0.0f, false},
        {0.15f, 1.0f, 0.3f, false},
        {0.4f, 0.6f, -0.2f, false},
        {0.7f, 0.8f, 0.0f, false},
        {1.0f, 0.0f, -0.4f, false}
    };
}

AetherMSEGEditor2D::~AetherMSEGEditor2D() {}

juce::Point<float> AetherMSEGEditor2D::nodeToScreen(const MSEGNode &node, const juce::Rectangle<float> &bounds)
{
    float x = bounds.getX() + node.time * bounds.getWidth();
    float y = bounds.getBottom() - node.value * bounds.getHeight();
    return {x, y};
}

MSEGNode AetherMSEGEditor2D::screenToNode(const juce::Point<float> &screenPt, const juce::Rectangle<float> &bounds)
{
    float time = (screenPt.x - bounds.getX()) / bounds.getWidth();
    float value = (bounds.getBottom() - screenPt.y) / bounds.getHeight();
    time = juce::jlimit(0.0f, 1.0f, time);
    value = juce::jlimit(0.0f, 1.0f, value);
    return {time, value, 0.0f, false};
}

void AetherMSEGEditor2D::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds().toFloat().reduced(8.0f);

    // Dark glassmorphic background container
    g.setColour(juce::Colour(0xFF0D101A).withAlpha(0.9f));
    g.fillRoundedRectangle(bounds, 8.0f);

    g.setColour(juce::Colour(0xFF1E2638));
    g.drawRoundedRectangle(bounds, 8.0f, 1.5f);

    // Draw rhythmic musical grid lines (16ths)
    g.setColour(juce::Colour(0xFF161C2C));
    for (int i = 1; i < 16; ++i)
    {
        float gridX = bounds.getX() + (static_cast<float>(i) / 16.0f) * bounds.getWidth();
        g.drawVerticalLine(static_cast<int>(gridX), bounds.getY(), bounds.getBottom());
    }

    if (msegNodes.empty())
        return;

    // Draw curve path connecting nodes
    juce::Path envelopePath;
    auto firstPt = nodeToScreen(msegNodes[0], bounds);
    envelopePath.startNewSubPath(firstPt);

    for (size_t i = 1; i < msegNodes.size(); ++i)
    {
        auto ptPrev = nodeToScreen(msegNodes[i - 1], bounds);
        auto ptCurr = nodeToScreen(msegNodes[i], bounds);

        // Bezier curvature offset
        float ctrlX = (ptPrev.x + ptCurr.x) * 0.5f;
        float ctrlY = (ptPrev.y + ptCurr.y) * 0.5f - msegNodes[i].curve * 30.0f;

        envelopePath.quadraticTo(ctrlX, ctrlY, ptCurr.x, ptCurr.y);
    }

    // Glow effect for envelope path
    g.setColour(juce::Colour(0x3000E5FF));
    g.strokePath(envelopePath, juce::PathStrokeType(6.0f));

    g.setColour(juce::Colour(0xFF00E5FF));
    g.strokePath(envelopePath, juce::PathStrokeType(2.5f));

    // Draw interactive node handles
    for (size_t i = 0; i < msegNodes.size(); ++i)
    {
        auto pt = nodeToScreen(msegNodes[i], bounds);
        bool isSel = (static_cast<int>(i) == selectedNodeIdx);

        float nodeRadius = isSel ? 7.0f : 5.0f;
        juce::Rectangle<float> nodeCircle(pt.x - nodeRadius, pt.y - nodeRadius, nodeRadius * 2.0f, nodeRadius * 2.0f);

        g.setColour(isSel ? juce::Colour(0xFFFFD700) : juce::Colour(0xFF00E5FF));
        g.fillEllipse(nodeCircle);

        g.setColour(juce::Colours::white);
        g.drawEllipse(nodeCircle, 1.5f);
    }
}

void AetherMSEGEditor2D::mouseMouseDown(const juce::MouseEvent &e)
{
    auto bounds = getLocalBounds().toFloat().reduced(8.0f);
    auto clickPt = e.position;

    selectedNodeIdx = -1;

    for (size_t i = 0; i < msegNodes.size(); ++i)
    {
        auto pt = nodeToScreen(msegNodes[i], bounds);
        if (clickPt.getDistanceSquaredFrom(pt) <= 144.0f) // 12px radius threshold
        {
            selectedNodeIdx = static_cast<int>(i);
            break;
        }
    }
    repaint();
}

void AetherMSEGEditor2D::mouseDrag(const juce::MouseEvent &e)
{
    if (selectedNodeIdx < 0 || selectedNodeIdx >= static_cast<int>(msegNodes.size()))
        return;

    auto bounds = getLocalBounds().toFloat().reduced(8.0f);
    auto newNode = screenToNode(e.position, bounds);

    // Keep start node at time 0.0 and end node at time 1.0
    if (selectedNodeIdx == 0)
        newNode.time = 0.0f;
    else if (selectedNodeIdx == static_cast<int>(msegNodes.size()) - 1)
        newNode.time = 1.0f;

    msegNodes[selectedNodeIdx].time = newNode.time;
    msegNodes[selectedNodeIdx].value = newNode.value;

    repaint();
}

void AetherMSEGEditor2D::mouseDoubleClick(const juce::MouseEvent &e)
{
    auto bounds = getLocalBounds().toFloat().reduced(8.0f);
    auto newNode = screenToNode(e.position, bounds);

    // Add new node on double click
    msegNodes.push_back(newNode);
    std::sort(msegNodes.begin(), msegNodes.end(), [](const MSEGNode &a, const MSEGNode &b) {
        return a.time < b.time;
    });

    repaint();
}

void AetherMSEGEditor2D::resized()
{
    repaint();
}

void AetherMSEGEditor2D::setNodes(const std::vector<MSEGNode> &nodes)
{
    msegNodes = nodes;
    repaint();
}

} // namespace Widgets
} // namespace GUI
} // namespace Surge
