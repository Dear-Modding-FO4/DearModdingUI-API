# DearModdingUI API Specification

This document details the low-level contracts, binary layout requirements, thread affinity rules, and visual helper semantics for the DearModdingUI client API.

## Stable UI Contract and Schema

`schema/ui-contract.json` defines the current C UI contract. `schema/ui-contract.manifest.json` records the latest published ABI-1 revision: validation fails if an existing operation ID, slot, signature, requirement, enum family, or enum value changes. Pass `--update-baseline` to the generator when publishing a new revision.

New operations or enum values must only be appended under a newer UI revision. The generated files are:
- `CUIAPI.h`: The C function table structure.
- `UIChecked.generated.h`: Explicit result-returning C++ wrappers.
- `UI.h`: The familiar boolean/void C++ facade (`dmui::ui`).

The host translates every stable enum and flag symbolically. Values do not necessarily match native Dear ImGui enum numeric values. Unknown flag bits return `DMUI_RESULT_INVALID_ARGUMENT`.

### Optional Slots and Minimum Prefixes

The required table prefix ends at `NewLine`. Subsequent slots are optional and additive (revision 1 adds `PlotLines`; revision 2 adds `PushStyleVarFloat`, `PushStyleVarVec2`, `PopStyleVar`, `ListClipperBegin`, `ListClipperStep`, and `ListClipperEnd`). A client can connect to a host that supplies its required prefix even if a newer optional tail is absent. Invoking an unsupported operation returns `DMUI_RESULT_UNSUPPORTED_ABI`.

Drawing wrappers record the first failure in a callback-scoped sticky result. The trampoline passes this result back to the host, which disables the malfunctioning callback. Scope-end operations continue to dispatch so nested stacks unwind cleanly.

`ListClipper` clips uniform-height rows. Counts must be nonnegative and less than
`INT32_MAX`; heights must be finite, with values <= 0 measuring the first row.
`Begin` returns a host-owned token; `Step` returns half-open display ranges and
releases the token when stepping finishes. `End` releases early; zero and already
finished tokens are no-ops. Nested clippers must be stepped and ended in LIFO
order by their owning client in the original window and table. Violations return
`DMUI_RESULT_UNBALANCED_BRACKET`. Tokens last only for the drawing callback;
the host abandons unfinished clippers at callback exit without seeking the cursor.
The non-copyable, non-movable C++ wrapper ends on destruction and before re-Begin.

`ClientOptions::minimumHostAPISize` applies the same prefix rule to the host
table. `PreflightHostAPI` verifies every function through that byte size before
registration, in addition to semantic service checks and stable UI preflight.
Set it to `DMUI_HOST_API_DRAW_TEXT_VIEW_SIZE` when the viewer and preceding host
widgets are required, or `DMUI_HOST_API_DRAW_SEARCH_INPUT_BUFFER_SIZE` when
growable C++ search input is required.

API feature version 0.2 adds the text-view descriptors and monospace font role.
The host table is only appended, so the host ABI remains ABI 1. Clients that do
not require the appended viewer slot keep the default registration prefix and
are not rejected solely for compiling against the newer feature header.

## Drawing Operations and Type Mapping

| Dear ImGui Type | DearModdingUI Equivalent | Notes |
|---|---|---|
| `ImGui::*` | `dmui::ui::*` | Checked C++ facade. |
| `ImVec2`, `ImVec4` | `dmui::ui::Vec2`, `dmui::ui::Vec4` | Equivalent standard value types. |
| `ImGuiCol_*` | `dmui::ui::Color` | Scoped enum. |
| `ImGuiDataType_*` | `dmui::ui::DataType` | Scoped enum. Scalar functions carry explicit types and sizes. |
| Native flags | `dmui::ui::*Flags` | Scoped bitfield enums. |
| `PushFont` | `dmui::FontGuard` | Takes a `DMUI_FontRole`, never raw font pointers. |

`InputText`, `InputTextWithHint`, and `InputTextMultiline` accept standard buffer, capacity, and flag parameters, but omit native callback and userdata pointers across the ABI boundary.

`DMUI_DrawSearchInputFn` edits the caller buffer directly. Capacity includes
the NUL terminator and must be no greater than `INT_MAX`. Editing never truncates
the existing value. New input is limited to the remaining capacity and may be
inserted partially, clipped at a complete UTF-8 boundary. Hosts retain this
entry as the fixed-buffer form of the same editing core used by
`DMUI_DrawSearchInputBufferFn`.

`DMUI_DrawSearchInputBufferFn` accepts `DMUI_TextBuffer`. A null resize callback
retains the fixed-capacity behavior. A nonnull callback may replace the
frame-local writable allocation during the draw so a large edit completes in
the same frame. The callback receives a minimum capacity including the NUL and
returns storage of at least that size while preserving prior bytes. It is
nonreentrant, cannot issue drawing calls, and is valid only for the active draw
call. The host retains no buffer, callback, or userdata pointer. Callback
failure must leave the prior allocation and input data/capacity values intact.

`TextInputBuffer` is the reusable C++ owner for this contract. It copies the
input into a small padded candidate, grows geometrically through the callback,
and limits all capacities to `INT_MAX`. Construction and callback failures are
reported as `DMUI_Result`. After a successful draw it shrinks the candidate to
the terminating NUL without allocation and swaps it into the caller string, so
resize and draw failures preserve the original value.

`Client::DrawSearchInput(id, hint, text)` uses a growable `TextInputBuffer`
without an application cap. The overload with `maximumBytes` uses a fixed
buffer of `maximumBytes + 1` and no resize callback. It rejects an existing
value beyond the maximum and a maximum of `INT_MAX` or greater. Fixed insertion
may accept a UTF-8-safe prefix but never truncates existing text. Both overloads
reject embedded NUL bytes, require the appended
`drawSearchInputBuffer` operation, and do not fall back to the legacy fixed C
entry.

The bundled native input widget intentionally suppresses single-line glyph
rendering above 2 MiB. This limits display work only; it is not a wrapper cap
and does not change the growable storage contract or the `INT_MAX` capacity
representation bound.

### Read-Only Text View

`drawTextView` owns the child region, vertical and horizontal scrolling,
visible-line clipping, monospace font selection, and match highlighting.
`DMUI_TextViewDescriptor` borrows immutable UTF-8 text, complete line-start
offsets, sorted overlapping match offsets, a shared match byte length, stable
content/search revisions, and the viewport. It performs no I/O or parsing and
retains none of those pointers. Text cannot contain embedded NUL bytes. A
nonzero match byte length is valid with zero match offsets, representing a
nonempty query with no matches.

`DMUI_TextViewState` contains only caller presentation state: revision identity,
the active match index, and a one-shot byte offset to reveal. The C++ helpers
reset stale state, wrap previous/next match selection, and route section or
source navigation through the same reveal field. Stale state is reset rather
than rejected. A direct section or source reveal clears the active search match.
Invalid line, match, active, or reveal offsets return `DMUI_RESULT_INVALID_ARGUMENT`.

`DrawTextViewNavigation` lays out a caller-owned span of section or source
buttons with wrapping derived from `DMUI_StyleMetrics`. Each button is clamped
to the available pane width. Clipped labels expose their full literal text,
including `##`, in a hover tooltip. A projection supplies each label and byte
offset; the helper introduces no separate anchor model.

`DMUI_FONT_ROLE_MONOSPACE` is provisioned through the host's
`AddFontDefaultVector` path. It never falls back to a configured proportional
font family.

## Presentation Helpers

### Text Styling and Tones

`TextStyle` fields are ordered: `fontRole`, `tone`, and `wrapped`.
- Omitting `fontRole` inherits the active font.
- `TextTone::kInherit` retains the active text color.
- Other tones map directly to host theme colors (`kAccent`, `kAccentMuted`, `kMuted`, `kSuccess`, `kWarning`, `kError`, `kInfo`, `kStatusDisable`, `kStatusError`, `kStatusWarning`, `kStatusRestartNeeded`, `kStatusCurrentHotkey`, `kStatusSuccess`, and `kStatusInfo`).

`DrawStyledText` renders length-delimited unformatted text: characters such as `%` and `##` are treated as literal characters and string views do not require NUL termination.

### Style Metrics Layout

`DMUI_StyleMetrics` layout consists of:
- `structSize`
- `itemSpacing`
- `framePadding`
- `itemInnerSpacing`
- `cellPadding`
- `windowPadding`
- `indentSpacing`
- `scrollbarSize`
- `fontSizeBase`

`GetStyleMetrics` populates through `scrollbarSize` (52 bytes) when given that prefix size, and populates `fontSizeBase` (56 bytes) when given the complete structure size. Callers specify `structSize` and trailing unallocated storage is never overwritten.

### Scopes and Choice Controls

- `SettingsTableScope`: Optional RAII grouping for the standard two-column settings layout.
- `FieldScope`: The single RAII field bracket. Inside a settings table it creates the next row; outside one it owns equivalent standalone geometry. It ends only a bracket it successfully opened, and idempotent `End(showReset, resetEnabled)` calls return the cached Reset result.
- `DisabledScope`: Manages `BeginDisabled` / `EndDisabled` pairs cleanly.
- `TooltipScope`: Manages hover evaluation and `BeginTooltip` / `EndTooltip`.
- `ChoiceOption<Value>`: Represents an option in `DrawChoice` with fields `value`, `label`, `key`, and `enabled`. Selection is type-deduced. If the active value does not match any entry, it renders an explicit "Unavailable" fallback without altering the underlying data.

### Field Feedback

`beginField` / `endField` work in settings-page and overlay-page drawing callbacks,
inside a settings table or standalone. Dialog services expose no custom drawing
callback. Only a successful begin with `visible != 0` requires an end.
`DMUI_FieldBeginOptions` selects label/value (non-empty label required) or full-span
geometry (label optional). Without a label column, under-label feedback uses the
control region. `DMUI_FieldEndOptions` configures Reset; the client changes the value.

`setFieldFeedback` accepts `DMUI_FieldFeedback`: Info, Warning, or Error plus a
NUL-terminated UTF-8 message of at most 16 KiB. Text is copied until field end;
the last call wins, and null/empty text or `ClearFeedback()` clears it. Supply it
after edits for same-frame feedback; absence reserves no space. Invalid severity,
size, ownership, or bracket state returns an error.

The host only presents feedback, never validates values or controls saving.
Users select placement (label, control, or strip; default strip) and severity colors.
Text wraps with an inline severity prefix, so meaning is not color-only.

## Icon Resolution

`IconResolver` is a game-independent, header-only snapshot utility backed by
the generated Phosphor catalog. It resolves a valid explicit raw glyph or exact
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

The appended `DMUI_HostAPI::resolveIconGlyph` entry makes automatic client
drawing host-authoritative. It is a stateless, thread-safe, no-render query
over immutable host data and does not require host readiness, an active frame,
or a client handle. `DMUI_IconResolutionRequest` is size-prefixed:
`DMUI_ICON_RESOLUTION_REQUEST_0_1_SIZE` is the minimum accepted prefix and
larger caller tails are ignored. `explicitName` is limited to 128 bytes and
each metadata field to 256 bytes; null and empty are equivalent. Every string
must terminate within its limit and may not contain bytes below `0x20` other
than tab. A successful zero glyph means no match. Errors also zero a valid
output pointer.

`Client::ResolveIconGlyph(primaryMetadata, explicitName, secondaryMetadata)`
requires a connected client so it can use the negotiated host table, although
the underlying C operation itself has no lifecycle or thread-affinity
requirement. It returns an engaged zero for a successful no-match and
`std::nullopt` for failure, with `LastResult()` preserving the host result.
Serialize access to a shared `Client` instance; unlike the underlying pure
query, the wrapper updates mutable client state.
Missing or short older host entries return `UNSUPPORTED_ABI`; there is no local
resolver fallback.

An automatic `SettingGroup` resolves its current label on each draw (or its key
when the label is empty), then uses Question only for a successful no-match.
A nonzero explicit glyph, including Question or an invalid raw scalar, is
forwarded unchanged. Divider groups bypass resolution. Resolver errors fail the
page draw; the existing host callback isolation permanently disables a page
whose callback returns that failure. Mods need one rebuild to adopt this API
path, after which future vocabulary changes are host-only. The header-only
resolver remains useful for offline decisions, but
`ResolveAutomaticIconGlyph` should not be used for host-authoritative drawing
because its vocabulary remains compiled into the mod.

## External Open and Virtual Files

`Client::OpenExternal` dispatches targets via the host process:
- Accepts URIs, absolute file paths, and absolute directory paths.
- An optional application path overrides the default OS handler with an argv array.
- When `DMUI_HOST_SERVICE_VIRTUAL_FILE_TARGETS` is available, `VIRTUAL_FILE` and `VIRTUAL_FILE_PARENT` resolve physical backing files for virtualized paths (such as loose mod files managed by Mod Organizer 2 / USVFS).
- Path resolution is synchronous and read-only. Unresolved paths do not fall back to virtual paths or guessed directories.

## Thread Affinity and Resources

- **Render Thread**: All drawing calls must execute synchronously inside the registered page or overlay callback on the render thread. Dialog queries and polling also run here.
- **Background / Any Thread**: Image handle release, notification submission, and dialog completion resolution may be called from any thread.
- **D3D11 Texture Views**: Imported shader resource views (`ID3D11ShaderResourceView`) require single-sample 2D textures on the same Direct3D device. The host maintains COM references and per-frame draw leases. Handles carry generational counters to prevent aliasing after handle reuse.
- **CPU Pixel Buffers**: `DMUI_ImageDescriptor` supplies RGBA8 (straight alpha) pixel buffers. The host validates `rowPitch` (minimum `width * 4`) and `accessibleByteCount` before creating textures, copying bytes synchronously so the caller does not need to retain pixel memory after the call returns.
