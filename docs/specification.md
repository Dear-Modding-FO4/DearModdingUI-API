# DearModdingUI API Specification

This document details the low-level contracts, binary layout requirements, thread affinity rules, and visual helper semantics for the DearModdingUI client API.

## ABI Versioning

The ABI version is `DMUI_ABI_MAJOR.DMUI_ABI_MINOR` and covers every public table and struct. Clients request their header version with `DMUI_ABI_VERSION`, then retry lower minors of the same major until `DMUI_GetAPI` returns a table. It packs the minor into the high 16 bits, so a `.0` version equals its bare major. The host returns its single current table when the major matches and the requested minor is at most its own. Otherwise it logs both versions and returns null. `DMUI_HostAPI::abiMajor` and `DMUI_HostReadyInfo::abiMajor` carry the major only.

A minor version only appends:
- Slots at the end of `DMUI_HostAPI` or `DMUI_UIAPI`.
- New structs, function and callback types, enum families, enum or flag values, constants, and result codes.

Within a major, every existing slot, signature, struct layout, constant, and enum value is frozen. This includes client descriptors, options, out-parameters, and the info structs passed to callbacks. To extend a struct, add a new struct and a new entry point that takes it. Existing entry points keep their behavior for the inputs they already accept, and they never return values that a later minor introduced. The exception is `DMUI_Result`: treat any unknown non-OK code as a failure. A minor may deprecate an entry point. A deprecated entry point keeps working, delegating to the current implementation, until the next major removes it.

A major version is the only point where removals and changes happen. It batches pending deprecations, resets the minor to 0, and requires clients to rebuild. Before DearModdingUI 1.0, a host serves exactly one major.

From DearModdingUI 1.0, a host serves its current major N and the previous major N-1, and no older. The N-1 table is frozen and adapts onto the current implementation; it is removed when major N+1 ships, so at most one compatibility layer exists. Majors should be rare, because minors absorb additions and deprecations.

Clients must only read or call slots introduced at or before the negotiated minor; later slots may not exist in the returned table. The C++ client negotiates automatically and exposes the selected minor through `Client::AbiMinor()`. Operations newer than that minor return `DMUI_RESULT_UNSUPPORTED_ABI` without reading the slot. Host wrappers record it in `Client::LastResult()`, and UI wrappers expose it through their explicit result or `ui::LastResult()`.

`UNSUPPORTED_ABI` is a soft, non-sticky drawing failure so clients can probe a feature and fall back.

The version word is the only negotiation; there are no service bits or minimum-version options. Resource byte counts and buffer capacities remain runtime validation inputs. Runtime unavailability is reported through operation results and lifecycle callbacks.

### Contract Guard

`schema/ui-contract.json` defines the C UI table and enums, and `API.h` defines the host table, structs, function types, and constants. `schema/ui-contract.manifest.json` records both for the last published version. Generation compares them against that baseline:
- At an unchanged version, any difference is rejected.
- At a higher minor, the baseline UI operations and `DMUI_HostAPI` slots must be an unchanged prefix. Other baseline structs, types, constants, and enum values must be unchanged.
- At a higher major, any change is accepted.
- A lower version is rejected.

`--update-baseline` validates any version change before writing. Refreshing an unchanged version skips validation, and is reserved for unpublished development slices. `Tests/CompileHostAPILayout.cpp` additionally pins compiler-computed `DMUI_HostAPI` offsets. The generated files are:
- `CUIAPI.h`: The C function table structure.
- `UIChecked.generated.h`: Explicit result-returning C++ wrappers.
- `UIBindings.generated.h`: Host declarations and symbolic translations.

`UI.h` is the handwritten boolean/void C++ facade (`dmui::ui`). The schema's `operationMinors` maps each nonzero minor to its first UI operation ID; generated member metadata gates table access.

The host translates every stable enum and flag symbolically. Values do not necessarily match native Dear ImGui enum numeric values. Unknown flag bits return `DMUI_RESULT_INVALID_ARGUMENT`.

### Drawing Failures

Drawing wrappers record the first failure in a callback-scoped sticky result. The trampoline passes this result back to the host, which disables the malfunctioning callback. Scope-end operations continue to dispatch so nested stacks unwind cleanly.
Page and action C callbacks return `DMUI_Result`; frame callbacks remain void and non-drawing.

`registerAction` returns a handle. `unregisterAction` follows `unregisterHotkeyAction`: it runs only in the render-execution scope (otherwise `WRONG_THREAD`), no callback starts after it returns, and an action may unregister itself, so keep its `userData` valid until that callback returns. `setActionEnabled` is thread-safe. A disabled action stays on its toolbar but cannot be pressed, and the command palette omits it; an unregistered action disappears from both. A foreign, stale, or removed handle returns `ACTION_NOT_FOUND`. `Client::AddAction` returns the handle; `Client::UnregisterAction` frees the callback, and `Client::SetActionEnabled` toggles it.

`ListClipper` clips uniform-height rows. Counts must be nonnegative and less than
`INT32_MAX`; heights must be finite, with values <= 0 measuring the first row.
`Begin` returns a host-owned token; `Step` returns half-open display ranges and
releases the token when stepping finishes. `End` releases early; zero and already
finished tokens are no-ops. Nested clippers must be stepped and ended in LIFO
order by their owning client in the original window and table. Violations return
`DMUI_RESULT_UNBALANCED_BRACKET`. Tokens last only for the drawing callback;
the host abandons unfinished clippers at callback exit without seeking the cursor.
The non-copyable, non-movable C++ wrapper ends on destruction and before re-Begin.

Draw images with `ui::Image(handle, size)` or `ui::Image(handle, options)`, returning
whether an image was drawn. Device-invalidated images return false without a
sticky error; stale/foreign/malformed handles and invalid options return errors.
Invalidated handles remain owned until released. `DMUI_ImageInfo::failure`
carries the result for `FAILED`. `Client::LoadImageFile` starts in `LOADING`;
both states return false from `ui::Image` without a sticky error. See the
[file image contract](../README.md#file-images) for paths, formats, limits, and reload behavior.
`ui::PlotAnnotated(id, descriptor)` draws annotated plots. Image lifetime operations
remain on `Client` and the host table.

`GetCursorPos` / `SetCursorPos` use window-local coordinates. X/Y variants are
wrapper conveniences. `TextAligned(alignX, width, text, length)` renders unformatted
UTF-8 with alignment clamped to [0,1]. Nonpositive width uses available content
width; overflow is ellipsized and exposes full text on hover/nav focus.

### Buttons and tooltips

`ui::Button(options)` is the one host button. `ButtonOptions` carries an optional
`id` (the label stands in when it is empty), a `label` drawn verbatim (`##` is
literal; put identity in `id`), a Phosphor `glyph` resolved
through the theme icon set, `tooltip`, `enabled`, a `ButtonStyle` (`kDefault`,
`kPrimary`, or `kSubtle` for chrome-like icon buttons), and a `size` whose zero axis
fits its content and whose negative axis fills the remaining region. It returns
true on the frame the button is pressed. A glyph the theme cannot draw is omitted.
`ui::MeasureButton(options)` returns the size `Button` would take, so layouts need no
parallel arithmetic. The host draws buttons through its theme-selected control
renderer; clients choose only the content and style.

`BeginItemTooltip` is the one way to attach a tooltip to the last item. The host
shows it after the theme's normal hover delay or on navigation focus, including
while the item is disabled, and it pairs with `EndTooltip`. Put `ui::Text(spans)`
inside for styled content. `BeginTooltipAt(placement)` and
`BeginPopupAt(id, placement)` are the explicit placement forms for floating
windows such as completion lists and context menus. A `DMUI_Placement` is a
screen-space `position` and a `pivot` (0..1 of the window size) that sits at it;
a pivot of `{0, 0}` anchors the top-left. Both reject non-finite values. The
tooltip follows its placement every frame. A popup uses it on the frame it
appears, in place of the default spot under the item that opened it, so a
context menu is placed from `getPointerState().position`.

## Drawing Operations and Type Mapping

| Dear ImGui Type | DearModdingUI Equivalent | Notes |
|---|---|---|
| `ImGui::*` | `dmui::ui::*` | Checked C++ facade. |
| `ImVec2`, `ImVec4` | `dmui::ui::Vec2`, `dmui::ui::Vec4` | Equivalent standard value types. |
| `ImGuiCol_*` | `dmui::ui::Color` | Scoped enum. |
| `ImGuiDataType_*` | `dmui::ui::DataType` | Scoped enum. Scalar functions carry explicit types and sizes. |
| Native flags | `dmui::ui::*Flags` | Scoped bitfield enums. |
| `PushFont` | `dmui::FontGuard` | Takes a `DMUI_FontRole`, never raw font pointers. |

### Text Field

`ui::InputText` is the one text input; there are no separate plain, hint, multiline, or search operations. It edits a `DMUI_TextBuffer`, so the client owns the text. The ID is required, scopes the field, and is never drawn; the label belongs to the field bracket.

`DMUI_TextFieldOptions` carries the `hint`, a `style` (`PLAIN`, or `SEARCH` for the host search frame with a magnifier), `DMUI_UIInputTextFlags`, the edit flags below, `maxBytes`, `lines` and `size`, and `cursor`. ImGui callbacks never cross the ABI. `lines` of zero with a zero `size.y` is a single line; otherwise the field is multiline, `lines` tall unless `size.y` is positive. `size.x` is the width: zero uses the item width and a negative value fills the remaining region. A multiline field has no hint, is never `SEARCH`, and rejects `HistoryKeys` and `CompletionKey`.

A non-null `styleSpans` (with `styleUserData`) colors and decorates the text: the host calls it on the render thread after this frame's edits and before drawing, with the current text, and it returns `DMUI_TextSpan` ranges over that text that stay valid until `inputText` returns. It is nonreentrant, cannot issue drawing calls, and is rejected with a multiline field, the `Password` flag, or `ElideLeft`; spans with `INTERACTIVE` or `HAS_ROLE` are rejected. The host draws the field's own glyphs transparent, keeps ImGui's caret, selection, and clipboard behavior, and overlays the runs at the field's horizontal scroll; the text uses the monospace role so advances match the caret. A failing callback or invalid spans make the call return that failure after drawing the text unstyled. `ui::TextFieldOptions::styler` wraps a `std::function` and turns exceptions into `CALLBACK_FAILED`. Selection highlights draw under the runs' backgrounds.

A null resize callback makes the buffer fixed-capacity: capacity includes the NUL and must be no greater than `INT_MAX`, editing never truncates the existing value, and new input is limited to the remaining capacity, inserted partially at a UTF-8 boundary when needed. A nonnull callback lets the host request same-frame growth. It receives a minimum capacity including the NUL and returns storage of at least that size while preserving prior bytes. It is nonreentrant, cannot issue drawing calls, and is valid only for the active draw call. The host retains no buffer, callback, or userdata pointer, and a failing callback must leave the prior allocation and input values intact.

`maxBytes` limits the text length in UTF-8 bytes; zero is unlimited. A limited field is sized once through the resize callback and then clips input at the limit like a fixed buffer, so a limit never asks the client to grow past it. Text already longer than the limit is kept, and the field accepts no more input.

`TextInputBuffer` is the reusable C++ owner for this contract. It copies the
input into a small padded candidate, grows geometrically through the callback,
and limits all capacities to `INT_MAX`. Construction and callback failures are
reported as `DMUI_Result`. After a successful draw it shrinks the candidate to
the terminating NUL without allocation and swaps it into the caller string, so
resize and draw failures preserve the original value.

The C++ `std::string` overloads stage the text through a growable `TextInputBuffer`, reject embedded NUL bytes, and update the string only after a successful draw, so a resize or draw failure preserves the original value.

The bundled native input widget intentionally suppresses single-line glyph
rendering above 2 MiB. This limits display work only; it is not a wrapper cap
and does not change the growable storage contract or the `INT_MAX` capacity
representation bound.

### Text Field Events

`inputText` reports per-frame events in `DMUI_TextEditState::events`:
- `Edited`: the text changed this frame.
- `Submitted`: Enter was pressed.
- `HistoryPrevious` / `HistoryNext`: Up / Down, with `HistoryKeys`.
- `Completion`: Tab, with `CompletionKey`.
- `Canceled`: Escape, with `CaptureCancel`.

Each opt-in flag consumes its key for the active field, so arrows do not navigate, Tab does not move focus, and Escape neither reverts nor deactivates. `CompletionKey` rejects `AllowTabInput`. `KeepFocusOnSubmit` reactivates the field on the next frame. `RequestFocus` focuses an inactive field.

The client owns the text. To replace it, the client writes the buffer and passes `Reload` with a byte `cursor` on a UTF-8 boundary. An active field reloads before applying that frame's keyboard input, so typing lands after the placed cursor. An inactive field always displays the buffer and ignores the cursor.

While active, the state reports the cursor, the selection bounds (equal to the cursor when nothing is selected), the caret line's screen-space top-left in `caretPosition`, and its `lineHeight`. These fields are zero while inactive, including the frame of a kept-focus submission. `ui::BeginTooltipAt(placement)` opens a non-focusable tooltip at a screen position, for example a completion list below the caret. A multiline field still reports `Edited`; Enter inserts a newline and Ctrl+Enter reports `Submitted`.

### Read-Only Text View

`drawTextView` owns the child region, vertical and horizontal scrolling,
visible-line clipping, monospace font selection, match highlighting, styled
spans, and pointer selection.
`DMUI_TextViewDescriptor` borrows immutable UTF-8 text, complete line-start
offsets, sorted overlapping match offsets, a shared match byte length, styled
spans, stable content/search/span revisions, and the viewport. It performs no
I/O or parsing and retains none of those pointers. Text cannot contain embedded
NUL bytes. A nonzero match byte length is valid with zero match offsets,
representing a nonempty query with no matches.

Spans are `DMUI_TextSpan` ranges of the shared text (see Text Styling and Tones).
A view accepts `INTERACTIVE` spans and rejects `HAS_ROLE`; `BOLD` and `ITALIC`
pick the monospace role's faces and fall back to the regular face where a face
would change advances. Search matches stay their own concept and draw through
the same run and background pass, under the glyphs, without becoming spans.

`firstLineNumber` is the absolute index of line 0. A content change keeps the
scroll position, anchored by absolute line: a caller that trims lines from the
front raises `firstLineNumber` by the number removed and the reader keeps their
place, and a caller that wants the top sends the `TOP` scroll command.
`FOLLOW_TAIL` keeps the view pinned to the end only while the reader is at the
end; scrolling up releases it and reaching the end again resumes it.
`SELECTABLE` lets a mouse drag select text across lines, shown as a highlight,
and Ctrl+C or Ctrl+Insert copy the selection to the clipboard while the view
has focus; the selection is host-internal.

`DMUI_TextViewState` contains caller presentation state: revision identity,
the active match index, a one-shot byte offset to reveal, and a one-shot
`scrollCommand` (`PAGE_UP`, `PAGE_DOWN`, `TOP`, `BOTTOM`; an explicit command or a
pending reveal outranks following). Every successful draw rewrites the outputs: `atBottom`
(nonzero while the view shows its end), `events` (`SPAN_HOVERED` while the
pointer is over an `INTERACTIVE` span, `SPAN_CLICKED` on the frame a press and
release without dragging both land on it), and the `spanIndex` and `userId` of
that span. The C++ helpers
reset stale state, wrap previous/next match selection, and route section or
source navigation through the same reveal field. Stale state is reset rather
than rejected. A direct section or source reveal clears the active search match.
Invalid line, match, active, span, scroll command, or reveal offsets return
`DMUI_RESULT_INVALID_ARGUMENT`.

`DrawTextViewNavigation` lays out a caller-owned span of section or source
buttons with wrapping derived from `DMUI_StyleMetrics`. Each button is clamped
to the available pane width. Clipped labels expose their full literal text,
including `##`, in a hover tooltip. A projection supplies each label and byte
offset; the helper introduces no separate anchor model.

`DMUI_FONT_ROLE_MONOSPACE` uses the host's monospace family, JetBrains Mono by
default, with its regular, bold, italic, and bold italic faces. The family is a
theme typography value, and `sMenuMonospaceFontFamily` overrides it. The built-in
vector font (the family name `ImGui`, from `AddFontDefaultVector`) is the
explicit alternative and the fallback when the configured family is not
installed; the role never falls back to a proportional family.

## Presentation Helpers

### Text Styling and Tones

`text(text, length, spans, count, options)` is the one text operation; `ui::Text(text, spans, options)` wraps it, with overloads for one style over the whole text and for a `StyledText`. Spans are `DMUI_TextSpan` byte ranges of one shared UTF-8 text, and the host draws the runs inline as a single item, so `SameLine`, hover, and tooltips apply to the whole block. A call without spans or options draws plain text with ImGui's unbounded path; styled or wrapped text is limited to 1 MiB and 4096 spans.
- `offset` and `length` are bytes. Spans are sorted, never overlap, and begin and end on UTF-8 boundaries inside the text; text outside every span draws with the defaults. `%` and `##` are literal.
- `foreground` and `background` are theme tones. `DMUI_TEXT_TONE_INHERIT` keeps the active text color and draws no background; any other background tone fills translucently. `CUSTOM_FOREGROUND` and `CUSTOM_BACKGROUND` select `foregroundColor` and `backgroundColor` instead, with their alpha.
- `BOLD` and `ITALIC` select the face, `UNDERLINE` and `STRIKETHROUGH` decorate the range, `HAS_ROLE` selects `role` (a `DMUI_FontRole`; without it the run inherits the active font), and `INTERACTIVE` plus `userId` makes a text view range hoverable and clickable.
- Each consumer takes a subset of the flags: the text operation rejects `INTERACTIVE`; a text view rejects `HAS_ROLE`; a text field rejects both. Unknown flag bits, tones, and roles return `DMUI_RESULT_INVALID_ARGUMENT`. In a text view or field, `BOLD` and `ITALIC` use the monospace role's faces and fall back to the regular face where a face would change advances.
- One host pass draws the run backgrounds, then any highlights, then the glyphs, then the decorations, for the text operation, the text view, and the text field alike.
- `DMUI_TEXT_OPTION_WRAP` breaks lines at spaces across runs, per codepoint inside words longer than the line and at CJK characters, at the content region or at `wrapWidth` when positive. `BULLET` draws a bullet and indents the item.
- Null `spans` with a zero count is plain text. Null pointers, invalid UTF-8, unknown values, non-finite colors, and oversized input return `DMUI_RESULT_INVALID_ARGUMENT`.

`StyledText` appends runs and records their offsets: `Append(text)` adds unstyled text and `Append(text, TextStyle{}.Bold().Tone(...))` adds a styled run. `TextStyle` is a span without its range, with `Tone`, `Background`, `Color`, `BackgroundColor`, `Bold`, `Italic`, `Underline`, `Strikethrough`, `Interactive`, and `Role`.

### Style Metrics Layout

`DMUI_StyleMetrics` layout consists of:
- `itemSpacing`
- `framePadding`
- `itemInnerSpacing`
- `cellPadding`
- `windowPadding`
- `indentSpacing`
- `scrollbarSize`
- `fontSizeBase`
- `alpha`
- `frameRounding`
- `frameBorderSize`
- `sectionGap`
- `panelPadding`

`GetStyleMetrics` populates the complete structure.

### Scopes and Choice Controls

- `SettingsTableScope`: Optional RAII grouping for the standard two-column settings layout.
- `FieldScope`: The single RAII field bracket. Inside a settings table it creates the next row; outside one it owns equivalent standalone geometry. It ends only a bracket it successfully opened, and idempotent `End(showReset, resetEnabled)` calls return the cached Reset result.
- `DisabledScope`: Manages `BeginDisabled` / `EndDisabled` pairs cleanly.
- `TooltipScope`: Pairs `BeginItemTooltip` with `EndTooltip` under the host tooltip policy.
- `ChoiceOption<Value>`: An option for the typed `ui::Choice(id, current, options, unmatchedLabel)` with fields `value`, `label`, `key`, `enabled`, and `description`. Selection is type-deduced. If the active value does not match any entry, the preview shows the unmatched label without altering the underlying data.

### Sections

`beginSection` / `endSection` (`ui::SectionScope`) are the one section primitive: a header followed by content. `DMUI_SectionDescriptor` carries a required `id` that scopes the section, a `title` drawn verbatim, a `glyph`, and flags. The glyph is a Phosphor codepoint, zero to let the host infer an icon from a non-empty title with Question as the fallback, or `DMUI_GLYPH_NONE` (`PhosphorGlyph::kNone`) for no icon. A section without title and icon is a single rule across the content region. `BOX` holds the content in the themed panel; `HAS_COUNT` appends "(count)" to the title. A non-null `expanded` makes the section collapsible: the caller owns that state, the host toggles it when the header is clicked, and the content is visible while it is nonzero. A collapsed section draws only its header.

A successful begin with `visible != 0` must be matched by `endSection`; a begin with `visible == 0` opens no bracket. Brackets are render-thread-only, nest, and end in reverse order.

### Choice

`choice` (`ui::Choice`) is the one dropdown. `DMUI_ChoiceDescriptor` carries a required `id`, `options` (a label, an optional `description` shown as the option's item tooltip, and `enabled`), the `selected` index, an `unmatchedLabel`, and an optional `width`. No label is drawn beside it; put the label in a field bracket. Any `selected` outside the options is unmatched: the preview shows `unmatchedLabel` and the client's value is untouched. `DMUI_ChoiceResult` returns the chosen index with `changed` and `completed`, both set on the frame an enabled option other than the selected one is picked. A control without options is disabled.

### Toggle

`toggle` (`ui::Toggle`) is the one checkbox. `DMUI_ToggleDescriptor` carries a required `id`, an optional `label` drawn verbatim beside the box that toggles it too, and the `value`; inside a field bracket leave `label` null and let the bracket own it. `DMUI_ToggleResult` returns the value, which differs from the input only on the frame `changed` is set. Disable it with `beginDisabled`.

### Spinner

`spinner` (`ui::Spinner`) is the one busy indicator. `DMUI_SpinnerDescriptor` carries a required `id`, a `size` (the diameter, with zero using the frame height), and a zero `reserved`. It takes one square item and animates from the frame clock below, so redraw it each frame while work is pending. The default look is a rotating arc in the accent color.

### Frame time

`getTime` (`ui::GetTime`) returns seconds as a double and `getDeltaTime` (`ui::GetDeltaTime`) the last frame's seconds as a float. They are the host frame clock, the one its own animations use: real time that advances only on DMUI frames, so a game pause does not move it. Use them for animation; call them from a draw callback.

### Numeric

`numeric` (`ui::Numeric<T>`) is the one numeric input for signed and unsigned 8 to 64 bit integers, `float`, and `double`. `DMUI_NumericDescriptor` carries a required `id`, the `type`, an explicit `presentation` (`SLIDER`, `DRAG`, or `INPUT`; nothing is inferred from the bounds), `value`, optional `minimum`, `maximum`, and `step` (with a `stepOrigin`) named by `HAS_` flags, a `format`, a `unit` suffix, a `width`, and a `dragSpeed`. Values travel in a `DMUI_NumericValue` union whose member follows the type (`i`, `u`, or `f`) and must be representable in it. `SLIDER` needs both bounds; a reversed pair is swapped. `CLAMP` keeps every result inside the present bounds, and a step snaps every edit to `stepOrigin + n * step`, reporting no change when that restores the input. `format` holds exactly one printf conversion valid for the type (`f F e E g G`, or `d i u x X`) with optional text around it; the host rebuilds it for the argument width, so a client does not pick `lld`. `unit` is appended literally. The host rejects a malformed descriptor. `DMUI_NumericResult` returns the value with `changed` and `completed`. No label is drawn; put it in a field bracket.

### Color Edit

`colorEdit` (`ui::ColorEdit`) is the one color edit. `DMUI_ColorEditDescriptor` carries a required `id`, the packed `0xRRGGBBAA` color, an optional `ALPHA` flag, a `width`, and up to 64 `DMUI_ColorPreset` swatches (a `name`, an optional tooltip `description`, and an `rgba`) under an optional `presetsLabel`. Without `ALPHA` only RGB is edited and the alpha byte passes through unchanged, including when a swatch is chosen. No label is drawn beside it; put the label in a field bracket. `DMUI_ColorEditResult` returns the color with `changed` and `completed`, the latter on a swatch choice or the end of a drag or typed entry.

### Key Capture

`keyCapture` (`ui::KeyCapture`) is the one key binding editor. `DMUI_KeyCaptureDescriptor` carries a required `id`, the `binding` chord (the grammar of a hotkey action default, such as `Ctrl+F5`; empty or `none` is unbound), the `slot` (`KEYBOARD_MOUSE` or `GAMEPAD`), and flags: `ALLOW_CLEAR` adds an unbind button while bound, and `READ_ONLY` shows the binding disabled and captures nothing. `details` leads the tooltip and `warning` explains a conflict or invalid binding; it tints the field, and an unbound field reads "Check binding". A non-empty `displayText` replaces the formatted binding for display, for a client whose bindings are not host chords. Pressing the field captures the next chord of the slot, bound on release, and Escape or the controller B button cancels. `DMUI_KeyCaptureResult` sets `changed` on the frame a chord was captured or cleared and returns it in `chord`, `none` when cleared. The editor stores nothing and does not register or move a hotkey action; the client applies the result. No label is drawn; put it in a field bracket.

### Field Feedback

`beginField` / `endField` work in settings-page and overlay-page drawing callbacks,
inside a settings table or standalone. Dialog services expose no custom drawing
callback. Only a successful begin with `visible != 0` requires an end.
`DMUI_FieldBeginOptions` selects label/value or full-span geometry. Label and
description are optional; an empty label keeps a blank label column. Without a label column, under-label feedback uses the
control region. `DMUI_FieldEndOptions` configures Reset; the client changes the value.

`setFieldFeedback` accepts `DMUI_FieldFeedback`: a `DMUI_Severity` plus a
NUL-terminated UTF-8 message of at most 16 KiB. Text is copied until field end;
the last call wins, and null/empty text or `ClearFeedback()` clears it. Supply it
after edits for same-frame feedback; absence reserves no space. Invalid severity,
size, ownership, or bracket state returns an error.

The host only presents feedback, never validates values or controls saving.
Users select placement (label, control, or strip; default strip) and severity colors.
Text wraps with an inline severity prefix, so meaning is not color-only.

## Icon Resolution

The host-only `IconResolver` is backed by the generated Phosphor catalog and
is never compiled into clients. It resolves a valid explicit raw glyph or exact
canonical/accepted alias first, then primary metadata, then secondary
metadata. Full-label authoritative terms precede the longest whole
authoritative phrase, which precedes the longest descriptive tag phrase.
Canonical names and accepted aliases outrank reviewed domain terms at the same
phrase length. If equally ranked terms identify several glyphs, the resolver
chooses the lowest pinned glyph codepoint so the result is stable and
independent of metadata order. A primary match is final; secondary metadata is
consulted only when primary metadata has no match. Only a genuine no-match
result uses the caller's declared surface fallback.

Normalization performs ASCII case folding, collapses punctuation and
separators to word boundaries, and preserves lower/digit-to-uppercase and
acronym-run boundaries. Thus spaces, hyphens, underscores, and forms such as
`DearModdingUI` normalize consistently. Short terms such as `AI` and `UI`
match whole words only.

The host applies that shared selection policy as follows:

| Target | Metadata and fallback |
|---|---|
| Client | Display name; category display names; Question. |
| Category heading | Category display name; Question. |
| Page (Palette) | Page display name; category display name; Files. |
| Action | Action label; text-only toolbar or Terminal Window palette. |
| Setting group | Group label; existing Question heading fallback. |

An unknown or blank well-formed explicit name falls through to metadata.
Malformed or oversized descriptor strings (over 128 bytes) still reject the
descriptor. Raw zero remains no-icon for section/link operations, while an
unset `SettingGroup::glyph` requests automatic inference. Invalid raw Unicode
is not treated as a semantic miss.

The `DMUI_HostAPI::resolveIconGlyph` entry makes automatic client
drawing host-authoritative. It is a stateless, thread-safe, no-render query
over immutable host data and does not require host readiness, an active frame,
or a client handle. `explicitName` is limited to 128 bytes and
each metadata field to 256 bytes; null and empty are equivalent. Every string
must terminate within its limit and may not contain bytes below `0x20` other
than tab. A successful zero glyph means no match. Errors also zero a valid
output pointer.

`Client::ResolveIconGlyph(primaryMetadata, explicitName, secondaryMetadata)`
requires a connected client so it can use the host table, although
the underlying C operation itself has no lifecycle or thread-affinity
requirement. It returns an engaged zero for a successful no-match and
`std::nullopt` for failure, with `LastResult()` preserving the host result.
Serialize access to a shared `Client` instance; unlike the underlying pure
query, the wrapper updates mutable client state.
There is no local resolver fallback.

An automatic `SettingGroup` resolves its current label on each draw (or its key
when the label is empty), then uses Question only for a successful no-match.
A nonzero explicit glyph, including Question or an invalid raw scalar, is
forwarded unchanged. Divider groups bypass resolution. Resolver errors fail the
page draw; the existing host callback isolation permanently disables a page
whose callback returns that failure. Mods need one rebuild to adopt this API
path, after which future vocabulary changes are host-only. `IconGlyphs.h`
exposes only the glyph constants and `IsRepresentableIconGlyph`.

## Localization

The host reads translations itself from `Data\Interface\Translations\<owner>_<language>.txt`, the MCM file format, where `<language>` is the game's `sLanguage`. A key missing from that file falls back to `<owner>_en.txt`. A translation key is a whole string that starts with `$` and contains no whitespace or control characters, such as `$MyMod_General`. Text that merely contains `$` is literal.

- Registration text: when registration closes, the host replaces every client, category, page, and action display name, page summary, action tooltip, and bridge source label that is a key found in the client's files, whose owner is the client ID. Ordering, search, and the sidebar use the translated text. A key found in neither file is shown as written. Stable IDs, icon names, and page ImGui IDs never change. Automatic icons are guessed only from names that are not keys, so set `iconName` on translated entries.
- Drawn text: labels, tooltips, notifications, dialogs, and other per-frame strings are drawn exactly as passed. Translate them with `resolveText` before drawing, ideally once in `onHostReady`.

`resolveText(owner, key, buffer, capacity, requiredCapacity)` writes the NUL-terminated translation of `key` from `owner`'s files and needs no client handle. It returns `TEXT_NOT_FOUND` when neither file has the key, `HOST_NOT_READY` until the game language is known, and `BUFFER_TOO_SMALL` when `capacity` is less than `requiredCapacity`, the byte count including the terminator. It may be called from any thread. `Client::ResolveText(owner, key)` returns the translation or `std::nullopt`, and `Client::Localize(owner, key, fallback)` returns the fallback when there is none. The built-in `SettingsPage` text (`$DMUI_SettingsPage_*`, in `SettingsPageText`) resolves once against the host's own `DearModdingUI` owner, with English fallbacks; an empty `SettingFilterOptions::searchHint` or `ChoiceSettingControl::unmatchedLabel` uses it. `<DearModdingUI/Localization.h>` keeps `localize::LocalizeString` values that `LocalizationManager::Load(owner)` refreshes through the same lookup.

## External Open and Virtual Files

`Client::OpenExternal` dispatches targets via the host process:
- Accepts URIs, absolute file paths, and absolute directory paths.
- An optional application path overrides the default OS handler with an argv array.
- `VIRTUAL_FILE` and `VIRTUAL_FILE_PARENT` resolve physical backing files for virtualized paths (such as loose mod files managed by Mod Organizer 2 / USVFS).
- Path resolution is synchronous and read-only. Unresolved paths do not fall back to virtual paths or guessed directories.

## Thread Affinity and Resources

### Managed overlay placement

`configureOverlay` supplies author defaults, not client-persisted placement.
Completed arrangements are stored in the host's existing `imgui.ini` as
`[DMUIOverlay][<hex client ID>/<hex page ID>]` with `Anchor=n` and `Size=x,y`.
Free overlays also store `Offset=x,y`, the host-unscaled viewport position, and
restore it only when both saved and current anchors are free. Anchored overlays
retain the author's inset and persist size only. Records without an anchor
restore size only. Hex encodes UTF-8 bytes and avoids ini delimiters.
Size is pixel geometry, matching `DMUI_ManagedOverlayPlacement::size`; it is not
multiplied by host or content scale, but current size constraints still apply.
Unconfigured entries survive writes. Saved geometry wins on first configuration;
subsequent changed offset/anchor or size defaults apply once to their component.

`DMUI_Result resetOverlay(DMUI_ClientHandle client, DMUI_PageHandle page)` removes
that page's saved arrangement and reapplies its current defaults once. It uses
the same client/page ownership and thread contract as configure/query, including
stale and foreign page rejection.

### Focused overlays

A focused overlay is an interactive standalone surface, such as a console, quick palette, or inspector, while the shell stays closed. At most one page holds focus.

`requestOverlayFocus(client, page)` grants focus to a configured managed overlay page that has frame demand. It returns `PAGE_NOT_FOUND` for an unowned or unconfigured page, `INVALID_PAGE_KIND`, `NO_FRAME_DEMAND`, `CALLBACK_FAILED` for a disabled page, and the host state result unless the host is ready. It returns `BUSY` while the shell is visible or another page holds focus, including another page of the same client. Requesting the focused page again succeeds without a new grant. Each grant increments the page's generation.

While an overlay is focused:
- The host pushes its cursor carrier menu, which blocks game input and pauses the game unless Fall Souls mode is enabled.
- The overlay accepts keyboard and mouse input, including the mouse wheel for its content and `drawTextView` regions. The host brings it to the front and gives it window focus on each grant and whenever window focus would otherwise land on no window.
- `allowArrangement` permits moving and resizing, and completed arrangements persist as in the shell. Otherwise the overlay is fixed.
- Escape and controller B never reach the game. Each press first leaves an active widget, then closes the top client popup, then ends focus with `CANCELED`. An `inputText` field with `CaptureCancel` claims the press instead and reports `Canceled`. There is no other controller navigation.
- The menu toggle and `ALWAYS` hotkey actions remain active. `HOST_INPUT_INACTIVE` and `GAMEPLAY_UNOBSTRUCTED` actions are suppressed.

Focus ends with one `DMUI_OverlayFocusEndReason`:
- `RELEASED`: `releaseOverlayFocus`, or releasing the page's last frame demand. Releasing an unfocused page succeeds without effect.
- `CANCELED`: Escape or controller B that no widget or popup claimed.
- `SHELL_OPENED`: the shell opened, for example from the menu toggle.
- `INTERRUPTED`: a game load or new game, a renderer change, or a carrier menu that the game closed or never showed, detected when the cursor is absent for two seconds.
- `HOST_UNAVAILABLE`: the host became unavailable or shut down.
- `CALLBACK_FAILED`: the overlay's callback failed and was disabled.

Deactivating the game window suspends input but keeps focus. `queryOverlayFocus(client, page, info)` reports `focused`, the latest grant's `generation`, and `endReason`, which is `NONE` while focused or before the first grant. A client polls it from its overlay callback or a frame observer; a change from focused to unfocused is the focus-lost signal, and `endReason` says why. Ownership validation matches `queryOverlay`. All three entries may be called from any thread, and their effect on input and drawing applies from the next frame.

### Keyboard shortcuts

`shortcut(client, chord, flags, pressed)` sets `pressed` on the frame the keyboard chord is pressed. The chord uses the hotkey grammar (`Ctrl+Shift+S`, one key plus optional `Shift`, `Ctrl`, `Alt`); gamepad and mouse chords, multi-key chords, unknown names, and `Escape` (reserved for dismissal) return `INVALID_ARGUMENT`, as do unknown flags (`REPEAT` is the only one) and null arguments. `ui::Shortcut(chord, flags)` wraps it. Call it from the page's draw callback every frame.

The chord reaches only the page whose window has keyboard focus, for shell pages and focused overlays alike, through ImGui's focused shortcut routing: an active text field keeps its own keys, and `pressed` is zero for a chord a host hotkey would act on, the same test `getPointerState` applies to mouse buttons. A page that stops calling `shortcut` for a frame loses its route for one frame.

### Pointer state

The UI operation `getPointerState(client, state)` returns the pointer input that DMUI did not take, so a focused overlay can pick in the world. It returns `OK` for a valid call, and `INVALID_ARGUMENT` for a null `state`. Call it from the page's draw callback.

`position` and `viewport` are reported on every page that draws, so a tooltip or popup can be placed at the mouse. A `position` of `{0, 0}` means the host has no pointer position. Everything else is gated to the page that owns the pointer, which is the page holding overlay focus while the cursor carrier is shown: `active` is set only for that page, and `captured`, `wheel`, `down`, `pressed`, and `released` are zero on any other page.

- `position` and `viewport` are screen-space pixels with a top-left origin. To pick, call `NiCamera::WindowPointToRay(camera, round(x), round(y), origin, direction, viewport.x, viewport.y)`.
- `down`, `pressed`, and `released` are `DMUI_POINTER_BUTTON_*` masks (`LEFT`, `RIGHT`, `MIDDLE`, `X1`, `X2`). They hold only presses that landed on no DMUI window while no popup was open and no DMUI drag was active, plus the release of such a press. ImGui never sees them, so window focus and active widgets are kept. Mouse buttons that a host hotkey would act on, with the held modifiers, are excluded.
- `wheel` reports notches (positive y is away from the user, positive x is right) of wheel input that no DMUI window took and no host hotkey would act on. `captured` is informational: it says whether a DMUI window or popup captures hover and wheel now.
- Edges are per frame and not queued, unlike hotkey edges. `pressed` and `released` may both be set in one frame.
- Mouse movement always reaches DMUI windows.

### Dialog sessions

`dmui::DialogSession` owns a single handle, callback, and text buffer. `Open` and
`Poll` run in the client's render callback; `Poll` consumes completion/cancellation
and stops at pending. Submitted events repeat until resolved, so rejection is
resolved before polling again and acceptance is drained through completion.
`Cancel` rejects any unresolved submission before cancellation and terminal drain;
active destruction has the same render-callback requirement. User exceptions do
not cross the helper boundary. The client must outlive its session.

- **Render Thread**: All drawing calls must execute synchronously inside the registered page, overlay, or action callback on the render thread. Actions run inside the shell's ImGui frame; dialog queries and polling also run here. Settings fields/tables and client popups retain their page-specific requirements.
- **Background / Any Thread**: Image handle release, notification submission, and dialog completion resolution may be called from any thread.
- **D3D11 Texture Views**: Imported shader resource views (`ID3D11ShaderResourceView`) require single-sample 2D textures on the same Direct3D device. The host maintains COM references and per-frame draw leases. Handles carry generational counters to prevent aliasing after handle reuse.
- **CPU Pixel Buffers**: `DMUI_ImageDescriptor` supplies RGBA8 (straight alpha) pixel buffers. The host validates `rowPitch` (minimum `width * 4`) and `accessibleByteCount` before creating textures, copying bytes synchronously so the caller does not need to retain pixel memory after the call returns.
