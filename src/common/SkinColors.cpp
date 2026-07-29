/*
 * Aether Synth - a next-gen hybrid synthesizer
 * Fork and customization by @dbgrecco
 *
 * Based on Surge XT by Surge Synth Team
 * https://surge-synthesizer.github.io/
 *
 * Original Surge XT Copyright 2018-2024, various authors
 * Released under GNU General Public Licence v3 (GPL-3.0-or-later)
 *
 * === AETHER SKIN COLOR PALETTE ===
 * Primary Cyan:   #00E5FF (0x00E5FF) - Main accent, waveforms, active controls
 * Primary Purple: #7C4DFF (0x7C4DFF) - Selections, mod sources armed, gradients
 * Dark BG:        #090B10           - Background panels
 * Accent Gold:    #FFD700 (0xFFD700) - Macro controls, special highlights
 *
 * This file overrides the default Surge XT color scheme to apply
 * the Aether visual identity. See also: SkinModel.cpp, SkinColors.h
 */
#include "SkinColors.h"

namespace Colors
{
namespace AboutPage
{
const Surge::Skin::Color Text("about.page.text", 0xFFFFFF),
    ColumnText("about.page.column.text", 0xFF9000), Link("about.page.link", 0x2D86FE),
    LinkHover("about.page.hover.link", 0x60C4FF);
}

namespace Dialog
{
const Surge::Skin::Color Background("dialog.background", 0xCDCED4),
    Border("dialog.border", 0x979797);

namespace Titlebar
{
const Surge::Skin::Color Background("dialog.titlebar.background", 0x979797),
    Text("dialog.titlebar.text", 0xFFFFFF);
}
namespace Button
{
const Surge::Skin::Color Background("dialog.button.background", 0xE3E3E3),
    Border("dialog.button.border", 0x979797), Text("dialog.button.text", 0x000000, 0xFF),
    BackgroundHover("dialog.button.hover.background", 0xF1BB72),
    BorderHover("dialog.button.hover.border", 0, 0, 0, 0),
    TextHover("dialog.button.hover.text", 0x000000, 0xFF),
    BackgroundPressed("dialog.button.pressed.background", 0x804900),
    BorderPressed("dialog.button.pressed.border", 0x979797),
    TextPressed("dialog.button.pressed.text", 0xFF9300);
}
namespace Checkbox
{
const Surge::Skin::Color Background("dialog.checkbox.background", 0xFFFFFF),
    Border("dialog.checkbox.border", 0x000000, 0xFF), Tick("dialog.checkbox.tick", 0x000000, 0xFF);
}
namespace Entry
{
const Surge::Skin::Color Background("dialog.textfield.background", 0xFFFFFF),
    Border("dialog.textfield.border", 0xA0A0A0), Caret("dialog.textfield.caret", 0xA0A0A0),
    Focus("dialog.textfield.focus", 0xD6D6FF), Text("dialog.textfield.text", 0x000000, 0xFF);
}
namespace Label
{
const Surge::Skin::Color Error("dialog.label.text.error", 0xFF0000),
    Text("dialog.label.text", 0x000000, 0xFF);
}
} // namespace Dialog

namespace Effect
{
namespace Label
{
const Surge::Skin::Color Text("effect.label.text", 0x4C4C4C),
    Separator("effect.label.separator", 0x6A6A6A);
}
namespace Preset
{
const Surge::Skin::Color Name("effect.label.presetname", 0x000000, 0xFF);
}
namespace Menu
{
const Surge::Skin::Color Text("effect.menu.text", 0x000000, 0xFF),
    TextHover("effect.menu.text.hover", 0x000000, 0xFF);
}

namespace Grid
{
const Surge::Skin::Color Border("effect.grid.border", 0x000000, 0xFF);

namespace Scene
{
const Surge::Skin::Color Background("effect.grid.scene.background", 0xFFFFFF),
    Border("effect.grid.scene.border", 0x000000, 0xFF),
    Text("effect.grid.scene.text", 0x000000, 0xFF),
    BackgroundHover("effect.grid.scene.background.hover", 0xD4D4D4),
    BorderHover("effect.grid.scene.border.hover", 0x000000, 0xFF),
    TextHover("effect.grid.scene.text.hover", 0x000000, 0xFF);
}
namespace Unselected
{
const Surge::Skin::Color Background("effect.grid.unselected.background", 0x979797),
    Border("effect.grid.unselected.border", 0x000000, 0xFF),
    Text("effect.grid.unselected.text", 0xFFFFFF),
    BackgroundHover("effect.grid.unselected.background.hover", 0xD4D4D4),
    BorderHover("effect.grid.unselected.border.hover", 0x000000, 0xFF),
    TextHover("effect.grid.unselected.text.hover", 0xFFFFFF);
}
namespace Selected
{
const Surge::Skin::Color Background("effect.grid.selected.background", 0xFFFFFF),
    Border("effect.grid.selected.border", 0x000000, 0xFF),
    Text("effect.grid.selected.text", 0x202020),
    BackgroundHover("effect.grid.selected.background.hover", 0xD4D4D4),
    BorderHover("effect.grid.selected.border.hover", 0x000000, 0xFF),
    TextHover("effect.grid.selected.text.hover", 0x202020);
}
namespace Bypassed
{
const Surge::Skin::Color Background("effect.grid.bypassed.background", 0x393B45),
    Border("effect.grid.bypassed.border", 0x000000, 0xFF),
    Text("effect.grid.bypassed.text", 0x979797),
    BackgroundHover("effect.grid.bypassed.background.hover", 0x60626C),
    BorderHover("effect.grid.bypassed.border.hover", 0x000000, 0xFF),
    TextHover("effect.grid.bypassed.text.hover", 0x979797);
}
namespace BypassedSelected
{
const Surge::Skin::Color Background("effect.grid.bypassed.selected.background", 0x393B45),
    Border("effect.grid.bypassed.selected.border", 0x000000, 0xFF),
    Text("effect.grid.bypassed.selected.text", 0xFFFFFF),
    BackgroundHover("effect.grid.bypassed.selected.background.hover", 0x60626C),
    BorderHover("effect.grid.bypassed.selected.border.hover", 0x000000, 0xFF),
    TextHover("effect.grid.bypassed.selected.text.hover", 0x979797);
}
} // namespace Grid
} // namespace Effect

namespace FormulaEditor
{
const Surge::Skin::Color Background("formulaeditor.background", 0xF5F6EE),
    Highlight("formulaeditor.highlight", 0xD4D4D4), Text("formulaeditor.text", 0x000000, 0xFF),
    LineNumBackground("formulaeditor.linenumber.background", 0xCDCED4),
    LineNumText("formulaeditor.linenumber.text", 0x000000, 0xFF);

namespace Debugger
{
const Surge::Skin::Color Row("formulaeditor.debugger.row.primary", 0xCDCED4),
    LightRow("formulaeditor.debugger.row.secondary", 0xBABBC0),
    Text("formulaeditor.debugger.text", 0x000000, 0xFF),
    InternalText("formulaeditor.debugger.text.subscriptions", 0x606060);
}
namespace Lua
{
const Surge::Skin::Color Bracket("formulaeditor.lua.bracket", 0x393B45),
    Comment("formulaeditor.lua.comment", 0x979797), Error("formulaeditor.lua.error", 0xFF0000),
    Identifier("formulaeditor.lua.identifier", 0x266DBE),
    Interpunction("formulaeditor.lua.interpunction", 0x111111),
    Keyword("formulaeditor.lua.keyword", 0x537B00), Number("formulaeditor.lua.number", 0xCB4B16),
    String("formulaeditor.lua.string", 0x977500);
} // namespace Lua
} // namespace FormulaEditor

namespace InfoWindow
{
const Surge::Skin::Color Background("infowindow.background", 0xFFFFFF),
    Border("infowindow.border", 0x000000, 0xFF), Text("infowindow.text", 0x000000, 0xFF);

namespace Modulation
{
const Surge::Skin::Color Negative("infowindow.text.modulation.negative", 0x000000, 0xFF),
    Positive("infowindow.text.modulation.positive", 0x000000, 0xFF),
    ValueNegative("infowindow.text.modulation.value.negative", 0x000000, 0xFF),
    ValuePositive("infowindow.text.modulation.value.positive", 0x000000, 0xFF);
}
} // namespace InfoWindow

// === AETHER SKIN: LFO Section ===
// Replaces Surge XT default amber/orange (0xFF9000) with Aether Cyan (0x00E5FF)
namespace LFO
{
namespace Title
{
const Surge::Skin::Color Text("lfo.title.text", 0xC000E5FF); // Aether Cyan faded
}

namespace Type
{
const Surge::Skin::Color Text("lfo.type.unselected.text", 0x000000, 0xFF),
    SelectedText("lfo.type.selected.text", 0x000000, 0xFF),
    SelectedBackground("lfo.type.selected.background", 0x7C4DFF); // Aether Purple
}
namespace StepSeq
{
const Surge::Skin::Color Background("lfo.stepseq.background", 0x0D1020), // Aether Dark
    ColumnShadow("lfo.stepseq.column.shadow", 0x1A2040),
    DragLine("lfo.stepseq.drag.line", 0x00E5FF), // Aether Cyan
    Envelope("lfo.stepseq.envelope", 0x4C3A8A),
    TriggerClick("lfo.stepseq.trigger.click", 0x7C4DFF), // Aether Purple
    Wave("lfo.stepseq.wave", 0x00E5FF); // Aether Cyan

namespace Button
{
const Surge::Skin::Color Background("lfo.stepseq.button.background", 0x141828),
    Border("lfo.stepseq.button.border", 0x2A3560),
    Hover("lfo.stepseq.button.hover", 0x7C4DFF), // Aether Purple
    Arrow("lfo.stepseq.button.arrow", 0xCCCCFF, 0xFF),
    ArrowHover("lfo.stepseq.button.arrow.hover", 0x00E5FF, 0xFF); // Aether Cyan
}
namespace InfoWindow
{
const Surge::Skin::Color Background("lfo.stepseq.popup.background", 0x0D1020),
    Border("lfo.stepseq.popup.border", 0x00E5FF, 0xFF), // Aether Cyan border
    Text("lfo.stepseq.popup.text", 0xF0F4FF, 0xFF);
}
namespace Loop
{
const Surge::Skin::Color Marker("lfo.stepseq.loop.markers", 0x7C4DFF), // Aether Purple
    PrimaryStep("lfo.stepseq.loop.step.primary", 0x002A3A),
    SecondaryStep("lfo.stepseq.loop.step.secondary", 0x001E2A),
    OutsidePrimaryStep("lfo.stepseq.loop.outside.step.primary", 0x1A1A2A),
    OutsideSecondaryStep("lfo.stepseq.loop.outside.step.secondary", 0x141420);
}
namespace Step
{
const Surge::Skin::Color Fill("lfo.stepseq.step.fill", 0x00E5FF), // Aether Cyan
    FillDeactivated("lfo.stepseq.step.fill.deactivated", 0x7C4DFF), // Aether Purple
    FillOutside("lfo.stepseq.step.fill.outside", 0x404060);
}
} // namespace StepSeq
namespace Waveform
{
const Surge::Skin::Color Background("lfo.waveform.background", 0x0A0E1A), // Aether Dark
    Bounds("lfo.waveform.bounds", 0x1A2040),
    Center("lfo.waveform.center", 0x2A3060),
    Dots("lfo.waveform.dots", 0x004060),
    Envelope("lfo.waveform.envelope", 0x7C4DFF), // Aether Purple
    Wave("lfo.waveform.wave", 0x00E5FF, 0xFF),   // Aether Cyan
    DeactivatedWave("lfo.waveform.wave.deactivated", 0x6000E5FF),
    GhostedWave("lfo.waveform.wave.ghosted", 0x407C4DFF); // Purple ghosted

namespace Ruler
{
const Surge::Skin::Color Text("lfo.waveform.ruler.text", 0x94A3B8, 0xFF),
    Ticks("lfo.waveform.ruler.ticks", 0x2A3060, 0xFF),
    ExtendedTicks("lfo.waveform.ruler.ticks.extended", 0x7C4DFF), // Aether Purple
    SmallTicks("lfo.waveform.ruler.ticks.small", 0x3A2870);
}
} // namespace Waveform
} // namespace LFO

namespace Menu
{
const Surge::Skin::Color Name("menu.name", 0x000000, 0xFF), NameHover("menu.name.hover", 0x3C1400),
    NameDeactivated("menu.name.deactivated", 0xB4B4B4), Value("menu.value", 0x000000, 0xFF),
    ValueHover("menu.value.hover", 0x3C1400), ValueDeactivated("menu.value.deactivated", 0xB4B4B4);

const Surge::Skin::Color FilterValue("filtermenu.value", 0xFF9A10),
    FilterValueHover("filtermenu.value.hover", 0xFFFFFF);
} // namespace Menu

namespace PopupMenu
{
const Surge::Skin::Color Background("popupmenu.background", 0x303030),
    HiglightedBackground("popupmenu.background.highlight", 0x606060),
    Text("popupmenu.text", 0xFFFFFF), HeaderText("popupmenu.text.header", 0xFFFFFF),
    HighlightedText("popupmenu.text.highlight", 0xEEEEFF);
}

namespace ModSource
{
namespace Unused
{
const Surge::Skin::Color Background("modbutton.unused.fill", 0x123463),
    Border("modbutton.unused.frame", 0x205DB0),
    BorderHover("modbutton.unused.frame.hover", 0x90D8FF), Text("modbutton.unused.text", 0x2E86FE),
    TextHover("modbutton.unused.text.hover", 0x90D8FF);
}
namespace Used
{
const Surge::Skin::Color Background("modbutton.used.fill", 0x123463),
    Border("modbutton.used.frame", 0x40ADFF), BorderHover("modbutton.used.frame.hover", 0xC0EBFF),
    Text("modbutton.used.text", 0x40ADFF), TextHover("modbutton.used.text.hover", 0xC0EBFF),
    UsedModHover("modbutton.used.text.usedmod.hover", 0x9AE15F);
}
namespace Selected
{
const Surge::Skin::Color Background("modbutton.selected.fill", 0x2364C0),
    Border("modbutton.selected.frame", 0x2E86FE),
    BorderHover("modbutton.selected.frame.hover", 0xC0EBFF),
    Text("modbutton.selected.text", 0x123463), TextHover("modbutton.selected.text.hover", 0xC0EBFF),
    UsedModHover("modbutton.selected.text.usedmod.hover", 0x9AE15F);
namespace Used
{
const Surge::Skin::Color Background("modbutton.selected.used.fill", 0x2364C0),
    Border("modbutton.selected.used.frame", 0x90D8FF),
    BorderHover("modbutton.selected.used.frame.hover", 0xB9FFFF),
    Text("modbutton.selected.used.text", 0x90D8FF),
    TextHover("modbutton.selected.used.text.hover", 0xB9FFFF),
    UsedModHover("modbutton.selected.used.text.usedmod.hover", 0x9AE15F);
}
} // namespace Selected
namespace Armed
{
const Surge::Skin::Color Background("modbutton.armed.fill", 0x74AC48),
    Border("modbutton.armed.frame", 0xADFF6B), BorderHover("modbutton.armed.frame.hover", 0xD0FFFF),
    Text("modbutton.armed.text", 0xADFF6B), TextHover("modbutton.armed.text.hover", 0xD0FFFF),
    UsedModHover("modbutton.armed.text.usedmod.hover", 0x2E86FE);
}
namespace Macro
{
const Surge::Skin::Color Background("modbutton.macro.slider.background", 0x205DB0),
    Border("modbutton.macro.slider.border", 0x123463),
    Fill("modbutton.macro.slider.fill", 0x2E86FE),
    CurrentValue("modbutton.macro.slider.currentvalue", 0xFF9000);
}
} // namespace ModSource

// === AETHER SKIN: MSEG Editor ===
// Curve and highlight colors changed to Aether Cyan/Purple
namespace MSEGEditor
{
const Surge::Skin::Color
    Background("msegeditor.background", 0x080B14),              // Aether deep dark
    Curve("msegeditor.curve", 0x00E5FF),                        // Aether Cyan main curve
    DeformCurve("msegeditor.curve.deformed", 0x7C4DFF),         // Aether Purple deformed
    CurveHighlight("msegeditor.curve.highlight", 0xFFFFFF),     // White on highlight
    Panel("msegeditor.panel", 0x141828),
    Text("msegeditor.panel.text", 0xF0F4FF, 0xFF);

namespace Axis
{
const Surge::Skin::Color Line("msegeditor.axis.line", 0x4000E5FF), // Cyan axis
    Text("msegeditor.axis.text", 0x94A3B8),
    SecondaryText("msegeditor.axis.text.secondary", 0x6094A3B8);
}
namespace GradientFill
{
const Surge::Skin::Color
    StartColor("msegeditor.fill.gradient.start", 0x5000E5FF),   // Cyan fill gradient
    EndColor("msegeditor.fill.gradient.end", 0x087C4DFF);       // Purple fade
}
namespace Grid
{
const Surge::Skin::Color Primary("msegeditor.grid.primary", 0x2A3060),
    SecondaryHorizontal("msegeditor.grid.secondary.horizontal", 0x141A40),
    SecondaryVertical("msegeditor.grid.secondary.vertical", 0x0E1230);
}
namespace Loop
{
const Surge::Skin::Color
    Marker("msegeditor.loop.marker", 0x907C4DFF),               // Aether Purple marker
    RegionAxis("msegeditor.loop.region.axis", 0x2000E5FF),
    RegionFill("msegeditor.loop.region.fill", 0x0A0E20),
    RegionBorder("msegeditor.loop.region.border", 0x707C4DFF);  // Purple region border
}
namespace NumberField
{
const Surge::Skin::Color Text("msegeditor.numberfield.text", 0x00E5FF, 0xFF), // Cyan values
    TextHover("msegeditor.numberfield.text.hover", 0xFFFFFF, 0xFF);
}
} // namespace MSEGEditor

namespace NumberField
{
const Surge::Skin::Color Text("numberfield.text", 0x000000, 0xFF),
    TextHover("numberfield.text.hover", 0x000000, 0xFF);
}

// === AETHER SKIN: Oscillator Display ===
// Wave colors changed from amber (0xFF9000) to Aether Cyan (0x00E5FF)
// 3D gradient uses cyan-to-purple for Aether identity
namespace Osc
{
namespace Display
{
const Surge::Skin::Color Bounds("osc.line.bounds", 0x1A2040), Center("osc.line.center", 0x2A3060),
    AnimatedWave("osc.waveform.animated", 0x00E5FF), // Aether Cyan animated
    Wave("osc.waveform", 0x00E5FF),                 // Aether Cyan primary wave
    WaveCurrent3D("osc.waveform3d.current", 0x00E5FF, 0xFF),  // Cyan top frame
    WaveStart3D("osc.waveform3d.gradient.start", 0x7C4DFF),   // Purple far frames
    WaveEnd3D("osc.waveform3d.gradient.end", 0x3A2070),
    WaveFillStart3D("osc.waveform3d.fill.gradient.start", 0x00E5FF, 0x28), // Cyan fill
    WaveFillEnd3D("osc.waveform3d.fill.gradient.end", 0x7C4DFF, 0x14),    // Purple fill
    Dots("osc.waveform.dots", 0x2A3060);
}
namespace Filename
{
const Surge::Skin::Color Background("osc.wavename.background", 0x7C4DFF), // Aether Purple
    BackgroundHover("osc.wavename.background.hover", 0x00E5FF),           // Cyan on hover
    Frame("osc.wavename.frame", 0x7C4DFF),
    FrameHover("osc.wavename.frame.hover", 0x00E5FF),
    Text("osc.wavename.text", 0xFFFFFF, 0xFF),
    TextHover("osc.wavename.text.hover", 0x090B10); // Dark text on hover
}
namespace Type
{
const Surge::Skin::Color Text("osc.type.text", 0x00E5FF),        // Aether Cyan
    TextHover("osc.type.text.hover", 0xFFFFFF);
} // namespace Type
} // namespace Osc

namespace Waveshaper
{
const Surge::Skin::Color Text("waveshaper.text", 0x000000, 0xFF),
    TextHover("waveshaper.text.hover", 0x000000, 0xFF);
namespace Display
{
const Surge::Skin::Color Dots("waveshaper.waveform.dots", 0x404040),
    Wave("waveshaper.waveform", 0xFF9000), WaveHover("waveshaper.waveform.hover", 0xFFFFFF);
}
namespace Preview
{
const Surge::Skin::Color Border("waveshaper.preview.border", 0xFFFFFF),
    Background("waveshaper.preview.background", 0x0A0A14),
    Text("waveshaper.preview.text", 0xFFFFFF);
}
} // namespace Waveshaper

namespace Overlay
{
const Surge::Skin::Color Background("editor.overlay.background", 0xD9000000);
}

namespace PatchBrowser
{
const Surge::Skin::Color Text("patchbrowser.text", 0x000000, 0xFF),
    TextHover("patchbrowser.text.hover", 0x000000, 0x00);

namespace CommentTooltip
{
const Surge::Skin::Color Border("patchbrowser.tooltip.border", 0xFF979797),
    Background("patchbrowser.tooltip.background", 0xFFE3E3E3),
    Text("patchbrowser.tooltip.text", 0xFF151515);
}

namespace TypeAheadList
{
const Surge::Skin::Color Border("patchbrowser.typeahead.border", 0x646482),
    Background("patchbrowser.typeahead.background", 0xFFFFFF),
    Text("patchbrowser.typeahead.text", 0x000000, 0xFF),
    HighlightBackground("patchbrowser.typeahead.background.highlight", 0xD2D2FF),
    HighlightText("patchbrowser.typeahead.text.highlight", 0x000064),
    SubText("patchbrowser.typeahead.subtext", 0x000000, 0xFF),
    HighlightSubText("patchbrowser.typeahead.subtext.highlight", 0x000064),
    Separator("patchbrowser.typeahead.separator", 0x8C8C8C);
}

} // namespace PatchBrowser

namespace Scene
{
namespace PitchBendRange
{
const Surge::Skin::Color Text("scene.pbrange.text", 0xFF9300),
    TextHover("scene.pbrange.text.hover", 0xFFFFFF);
}
namespace SplitPoint
{
const Surge::Skin::Color Text("scene.split_poly.text", 0x000000, 0xFF),
    TextHover("scene.split_poly.text.hover", 0x000000, 0xFF);
}
namespace KeytrackRoot
{
const Surge::Skin::Color Text("scene.keytrackroot.text", 0x000000, 0xFF),
    TextHover("scene.keytrackroot.text.hover", 0x000000, 0xFF);
}
} // namespace Scene

namespace Slider
{
namespace Label
{
const Surge::Skin::Color Light("slider.light.label", 0xFFFFFF),
    Dark("slider.dark.label", 0x000000, 0xFF);
}
namespace Modulation
{
const Surge::Skin::Color Positive("slider.modulation.positive", 0xADFF6B),
    Negative("slider.modulation.negative", 0xADFF6B);
}
} // namespace Slider

namespace VuMeter
{
const Surge::Skin::Color Background("vumeter.background", 0x151515),
    Border("vumeter.border", 0xCDCED4), UnavailableText("vumeter.unavailabletext", 0xFF9000);

}

// === AETHER SKIN: Virtual Keyboard ===
// Key press color changed from amber to Aether Cyan
namespace VirtualKeyboard
{
const Surge::Skin::Color Text("vkb.text", 0x94A3B8, 0xFF), Shadow("vkb.shadow", 0x60000000);

namespace Wheel
{
const Surge::Skin::Color Background("vkb.wheel.background", 0x141828),
    Border("vkb.wheel.border", 0x2A3560),
    Value("vkb.wheel.value", 0x00E5FF); // Aether Cyan wheel fill
}

namespace Key
{
const Surge::Skin::Color
    Black("vkb.key.black", 0x0A0E1A, 0xFF),         // Aether Dark
    White("vkb.key.white", 0xF0F4FF),
    Separator("vkb.key.separator", 0x401A2040),
    MouseOver("vkb.key.mouse_over", 0x807C4DFF),    // Purple hover
    Pressed("vkb.key.pressed", 0x00E5FF);           // Aether Cyan pressed
}

namespace OctaveJog
{
const Surge::Skin::Color Background("vkb.octave.background", 0x1A2040),
    Arrow("vkb.octave.arrow", 0x00E5FF, 0xFF); // Aether Cyan arrows
}
} // namespace VirtualKeyboard

// === AETHER SKIN: JUCE Widgets ===
// Tab bar and multi-switch use Aether Purple/Cyan instead of Surge orange/amber
namespace JuceWidgets
{
namespace TabbedBar
{
const Surge::Skin::Color
    ActiveTabBackground("tabbar.active.background", 0x1A0A3A),  // Deep purple-dark
    InactiveTabBackground("tabbar.inactive.background", 0x0D1020),
    Border("tabbar.border", 0x7C4DFF),                          // Aether Purple
    Text("tabbar.text", 0xF0F4FF),
    TextHover("tabbar.text.hover", 0x00, 0xE5, 0xFF);           // Aether Cyan
}

namespace TextMultiSwitch
{
const Surge::Skin::Color
    Background("multiswitch.background", 0x141828),
    Border("multiswitch.border", 0x2A3560),
    Separator("multiswitch.separator", 0x2A3560),
    DeactivatedText("multiswitch.text.deactivated", 0x4A5A80),
    Text("multiswitch.text", 0xC0CCEE, 0xFF),
    OnText("multiswitch.on.text", 0x090B10, 0xFF),              // Dark on active
    TextHover("multiswitch.hover.text", 0x00E5FF, 0xFF),        // Aether Cyan hover
    HoverOnText("multiswitch.hoveron.text", 0x090B10),
    HoverFill("multiswitch.hover.fill", 0x1A2A4A),
    HoverOnFill("multiswitch.hoveron.fill", 0x00B8CC),          // Darker cyan on hover+active
    OnFill("multiswitch.on.fill", 0x00E5FF),                    // Aether Cyan active fill
    HoverOnBorder("multiswitch.hoveron.border", 0, 0, 0, 0),
    // special: if below color is not transparent, change drawing style
    UnpressedHighlight("multiswitch.unpressed.highlight", 0, 0, 0, 0);
} // namespace TextMultiSwitch
} // namespace JuceWidgets

namespace TuningOverlay
{
namespace FrequencyKeyboard
{
const Surge::Skin::Color WhiteKey("tuningeditor.keyboard.key.white", 0x979797),
    BlackKey("tuningeditor.keyboard.key.black", 0x242424),
    Separator("tuningeditor.keyboard.separator", 0x000000, 0xFF),
    Text("tuningeditor.keyboard.text", 0xFFFFFF),
    PressedKey("tuningeditor.keyboard.key.pressed", 0xFF9300),
    PressedKeyText("tuningeditor.keyboard.text.pressed", 0x000000, 0xFF);

}
namespace SCLKBM
{
const Surge::Skin::Color Background("tuningeditor.scala.background", 0x151515);
namespace Editor
{
const Surge::Skin::Color Border("tuningeditor.scala.border", 0xE3E3E3),
    Background("tuningeditor.scala.background", 0x151515),
    Text("tuningeditor.scala.text", 0xE3E3E3), Comment("tuningeditor.scala.comment", 0x979797),
    Cents("tuningeditor.scala.cents", 0xA3A3FF), Ratio("tuningeditor.scala.ratio", 0xA3FFA3),
    Played("tuningeditor.scala.played", 0xFF9000);
} // namespace Editor
} // namespace SCLKBM

namespace RadialGraph
{
const Surge::Skin::Color Background("tuningeditor.radial.background", 0x151515);
const Surge::Skin::Color KnobFill("tuningeditor.radial.knob.fill", 0x351500),
    KnobFillHover("tuningeditor.radial.knob.fill.hover", 0x904000),
    KnobBorder("tuningeditor.radial.knob.border", 0xE3E3E3),
    KnobThumb("tuningeditor.radial.knob.thumb", 0xFFFFFF),
    KnobThumbPlayed("tuningeditor.radial.knob.played", 0xFF9000);

const Surge::Skin::Color ToneLabel("tuningeditor.radial.tonelabel", 0x979797),
    ToneLabelPlayed("tuningeditor.radial.tonelabel.played", 0xFF9000),
    ToneLabelBorder("tuningeditor.radial.tonelabel.border", 0x979797),
    ToneLabelBackground("tuningeditor.radial.tonelabel.background", 0x151515),
    ToneLabelText("tuningeditor.radial.tonelabel.text", 0xE3E3E3),
    ToneLabelBackgroundPlayed("tuningeditor.radial.tonelabel.background.played", 0xFF9000),
    ToneLabelBorderPlayed("tuningeditor.radial.tonelabel.border.played", 0xE3E3E3),
    ToneLabelTextPlayed("tuningeditor.radial.tonelabel.text.played", 0x151515);

} // namespace RadialGraph
namespace Interval
{
const Surge::Skin::Color Background("tuningeditor.interval.background", 0x151515);
const Surge::Skin::Color NoteLabelBackground("tuningeditor.interval.notelabel.background",
                                             0x979797),
    NoteLabelBackgroundPlayed("tuningeditor.interval.notelabel.background.played", 0xFF9000),
    NoteLabelForeground("tuningeditor.interval.notelabel.foreground", 0x151515),
    NoteLabelForegroundHover("tuningeditor.interval.notelabel.foreground.hover", 0xE3E3E3),
    NoteLabelForegroundPlayed("tuningeditor.interval.notelabel.foreground.played", 0x151515),
    NoteLabelForegroundHoverPlayed("tuningeditor.interval.notelabel.foreground.hover.played",
                                   0xFFFFFF);
const Surge::Skin::Color IntervalText("tuningeditor.interval.text", 0x151515),
    IntervalTextHover("tuningeditor.interval.text.hover", 0x351500),
    IntervalSkipped("tuningeditor.interval.skipped", 0xAAAAAA);

const Surge::Skin::Color HeatmapZero("tuningeditor.interval.heatmap.zero", 0xFFFFFF),
    HeatmapNegFar("tuningeditor.interval.heatmap.negative.far", 0x8282FF),
    HeatmapNegNear("tuningeditor.interval.heatmap.negative.near", 0xE6E6FF),
    HeatmapPosFar("tuningeditor.interval.heatmap.positive.far", 0xFFFF00),
    HeatmapPosNear("tuningeditor.interval.heatmap.positive.near", 0xFFFFC8);
} // namespace Interval
} // namespace TuningOverlay

namespace ModulationListOverlay
{
const Surge::Skin::Color Border("modlist.border", 0x979797), Text("modlist.text", 0xFFFFFF),
    DimText("modlist.text.dimmed", 0x979797), Arrows("modlist.arrows", 0xE3E3E3);
}
} // namespace Colors
