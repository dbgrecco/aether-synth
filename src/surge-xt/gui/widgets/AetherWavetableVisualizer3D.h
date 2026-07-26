/*
 * Surge XT Next-Gen - Aether Synthesizer Architecture
 * Modern 3D Wavetable & Real-Time Spectrum Visualizer Component
 */

#ifndef SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERWAVETABLEVISUALIZER3D_H
#define SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERWAVETABLEVISUALIZER3D_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_opengl/juce_opengl.h"
#include "WidgetBaseMixin.h"
#include <vector>

namespace Surge
{
namespace GUI
{
namespace Widgets
{

class AetherWavetableVisualizer3D : public juce::Component, public WidgetBaseMixin
{
  public:
    AetherWavetableVisualizer3D();
    ~AetherWavetableVisualizer3D() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

    void setWavetableData(const std::vector<std::vector<float>> &frames);
    void setMorphPosition(float pos);
    void setRotation(float pitch, float yaw);

    void mouseDrag(const juce::MouseEvent &e) override;

  private:
    std::vector<std::vector<float>> wavetableFrames;
    float morphPosition{0.5f};
    float cameraPitch{30.0f};
    float cameraYaw{45.0f};
    juce::Point<int> lastMousePos;

    void renderWaterfall3D(juce::Graphics &g, const juce::Rectangle<float> &bounds);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AetherWavetableVisualizer3D)
};

} // namespace Widgets
} // namespace GUI
} // namespace Surge

#endif // SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERWAVETABLEVISUALIZER3D_H
