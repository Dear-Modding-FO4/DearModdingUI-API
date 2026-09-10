# DearModdingUI API Specification

This document details the low-level contracts, binary layout requirements, thread affinity rules, and visual helper semantics for the DearModdingUI client API.

## Stable UI Contract and Schema

`schema/ui-contract.json` defines the current C UI contract. The immutable `schema/ui-contract.manifest.json` serves as the published ABI-1 baseline: validation fails if an existing operation ID, slot, signature, requirement, enum family, or enum value changes.

New operations or enum values must only be appended under a newer UI revision. The generated files are:
- `CUIAPI.h`: The C function table structure.
- `UIChecked.generated.h`: Explicit result-returning C++ wrappers.
- `UI.h`: The familiar boolean/void C++ facade (`dmui::ui`).

The host translates every stable enum and flag symbolically. Values do not necessarily match native Dear ImGui enum numeric values. Unknown flag bits return `DMUI_RESULT_INVALID_ARGUMENT`.

### Optional Slots and Minimum Prefixes

The required table prefix ends at `NewLine`. Subsequent slots are optional and additive (for example, revision 1 adds optional `PlotLines`). A client can connect to a host that supplies its required prefix even if a newer optional tail is absent. Invoking an unsupported operation returns `DMUI_RESULT_UNSUPPORTED_ABI`.

Drawing wrappers record the first failure in a callback-scoped sticky result. The trampoline passes this result back to the host, which disables the malfunctioning callback. Scope-end operations continue to dispatch so nested stacks unwind cleanly.

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

- `SettingsTableScope` and `SettingsRowScope`: RAII scopes managing the standard two-column layout. They end only the brackets they successfully opened. Idempotent `End()` calls return the cached state.
- `DisabledScope`: Manages `BeginDisabled` / `EndDisabled` pairs cleanly.
- `TooltipScope`: Manages hover evaluation and `BeginTooltip` / `EndTooltip`.
- `ChoiceOption<Value>`: Represents an option in `DrawChoice` with fields `value`, `label`, `key`, and `enabled`. Selection is type-deduced. If the active value does not match any entry, it renders an explicit "Unavailable" fallback without altering the underlying data.

## Icon Resolution

`IconResolver` is a game-independent, header-only selector backed by the
generated Phosphor catalog. It resolves a valid explicit raw glyph or exact
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
is not treated as a semantic miss. Older binaries retain the icon helper code
they compiled; this pure inference change does not add a C ABI operation or
replace helper behavior inside an already-built mod.

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
