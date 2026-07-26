/*
 * Surge XT Next-Gen - Aether Synthesizer Architecture
 * Implementation of Preset Vault & Cloud Community Marketplace Browser
 */

#include "AetherPresetCloudBrowser.h"

namespace Surge
{
namespace GUI
{
namespace Widgets
{

AetherPresetCloudBrowser::AetherPresetCloudBrowser()
{
    setOpaque(false);

    searchBox.setTextToShowWhenEmpty("Search presets by name, tag, author...", juce::Colours::grey);
    searchBox.setFont(juce::Font(14.0f));
    addAndMakeVisible(searchBox);

    categorySelector.addItem("All Categories", 1);
    categorySelector.addItem("Bass & Sub", 2);
    categorySelector.addItem("Leads & Solo", 3);
    categorySelector.addItem("Pads & Atmospheres", 4);
    categorySelector.addItem("Plucks & Keys", 5);
    categorySelector.addItem("FX & Textures", 6);
    categorySelector.setSelectedId(1);
    addAndMakeVisible(categorySelector);

    fetchCloudPresetsMock();
}

AetherPresetCloudBrowser::~AetherPresetCloudBrowser() {}

void AetherPresetCloudBrowser::fetchCloudPresetsMock()
{
    allPresets = {
        {"Cyberpunk Bassline 2077", "Aether Audio", "Bass & Sub", "Analog, Drive, Midbass", 1420, 310, true},
        {"Ethereal Nebula Pad", "Starlight Audio", "Pads & Atmospheres", "Ambient, Shimmer, Space", 2890, 540, true},
        {"Hyperpop Pluck Lead", "NeonSound", "Leads & Solo", "Aggressive, Bright, FM", 980, 195, false},
        {"Cinematic Granular Texture", "Aether Audio", "FX & Textures", "Granular, Dark, Film", 3150, 720, true},
        {"Deep House Sub Punch", "GrooveLab", "Bass & Sub", "Sub, Warm, Clean", 1850, 410, false}
    };
    filteredPresets = allPresets;
}

void AetherPresetCloudBrowser::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds().toFloat();

    // Dark glassmorphic background
    g.setColour(juce::Colour(0xFF0F121C).withAlpha(0.92f));
    g.fillRoundedRectangle(bounds, 10.0f);

    g.setColour(juce::Colour(0xFF1E2638));
    g.drawRoundedRectangle(bounds, 10.0f, 1.5f);

    // Title header
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(16.0f, juce::Font::bold));
    g.drawText("AETHER PRESET VAULT & COMMUNITY CLOUD", 16, 12, bounds.getWidth() - 32, 24, juce::Justification::left);

    // Render presets list preview
    int yOffset = 90;
    g.setFont(juce::Font(13.0f));

    for (size_t i = 0; i < filteredPresets.size() && yOffset < bounds.getHeight() - 40; ++i)
    {
        const auto &preset = filteredPresets[i];
        juce::Rectangle<float> itemRect(16.0f, static_cast<float>(yOffset), bounds.getWidth() - 32.0f, 44.0f);

        g.setColour(juce::Colour(0xFF192030));
        g.fillRoundedRectangle(itemRect, 6.0f);

        g.setColour(juce::Colour(0xFF00E5FF));
        g.drawText(preset.title, itemRect.getX() + 12, itemRect.getY() + 4, 250, 20, juce::Justification::left);

        g.setColour(juce::Colours::lightgrey);
        g.drawText("by " + preset.author + "  |  " + preset.category, itemRect.getX() + 12, itemRect.getY() + 22, 300, 18, juce::Justification::left);

        g.setColour(juce::Colour(0xFFFFD700));
        g.drawText("★ " + std::to_string(preset.likesCount) + "  ⬇ " + std::to_string(preset.downloadsCount), 
                   itemRect.getRight() - 140, itemRect.getY() + 12, 130, 20, juce::Justification::right);

        yOffset += 50;
    }
}

void AetherPresetCloudBrowser::resized()
{
    searchBox.setBounds(16, 45, getWidth() - 200, 32);
    categorySelector.setBounds(getWidth() - 176, 45, 160, 32);
}

void AetherPresetCloudBrowser::setPresetsList(const std::vector<CloudPresetItem> &items)
{
    allPresets = items;
    filteredPresets = items;
    repaint();
}

void AetherPresetCloudBrowser::filterPresets(const std::string &searchQuery, const std::string &categoryFilter)
{
    filteredPresets.clear();
    for (const auto &item : allPresets)
    {
        if (!searchQuery.empty() && item.title.find(searchQuery) == std::string::npos)
            continue;
        if (!categoryFilter.empty() && categoryFilter != "All Categories" && item.category != categoryFilter)
            continue;
        filteredPresets.push_back(item);
    }
    repaint();
}

} // namespace Widgets
} // namespace GUI
} // namespace Surge
