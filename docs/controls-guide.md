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

### Search and Read-Only Text

`Client::DrawSearchInput` takes an explicit maximum UTF-8 byte count, excluding
the terminating NUL. It rejects an existing string longer than that maximum,
embedded NUL bytes, and a maximum of `INT_MAX` or greater. The raw C capacity,
including the terminator, cannot exceed `INT_MAX`. Editing never truncates the
existing value. New input is limited to the remaining capacity, so a paste may
be inserted partially, clipped at a complete UTF-8 boundary.

For large immutable text, pass existing storage and indexes to the host-owned
viewer:

```cpp
dmui::TextViewRequest request{
    .text = reportText,
    .lineOffsets = lineOffsets,
    .matchByteOffsets = matchOffsets,
    .matchByteLength = query.size(),
    .contentRevision = reportGeneration,
    .matchRevision = searchGeneration,
    .viewport = { 0.0f, 420.0f }
};

static dmui::TextViewState state;
if (nextPressed) {
    dmui::SelectNextTextMatch(request, state);
}
if (sectionPressed) {
    dmui::RevealTextOffset(request, state, sectionByteOffset);
}
client.DrawTextView("report-preview", request, state);
```

The arrays and text are borrowed only for the draw call. Line offsets enumerate
every line start, including the final empty line after a trailing newline.
Match offsets are sorted byte offsets and may overlap. Revisions invalidate
stale active-match and reveal state by resetting it, not by failing the draw.
All offsets must be UTF-8 boundaries, and text cannot contain embedded NUL
bytes. A nonempty query may legitimately produce an empty match-offset span
while retaining its nonzero byte length. Revealing a section or source offset
clears the active search-match selection.

`DrawTextViewNavigation` draws a caller-owned set of section or source jumps as
buttons that wrap according to `DMUI_StyleMetrics`. Button widths are clamped to
the available pane, and clipped labels retain their full text in a hover tooltip.
Labels are rendered literally, including `##`. The projection returns a pair-like
label and byte offset, so callers retain their own navigation model:

```cpp
const auto metrics = client.GetStyleMetrics();
if (metrics) {
    dmui::DrawTextViewNavigation(
        "report-jumps",
        std::span<const Section>{ sections },
        *metrics,
        request,
        state,
        [](const Section& section) {
            return std::pair{ std::string_view{ section.label }, section.offset };
        });
}
```

## Settings Tables and Rows

Use `FieldScope` for labeled controls. An optional `SettingsTableScope` groups fields
into standard two-column settings rows:

```cpp
dmui::SettingsTableScope table{ client, "settings_table_id" };
if (table.Visible())
{
    dmui::FieldScope field{
        client,
        "setting_id",
        "Feature Name",
        "Detailed explanation of what this option does."
    };

    if (field.Visible())
    {
        dmui::ui::Checkbox("##setting_id", &currentValue);
        if (needsRestart) {
            field.SetFeedback(
                dmui::FieldFeedbackSeverity::kInfo,
                "Restart the game for changes to take effect.");
        }

        // End takes (showReset, resetEnabled) and reports whether Reset was clicked.
        const auto resetClicked = field.End(true, currentValue != defaultValue);
        if (resetClicked.value_or(false)) {
            currentValue = defaultValue;
        }
    }

    table.End();
}
```

For standalone controls, use the same field without the table wrapper. Fields
work in settings-page and overlay-page draw callbacks without a persistence binding.
Feedback never changes value behavior; the client decides when to supply or clear
it. See [Field Feedback](specification.md#field-feedback) for the full contract.

`SettingDescriptor::resolveFeedback` returns optional `FieldFeedback` each visible
frame, after control edits and before row end. Keep it fast and side-effect-free;
Reset is reflected next frame.

### Compatibility

Existing `SettingsRowScope`, `BeginSettingsRow`, and `EndSettingsRow` code works
unchanged. `FieldScope` is optional for standalone fields and feedback. Appended
host-table entries, the search-capacity wrapper, and the appended monospace font
role retain host ABI 1. Clients that require the text viewer opt into its table
prefix with `ClientOptions::minimumHostAPISize`.

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
- `DMUI_FONT_ROLE_MONOSPACE`

The monospace role is always the host's built-in default-vector font, provisioned
through `AddFontDefaultVector`. It does not fall back to a proportional family.

## Notifications and Status

- **Toast Notifications**: `client.PostNotification(severity, message, durationMs)` displays a popup notification in the corner of the menu.
- **Health Status**: `client.SetStatus(severity, message)` records an active status condition displayed in the host Health page.
