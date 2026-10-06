# DearModdingUI API Specification

This document details the low-level contracts, binary layout requirements, thread affinity rules, and visual helper semantics for the DearModdingUI client API.

## ABI Versioning

The ABI version is `DMUI_ABI_MAJOR.DMUI_ABI_MINOR` and covers every public table and struct. Clients request their header version with `DMUI_ABI_VERSION`, then retry lower minors of the same major until `DMUI_GetAPI` returns a table. It packs the minor into the high 16 bits, so ABI 2.0 is the value `2` requested by released ABI 2 clients. The host returns its single current table when the major matches and the requested minor is at most its own. Otherwise it logs both versions and returns null. `DMUI_HostAPI::abiMajor` and `DMUI_HostReadyInfo::abiMajor` carry the major only.

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
wrapper conveniences. `BeginItemTooltip` uses `ForTooltip` hover policy and
`BeginTooltip`. `TextAligned(alignX, width, text, length)` renders unformatted
UTF-8 with alignment clamped to [0,1]. Nonpositive width uses available content
width; overflow is ellipsized and exposes full text on hover/nav focus.

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
reject embedded NUL bytes, use the
`drawSearchInputBuffer` operation, and do not fall back to the legacy fixed C
entry.

The bundled native input widget intentionally suppresses single-line glyph
rendering above 2 MiB. This limits display work only; it is not a wrapper cap
and does not change the growable storage contract or the `INT_MAX` capacity
representation bound.

### Input Text Editor (ABI 2.1)

`inputTextEditor` is a single-line input over the same editing core and `DMUI_TextBuffer` contract as `drawSearchInputBuffer`. It reports per-frame events in `DMUI_TextEditState::events`:
- `Edited`: the text changed this frame.
- `Submitted`: Enter was pressed.
- `HistoryPrevious` / `HistoryNext`: Up / Down, with `HistoryKeys`.
- `Completion`: Tab, with `CompletionKey`.
- `Canceled`: Escape, with `CaptureCancel`.

Each opt-in flag consumes its key for the active field, so arrows do not navigate, Tab does not move focus, and Escape neither reverts nor deactivates. `CompletionKey` rejects `AllowTabInput`. `KeepFocusOnSubmit` reactivates the field on the next frame. `RequestFocus` focuses an inactive field.

The client owns the text. To replace it, the client writes the buffer and passes `Reload` with a byte `cursor` on a UTF-8 boundary. An active field reloads before applying that frame's keyboard input, so typing lands after the placed cursor. An inactive field always displays the buffer and ignores the cursor.

While active, the state reports the cursor, the selection bounds (equal to the cursor when nothing is selected), the caret line's screen-space top-left in `caretPosition`, and its `lineHeight`. These fields are zero while inactive, including the frame of a kept-focus submission. `ui::BeginTooltipAt(position, pivot)` opens a non-focusable tooltip at a screen position, for example a completion list below the caret. The C++ `std::string` overload stages the text through `TextInputBuffer`, like `Client::DrawSearchInput`.

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
path, after which future vocabulary changes are host-only. The header-only
resolver remains useful for offline decisions, but
`ResolveAutomaticIconGlyph` should not be used for host-authoritative drawing
because its vocabulary remains compiled into the mod.

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
stale and foreign page rejection. Clients must rebuild for the revised ABI 2 table.

### Focused overlays (ABI 2.1)

A focused overlay is an interactive standalone surface, such as a console, quick palette, or inspector, while the shell stays closed. At most one page holds focus.

`requestOverlayFocus(client, page)` grants focus to a configured managed overlay page that has frame demand. It returns `PAGE_NOT_FOUND` for an unowned or unconfigured page, `INVALID_PAGE_KIND`, `NO_FRAME_DEMAND`, `CALLBACK_FAILED` for a disabled page, and the host state result unless the host is ready. It returns `BUSY` while the shell is visible or another page holds focus, including another page of the same client. Requesting the focused page again succeeds without a new grant. Each grant increments the page's generation.

While an overlay is focused:
- The host pushes its cursor carrier menu, which blocks game input and pauses the game unless Fall Souls mode is enabled.
- The overlay accepts keyboard and mouse input, including the mouse wheel for its content and `drawTextView` regions. The host brings it to the front and gives it window focus on each grant and whenever window focus would otherwise land on no window.
- `allowArrangement` permits moving and resizing, and completed arrangements persist as in the shell. Otherwise the overlay is fixed.
- Escape and controller B never reach the game. Each press first leaves an active widget, then closes the top client popup, then ends focus with `CANCELED`. An `inputTextEditor` with `CaptureCancel` claims the press instead and reports `Canceled`. There is no other controller navigation.
- The menu toggle and `ALWAYS` hotkey actions remain active. `HOST_INPUT_INACTIVE` and `GAMEPLAY_UNOBSTRUCTED` actions are suppressed.

Focus ends with one `DMUI_OverlayFocusEndReason`:
- `RELEASED`: `releaseOverlayFocus`, or releasing the page's last frame demand. Releasing an unfocused page succeeds without effect.
- `CANCELED`: Escape or controller B that no widget or popup claimed.
- `SHELL_OPENED`: the shell opened, for example from the menu toggle.
- `INTERRUPTED`: a game load or new game, a renderer change, or a carrier menu that the game closed or never showed, detected when the cursor is absent for two seconds.
- `HOST_UNAVAILABLE`: the host became unavailable or shut down.
- `CALLBACK_FAILED`: the overlay's callback failed and was disabled.

Deactivating the game window suspends input but keeps focus. `queryOverlayFocus(client, page, info)` reports `focused`, the latest grant's `generation`, and `endReason`, which is `NONE` while focused or before the first grant. A client polls it from its overlay callback or a frame observer; a change from focused to unfocused is the focus-lost signal, and `endReason` says why. Ownership validation matches `queryOverlay`. All three entries may be called from any thread, and their effect on input and drawing applies from the next frame.

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
