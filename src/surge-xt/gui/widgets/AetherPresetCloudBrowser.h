/*
 * Surge XT Next-Gen - Aether Synthesizer Architecture
 * Preset Vault & Cloud Community Marketplace Browser Component
 */

#ifndef SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERPRESETCLOUDBROWSER_H
#define SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERPRESETCLOUDBROWSER_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "WidgetBaseMixin.h"
#include <vector>
#include <string>

namespace Surge
{
namespace GUI
{
namespace Widgets
{

struct CloudPresetItem
{
    std::string title;
    std::string author;
    std::string category;
    std::string tags;
    int downloadsCount{0};
    int likesCount{0};
    bool isFavorite{false};
};

class AetherPresetCloudBrowser : public juce::Component, public WidgetBaseMixin
{
  public:
    AetherPresetCloudBrowser();
    ~AetherPresetCloudBrowser() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

    void setPresetsList(const std::vector<CloudPresetItem> &items);
    void filterPresets(const std::string &searchQuery, const std::string &categoryFilter);

  private:
    juce::TextEditor searchBox;
    juce::ComboBox categorySelector;
    juce::ListBox presetListBox;

    std::vector<CloudPresetItem> allPresets;
    std::vector<CloudPresetItem> filteredPresets;

    void fetchCloudPresetsMock();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AetherPresetCloudBrowser)
};

} // namespace Widgets
} // namespace GUI
} // namespace Surge

#endif // SURGE_SRC_SURGE_XT_GUI_WIDGETS_AETHERPRESETCLOUDBROWSER_H
