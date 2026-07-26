/*
 * Surge XT Next-Gen - Aether Synthesizer Architecture
 * Virtual Mouse XY Pitch Bend & Modulation Controller Pad
 */

#ifndef SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERVIRTUALMOUSEPAD_H
#define SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERVIRTUALMOUSEPAD_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "WidgetBaseMixin.h"

namespace Surge
{
namespace GUI
{
namespace Widgets
{

class AetherVirtualMousePad : public juce::Component, public WidgetBaseMixin
{
  public:
    AetherVirtualMousePad();
    ~AetherVirtualMousePad() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

    void mouseDrag(const juce::MouseEvent &e) override;
    void mouseUp(const juce::MouseEvent &e) override;

    float getPitchBendNorm() const { return currentX; } // 0.0 to 1.0 (0.5 = center)
    float getModWheelNorm() const { return currentY; }  // 0.0 to 1.0

  private:
    float currentX{0.5f}; // Pitch bend centered
    float currentY{0.0f}; // Mod wheel zero

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AetherVirtualMousePad)
};

} // namespace Widgets
} // namespace GUI
} // namespace Surge

#endif // SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERVIRTUALMOUSEPAD_H
