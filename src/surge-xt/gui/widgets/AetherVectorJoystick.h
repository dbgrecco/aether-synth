/*
 * Surge XT Next-Gen - Aether Synthesizer Architecture
 * Korg Vector Joystick 2D Pitch & Modulation Controller Component
 */

#ifndef SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERVECTORJOYSTICK_H
#define SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERVECTORJOYSTICK_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "WidgetBaseMixin.h"

namespace Surge
{
namespace GUI
{
namespace Widgets
{

class AetherVectorJoystick : public juce::Component, public WidgetBaseMixin
{
  public:
    AetherVectorJoystick();
    ~AetherVectorJoystick() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent &e) override;
    void mouseDrag(const juce::MouseEvent &e) override;
    void mouseUp(const juce::MouseEvent &e) override;

    float getPitchBendSemitones() const { return currentPitchX * 12.0f; } // -12.0 to +12.0 semitones
    float getModulationWheelNorm() const { return (currentModY + 1.0f) * 0.5f; } // 0.0 to 1.0

  private:
    float currentPitchX{0.0f}; // -1.0 to +1.0 (0.0 = center)
    float currentModY{0.0f};   // -1.0 to +1.0

    void updateFromPosition(const juce::Point<float> &pos);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AetherVectorJoystick)
};

} // namespace Widgets
} // namespace GUI
} // namespace Surge

#endif // SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERVECTORJOYSTICK_H
