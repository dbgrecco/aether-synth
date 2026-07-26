/*
 * Surge XT Next-Gen - Aether Synthesizer Architecture
 * MSEG 2.0 (Multi-Stage Envelope Generator) 2D Interactive Node Editor Component
 */

#ifndef SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERMSEGEDITOR2D_H
#define SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERMSEGEDITOR2D_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "WidgetBaseMixin.h"
#include <vector>

namespace Surge
{
namespace GUI
{
namespace Widgets
{

struct MSEGNode
{
    float time{0.0f};  // 0.0 to 1.0 (normalized time)
    float value{0.0f}; // 0.0 to 1.0 (normalized value)
    float curve{0.0f}; // -1.0 (log) to +1.0 (exp), 0 = linear
    bool isSelected{false};
};

class AetherMSEGEditor2D : public juce::Component, public WidgetBaseMixin
{
  public:
    AetherMSEGEditor2D();
    ~AetherMSEGEditor2D() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

    void mouseMouseDown(const juce::MouseEvent &e) override;
    void mouseDrag(const juce::MouseEvent &e) override;
    void mouseDoubleClick(const juce::MouseEvent &e) override;

    void setNodes(const std::vector<MSEGNode> &nodes);
    const std::vector<MSEGNode> &getNodes() const { return msegNodes; }

  private:
    std::vector<MSEGNode> msegNodes;
    int selectedNodeIdx{-1};
    bool isGridSnapEnabled{true};

    juce::Point<float> nodeToScreen(const MSEGNode &node, const juce::Rectangle<float> &bounds);
    MSEGNode screenToNode(const juce::Point<float> &screenPt, const juce::Rectangle<float> &bounds);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AetherMSEGEditor2D)
};

} // namespace Widgets
} // namespace GUI
} // namespace Surge

#endif // SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERMSEGEDITOR2D_H
