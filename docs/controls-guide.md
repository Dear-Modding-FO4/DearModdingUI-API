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

// Checkboxes and buttons; the toggle draws no label, so label it with a field
static bool enabled = true;
const auto toggle = dmui::ui::Toggle({ .id = "enabled", .value = enabled });
if (toggle.changed) {
    enabled = toggle.value;
}

if (dmui::ui::Button({
        .label = "Save Preset",
        .glyph = dmui::PhosphorGlyph::kFloppyDisk,
        .tooltip = "Saves the current preset.",
        .style = dmui::ButtonStyle::kPrimary })) {
    // Action logic
}
```

### Numeric Inputs

`dmui::ui::Numeric<T>` is the one numeric input for every integer width, `float`,
and `double`. The presentation is explicit: `kSlider` needs both bounds, while
`kDrag` and `kInput` take either. No label is drawn; put it in a field bracket.

```cpp
static float intensity = 1.0f;
const auto edited = dmui::ui::Numeric<float>({
    .id = "intensity",
    .presentation = dmui::NumericPresentation::kSlider,
    .value = intensity,
    .minimum = 0.0f,
    .maximum = 2.0f,
    .step = 0.05f,       // results snap to stepOrigin + n * step
    .format = "%.2f",    // one printf conversion; null uses the type default
    .unit = "x",         // appended to the formatted value
    .clamp = true        // typed values stay inside the bounds too
});
if (edited.changed) {
    intensity = edited.value;
}
```

`changed` is set on the frame an edit changes the value and `completed` when the
edit ends. `dragSpeed` applies to `kDrag`. `dmui::NumericOptions<T>` maps onto
`DMUI_NumericDescriptor`, which carries the data type, so the C ABI offers the
same control for any width.

### Color Edit

`dmui::ui::ColorEdit` is the one color edit: the inline RGB(A) fields with their
picker, then an optional row of preset swatches. Colors are packed `0xRRGGBBAA`.
No label is drawn; put it in a field bracket.

```cpp
static uint32_t tint = 0x42FA60FF;
const std::array presets{
    dmui::ColorPreset{ "Green", 0x42FA60FF, "The default accent" },
    dmui::ColorPreset{ "Blue", 0x0072B2FF }
};
const auto edit = dmui::ui::ColorEdit({
    .id = "tint",
    .rgba = tint,
    .alpha = false,                       // true also edits the alpha channel
    .presets = presets,
    .presetsLabel = "Suggested colors"
});
if (edit.changed) {
    tint = edit.rgba;
}
```

Without `alpha` the alpha byte passes through unchanged. A declarative page uses
`dmui::ColorSettingControl`, whose value is a `uint64_t` holding the packed color.

### Key Capture

`dmui::ui::KeyCapture` is the one key binding editor. The field shows the binding;
pressing it listens for the next chord of the slot (keyboard and mouse, or
gamepad) and binds it when released. Escape or the controller B button cancels.
No label is drawn; put it in a field bracket.

```cpp
static std::string binding = "Ctrl+F5";
const auto capture = dmui::ui::KeyCapture({
    .id = "toggle",
    .binding = binding.c_str(),
    .warning = conflict.empty() ? nullptr : conflict.c_str(),  // tints the field
    .slot = dmui::KeyCaptureSlot::kKeyboardMouse,
    .allowClear = true                                          // shows an unbind button
});
if (capture.changed) {
    binding = capture.chord;                                    // "none" when cleared
}
```

The editor stores nothing: assign the new chord to whatever owns it, for example
a hotkey override. `readOnly` shows a binding without editing it, and
`displayText` replaces the formatted binding for a client whose bindings are not
host chords. A declarative page uses `dmui::KeybindSettingControl`, whose value is
the chord string.

### List Clipping

Use `ListClipper` to submit only the requested rows of a large list:

```cpp
dmui::ui::ListClipper clipper;
clipper.Begin(static_cast<int32_t>(items.size()));
while (clipper.Step())
    for (int32_t i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
        dmui::ui::Text("%d: %s", i, items[i].c_str());
```

Rows must have uniform height. Omit `itemsHeight` or pass a value <= 0 to measure
the first row. Keep each clipper inside one drawing callback and the same window
and table; end nested clippers in reverse order. Destruction ends a clipper early.

### Text Fields

`dmui::ui::InputText(id, text, options)` is the one text field. The string grows as
needed, so a large paste completes in the same frame up to the backend limit of
`INT_MAX` bytes including the terminating NUL. The caller's string is updated only
after a successful draw; a resize or draw failure leaves it unchanged. The id scopes
the field and is not drawn; label it with a field bracket.

```cpp
dmui::ui::InputText("name", name);
dmui::ui::InputText("search", query, {
	.hint = "Search...",
	.style = dmui::ui::TextFieldStyle::kSearch // the host search frame
});
dmui::ui::InputText("notes", notes, { .lines = 4 });         // multiline
dmui::ui::InputText("tag", tag, { .maxBytes = 32 });         // at most 32 UTF-8 bytes
```

`maxBytes` clips new input at the limit and never truncates text that is already
longer. Pass a `DMUI_TextBuffer` instead of a string to own the storage; without a
resize callback its capacity is the limit, and a paste may be inserted partially at
a UTF-8 boundary. Both forms reject embedded NUL bytes. A multiline field has no
hint and no history or completion keys.

The bundled backend currently suppresses single-line glyph rendering once the
buffer exceeds 2 MiB. This is a display safeguard in the native input widget,
not a wrapper storage cap.

For a console or any autocompleting field, the same call reports history,
completion, submit, cancel, and edit events, plus the caret position.
Replace the text by assigning the string and passing `kReload` with
the new cursor on the next frame:

```cpp
using Flags = dmui::ui::TextEditFlags;
dmui::ui::TextEditState state{};
(void)dmui::ui::InputText("command", input, {
		.hint = "Command",
		.editFlags = Flags::kHistoryKeys | Flags::kCompletionKey |
			Flags::kKeepFocusOnSubmit | reload,
		.cursor = reloadCursor
	}, state);
if (dmui::ui::HasTextEditEvent(state, dmui::ui::TextEditEvents::kCompletion))
	ShowCompletions();
if (state.active && dmui::ui::BeginTooltipAt({
		.position = { state.caretPosition.x, state.caretPosition.y + state.lineHeight } }))
{
	DrawCompletionList();
	dmui::ui::EndTooltip();
}
```

The example plugin's Console page is a complete version with history and a
selectable suggestion list.

A single-line field can color its text, for example to highlight a command. The
styler runs after the frame's edits with the current text and returns spans over
it; the field then draws in the monospace role:

```cpp
dmui::ui::TextStyler styler = [](std::string_view text) {
    static std::vector<dmui::TextSpan> spans;
    spans.clear();
    if (const auto end = text.find(' '); end != 0) {
        spans.push_back(dmui::TextStyle{}.Tone(dmui::TextTone::kAccent).Bold()
            .Span(0, end == std::string_view::npos ? text.size() : end));
    }
    return std::span<const dmui::TextSpan>{ spans };
};
dmui::ui::InputText("command", input, { .styler = &styler });
```

`dmui::ui::Shortcut("Ctrl+Enter")` reports a keyboard chord for the focused page:

```cpp
if (dmui::ui::Shortcut("Ctrl+Enter")) {
    Submit();
}
```

For large immutable text, pass existing storage and indexes to the host-owned
viewer:

```cpp
dmui::TextViewRequest request{
    .text = reportText,
    .lineOffsets = lineOffsets,
    .matchByteOffsets = matchOffsets,
    .matchByteLength = query.size(),
    .spans = highlightSpans,           // optional styled and interactive ranges
    .contentRevision = reportGeneration,
    .matchRevision = searchGeneration,
    .spanRevision = highlightGeneration,
    .firstLineNumber = linesTrimmedFromFront,
    .followTail = true,                // pinned only while at the end
    .selectable = true,                // drag to select, Ctrl+C to copy
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
if (state.spanClicked) {
    OpenEntry(state.spanUserId);
}
```

Set `state.scroll = dmui::TextViewScroll::kTop` (or `kBottom`, `kPageUp`,
`kPageDown`) to scroll once; a content change otherwise keeps the reader's
place, anchored by `firstLineNumber`. `state.atBottom` tells whether the view
shows its end.

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
        const auto toggle = dmui::ui::Toggle({ .id = "value", .value = currentValue });
        if (toggle.changed) {
            currentValue = toggle.value;
        }
        if (needsRestart) {
            field.SetFeedback(
                dmui::Severity::kInfo,
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

## Sections

`dmui::ui::SectionScope` draws a section header and its content. Pass an `id`, a
`title`, and optionally a `glyph` (zero infers an icon from the title,
`PhosphorGlyph::kNone` draws none), `box` for the themed panel, a `count`, and a
caller-owned `expanded` flag that makes the section collapsible.

```cpp
static bool advancedOpen = true;
dmui::ui::SectionScope advanced{ {
    .id = "advanced",
    .title = "Advanced",
    .box = true,
    .expanded = &advancedOpen
} };
if (advanced) {
    // Draw the content; the host collapsed it when this is false.
}
```

A section without a title and glyph is a plain rule. `FaqEntry::expanded` is the
same caller-owned state for each `DrawFaq` row.

## Choice Dropdowns (`dmui::ui::Choice`)

`dmui::ui::Choice` renders the one dropdown from an array or span of options. No
label is drawn beside it; put it in a `FieldScope` or settings row for the label:

```cpp
const std::array options{
    dmui::ChoiceOption<int>{ 0, "Low Quality", "low", true },
    dmui::ChoiceOption<int>{ 1, "Medium Quality", "med", true, "Balanced" },
    dmui::ChoiceOption<int>{ 2, "High Quality", "high", true }
};

dmui::FieldScope field{ client, "quality", "Graphics Preset" };
auto choice = dmui::ui::Choice(
    "quality_combo",
    currentQuality,
    options,
    "Custom"   // Preview when currentQuality matches no option
);
(void)field.End();

if (choice.changed && choice.selected) {
    currentQuality = *choice.selected;
}
```

An option's description shows as its tooltip. The raw form takes `dmui::ChoiceOptions`
with `dmui::ChoiceItem` entries and an index.

## Typography and Styled Text

### Styled Text Spans

`dmui::ui::Text(text, spans, options)` draws one item of styled text. Spans are
byte ranges of the one text; each can set foreground and background tones or
colors, a font role, bold, italic, underline, and strikethrough, and the host
lays the runs out inline and wraps across them. `dmui::StyledText` composes the
text and records the ranges:

```cpp
dmui::StyledText text;
text.Append("Renderer Backend", dmui::TextStyle{}.Bold())
    .Append("  ")
    .Append("Direct3D 11", dmui::TextStyle{}.Tone(dmui::TextTone::kAccent));
dmui::ui::Text(text, { .wrap = true });

// One style over the whole string.
dmui::ui::Text("Restart required", dmui::TextStyle{}.Tone(dmui::TextTone::kWarning));
```

`TextOptions::wrap` breaks lines at the content region, or at `wrapWidth` when
it is positive. `TextOptions::bullet` draws a bullet and indents the item.
The text and spans are borrowed for the call.

Available tones in `dmui::TextTone`:
- `kInherit`: Uses active text color.
- `kSuccess`, `kWarning`, `kError`, `kInfo`: Status tones.
- `kAccent`, `kAccentMuted`: Host accent colors.
- `kMuted`, `kDisabled`: Subdued body text tones.
- `kRestartNeeded`, `kCurrentHotkey`: Settings-specific tones.

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

The monospace role is the host's monospace family (JetBrains Mono by default, with
bold and italic faces), which the user can change in the settings. The built-in
vector font is the fallback when that family is not installed; the role does not
fall back to a proportional family.

## Notifications and Status

- **Toast Notifications**: `client.PostNotification(severity, message, durationMs = 0, title = nullptr)` posts a passive bottom-right toast, including while the menu is closed. The host supplies the registered mod name; the title is optional additional text. Zero duration selects five seconds, starting on first presentation. Durations clamp to 250-30000 ms; hovering in menu cursor mode pauses expiry. Four toasts can be visible and 32 wait FIFO; overflow drops the oldest waiting toast. Consecutive identical posts from the same client coalesce with a count.
- **Health Status**: `client.SetStatus(severity, message)` records an active status condition displayed in the host Health page.
