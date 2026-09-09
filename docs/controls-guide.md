# DearModdingUI Controls Guide

This guide covers the UI controls, layout scopes, and presentation helpers available in `<DearModdingUI/Client.h>`, `<DearModdingUI/UI.h>`, and `<DearModdingUI/Presentation.h>`.

## Drawing Facade (`dmui::ui`)

Widgets in `dmui::ui` mirror familiar Dear ImGui functions with type safety and checked ABI boundaries.

### Standard Widgets

```cpp
// Plain unformatted text (literal %, ##, etc. are safe)
dmui::ui::TextUnformatted("Hello, world!");

// Spacing and separators
dmui::ui::Spacing();
dmui::ui::Separator();

// Checkboxes and buttons
static bool enabled = true;
dmui::ui::Checkbox("Enable feature", &enabled);

if (dmui::ui::Button("Run Action")) {
    // Action logic
}

// Sliders with explicit type and bounds
static float intensity = 1.0f;
float minVal = 0.0f;
float maxVal = 2.0f;
dmui::ui::SliderScalar("##intensity", &intensity, &minVal, &maxVal, "%.2fx");
```

## Settings Tables and Rows

To match the host theme, settings pages should format options using the standard two-column layout.

```cpp
dmui::SettingsTableScope table{ client, "settings_table_id" };
if (table.Visible())
{
    dmui::SettingsRowScope row{
        client,
        "setting_id",
        "Feature Name",
        "Detailed explanation of what this option does."
    };

    if (row.Visible())
    {
        dmui::ui::Checkbox("##setting_id", &currentValue);

        // row.End takes (canReset, isResetEnabled) and returns optional bool if clicked
        const auto resetClicked = row.End(true, currentValue != defaultValue);
        if (resetClicked.value_or(false)) {
            currentValue = defaultValue;
        }
    }

    table.End();
}
```

## Choice Dropdowns (`dmui::DrawChoice`)

`dmui::DrawChoice` renders a type-deduced dropdown from an array or span of options:

```cpp
const std::array options{
    dmui::ChoiceOption<int>{ 0, "Low Quality", "low", true },
    dmui::ChoiceOption<int>{ 1, "Medium Quality", "med", true },
    dmui::ChoiceOption<int>{ 2, "High Quality", "high", true }
};

auto choice = dmui::DrawChoice(
    "quality_combo",
    currentQuality,
    options,
    "Custom",          // Fallback text if currentQuality matches no option
    "Graphics Preset"  // Label drawn beside combo
);

if (choice.changed && choice.selected) {
    currentQuality = *choice.selected;
}
```

## Typography and Styled Text

### Formatted Text and Tones

`dmui::DrawStyledText` applies host theme colors and text wrapping:

```cpp
dmui::DrawStyledText(
    client,
    "Configuration loaded successfully.",
    {
        .tone = dmui::TextTone::kSuccess,
        .wrapped = true
    });
```

Available tones in `dmui::TextTone`:
- `kInherit`: Uses active text color.
- `kSuccess`: Green status tone.
- `kWarning`: Yellow warning tone.
- `kError`: Red error tone.
- `kAccent`, `kAccentMuted`: Host accent colors.
- `kMuted`: Subdued body text tone.

### Labeled Values

`dmui::DrawLabeledValue` creates inline key-value pairs:

```cpp
dmui::DrawLabeledValue(
    client,
    "Renderer Backend",
    "Direct3D 11",
    {
        .valueStyle = { .tone = dmui::TextTone::kAccent },
        .spacingScale = 2.0f
    });
```

### Font Roles (`dmui::FontGuard`)

Host fonts are pushed by role rather than raw pointers:

```cpp
{
    dmui::FontGuard font{ client, DMUI_FONT_ROLE_HEADING };
    if (font.Pushed()) {
        dmui::ui::TextUnformatted("Heading Title");
    }
}
```

Available roles:
- `DMUI_FONT_ROLE_BODY`
- `DMUI_FONT_ROLE_TITLE`
- `DMUI_FONT_ROLE_HEADING`
- `DMUI_FONT_ROLE_SUBHEADING`
- `DMUI_FONT_ROLE_SUBTEXT`

## Notifications and Status

- **Toast Notifications**: `client.PostNotification(severity, message, durationMs)` displays a popup notification in the corner of the menu.
- **Health Status**: `client.SetStatus(severity, message)` records an active status condition displayed in the host Health page.
