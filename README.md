# DearModdingUI API

DearModdingUI is a standalone F4SE plugin that hosts a shared Dear ImGui menu for Fallout 4 mods. This repository contains the client-facing headers for its versioned C ABI, header-only C++ client, ImGui forwarding API, compatibility fingerprint, and shared visual helpers.

## Using commonlibf4

The Dear Modding FO4 commonlibf4 fork includes this repository as a public dependency. Plugins that already depend on commonlibf4 can include the API directly with no additional dependency:

```cpp
#include <DearModdingUI/Client.h>
```

## Using the API standalone

Clone this repository and add its `include` directory to the consuming target's include paths. An xmake project can include this repository as a subproject and depend on its header-only target:

```lua
includes("path/to/dearmoddingui-api")
add_deps("dearmoddingui-api", { public = true })
```

The C++ client supports lockstep Dear ImGui and layout-independent forwarding modes on Windows. The fingerprint builder requires Dear ImGui headers; forwarding mode compiles without Dear ImGui and calls the loaded host DLL through `ImGuiForward.h`. Host discovery uses matching narrow Win32 declarations in `Win32Discovery.h`, so public headers do not include `Windows.h` or leak its macros. The C ABI in `API.h` is independent of commonlibf4 and the host binary.

## Shared C++ presentation helpers

`Presentation.h` contains the ImGui-facing helpers that do not require a
registered client. `Client.h` includes it and adds helpers that use client
theme, font, and settings-table services.

```cpp
dmui::DrawStyledText(
	client,
	"Restart required: 100% ## is ordinary text",
	{
		.fontRole = DMUI_FONT_ROLE_SUBTEXT,
		.tone = dmui::TextTone::kStatusRestartNeeded,
		.wrapped = true
	});

dmui::DrawLabeledValue(
	client,
	"Backend",
	"Available",
	{
		.valueStyle = { .tone = dmui::TextTone::kSuccess },
		.spacingScale = 2.0f
	});
```

`TextStyle` fields are `fontRole`, `tone`, and `wrapped`, in that order.
Omitting `fontRole` inherits the current font. `TextTone::kInherit` preserves
the current ImGui text color. The other tones map directly to
`DMUI_ThemeColors`: `kAccent`, `kAccentMuted`, `kMuted`, `kSuccess`,
`kWarning`, `kError`, `kInfo`, `kStatusDisable`, `kStatusError`,
`kStatusWarning`, `kStatusRestartNeeded`, `kStatusCurrentHotkey`,
`kStatusSuccess`, and `kStatusInfo`.

`ResolveTextColor` returns both a `DMUI_Result` and an optional color pointer;
the successful inherit result intentionally has no override. `ThemeToneColor`
is the pointer-only convenience for callers that already validated their
snapshot. `DrawStyledText(text, snapshot, style)` is the client-independent
core; it rejects a non-inherited font role because no client font provider is
available. Both draw functions use length-delimited `TextUnformatted`, so `%`
and `##` are ordinary text and embedded views need not be NUL-terminated.
Wrapping uses the current window, column, or table-cell width.

The client overload of `DrawStyledText` returns false and leaves the exact failure in
`Client::LastResult()` when theme or font acquisition fails. Every successful
font, color, and wrap push is balanced. `FontGuard` exposes `Pushed()`,
`Result()`, and idempotent `End()`; destructor cleanup does not replace a
pre-existing client error with a successful pop.

`DrawLabeledValue` is intentionally an inline label/value layout. It obtains
live style metrics before drawing, places the value after
`itemSpacing.x * spacingScale`, resolves the theme once, and acquires the
requested value font once before drawing either half. While that value font is
held, the label temporarily uses the caller's original font. A service failure
therefore does not leave a half-drawn pair. Inside an existing settings row,
draw only the value with `DrawStyledText`; the row already owns label and value
geometry.

`DMUI_StyleMetrics` fields are `structSize`, `itemSpacing`, `framePadding`,
`itemInnerSpacing`, `cellPadding`, `windowPadding`, `indentSpacing`,
`scrollbarSize`, and `fontSizeBase`, in that order. Forwarding clients obtain
the current unscaled base font size through the existing
`DMUI_GetStyleMetrics` export; there is no separate font-metrics export.

```cpp
dmui::SettingsTableScope table{ client, "rendering" };
if (table.Result() != DMUI_RESULT_OK)
	return;
if (table.Visible())
{
	dmui::SettingsRowScope row{
		client, "quality", "Quality", "Rendering quality."
	};
	if (row.Result() != DMUI_RESULT_OK)
		return;
	if (row.Visible())
	{
		// Draw the value control.
		const auto reset = row.End(true, canReset);
		if (!reset)
			return;
	}
	if (!table.End())
		return;
}
```

`SettingsTableScope` and `SettingsRowScope` own their successful visible
begin calls, are non-copyable and non-movable, and end only brackets they
opened. `Result()` distinguishes failure from clipping, while `Visible()`
preserves the existing clipping contract. Explicit `End()` is idempotent and
never retries a failed end. Row `End(resetVisible, resetEnabled)` returns the
cached `std::optional<bool>` reset result on repeated calls. Destructor cleanup
preserves an earlier client failure.

`DisabledScope` balances `BeginDisabled` for both true and false arguments.
`TooltipScope` checks the caller's hover flags, owns only a successful
`BeginTooltip`, exposes `Hovered()` and `Visible()`, and has idempotent
`End()`. It is suitable for multi-widget tooltip content and does not impose a
title, color, or mod-specific tooltip system.

The generic choice primitive uses an allocation-free span at draw time:

```cpp
const std::array options{
	dmui::ChoiceOption<int>{ 0, "Low", "low", true },
	dmui::ChoiceOption<int>{ 1, "High", "high", false }
};
const auto choice = dmui::DrawChoice(
	"quality",
	current,
	options,
	"Unavailable",
	"Quality mode");
if (choice.changed)
	current = *choice.selected;
```

`ChoiceOption<Value>` fields are `value`, `label`, `key`, and `enabled`.
The stable value and key are independent of the visible label. Duplicate
labels, `%`, and `##` are safe. Empty option spans draw a disabled combo.
Unknown current values show the explicit unavailable preview and are never
clamped or mutated merely by drawing. `changed` and `completed` become true
together only when the user selects a different enabled value.
`Value` is deduced from the current-value argument; arrays and vectors of
matching options convert to the non-deduced span parameter without an explicit
template argument. The optional final display-label argument is drawn
unformatted beside the combo and is independent of `id`; omit it inside an
existing settings-row label/value layout.
`ChoiceSettingOption` is now an alias of `ChoiceOption<std::string>`; aggregate
initializers that need a key or disabled state must use the new field order.

## Forwarding-only services

Forwarding clients can declare `dmui::ClientOptions::requiredServices` and
`minimumForwardingVersion`. `Client::Connect` calls `queryServices` before
registration and returns `SERVICE_UNAVAILABLE` or
`FORWARDING_VERSION_MISMATCH` without creating a partial client. Host service
flags are availability promises and are separate from client capability
permissions such as `RENDERER_REPLACEMENT`. Callers that do not opt into
requirements use the current 0.1 registration contract.
Require `DMUI_HOST_SERVICE_NAVIGATION_ICONS` when category-heading and
page-palette icon names are required presentation behavior. The preflight
checks both page and category registration entries.

The 0.1 table supplies official wrappers for frame demand
(`RequestFrame`/`ReleaseFrame`), swapchain attachment, contextual hotkey
enablement, D3D11 images, managed overlays, notifications, annotated plots,
submission-aware dialogs, generic CPU-pixel images, first-class categories,
and generic external opening. DearModdingUI remains pre-release at product
version 0.1.0 and API version 0.1. The Dear Modding team maintains all consumers,
so API improvements may break earlier development snapshots without changing
these versions. Use matching host and client headers; compatibility shims are
not maintained for superseded development interfaces. CPU producers require the separate
`DMUI_HOST_SERVICE_PIXEL_IMAGES` bit; imported-SRV availability remains
`DMUI_HOST_SERVICE_IMAGE_RESOURCES`.

Register each client category once with `Client::AddCategory` before adding a
page that references its stable `categoryId`. Category display names and sort
keys are independent of page metadata; all-zero category sort keys order by
display name and then stable ID. Empty category IDs remain ungrouped.
`CategoryDescriptor::iconName` and `PageDescriptor::iconName` are optional
trailing fields forwarded with the current structure size. The C ABI retains
the old 32-byte category and 64-byte page prefixes; hosts read the appended
pointers only at `DMUI_CATEGORY_DESCRIPTOR_ICON_SIZE` and
`DMUI_PAGE_DESCRIPTOR_ICON_SIZE`.

Named navigation icons use this precedence:

| Source | Precedence |
| --- | --- |
| Client | valid explicit name, category metadata, whole-word display-name concept, question |
| Category heading | valid explicit name, existing client-category rule, category concept, question |
| Page in command palette | valid explicit name, page-name concept, category concept, Files |
| Action in command palette | valid explicit name, action-label concept, Terminal Window |

Canonical Phosphor names and normalized spellings are accepted. Unknown or
blank well-formed names fall through instead of rejecting registration;
malformed or oversized strings reject the descriptor. Page icons are not
drawn in plain sidebar or title rows. Toolbar/header actions intentionally
remain text-only when their icon is absent or unknown.

For example, a Community Shaders client may keep `iconName = "cloud-sun"`
while registering `{ .id = "lighting", .displayName = "Lighting",
.iconName = "sun-horizon" }`; the category selection is independent of the
client selection. There is no icon-name editor, curated mod table, API version
bump, or product version bump for this appended 0.1 extension.

Links and section headers already take a raw glyph: zero means no icon, and an
unavailable glyph keeps the existing text fallback. `SettingGroup::glyph`
similarly uses a chosen nonzero glyph or automatic label inference, while
`HeadingMode::kDivider` remains explicitly iconless.

`Client::OpenExternal` accepts a URI, absolute file, or absolute directory for
the OS-associated handler. Supplying an absolute application path overrides
that handler; arguments are an argv array, the target is appended after those
arguments, and no command shell or placeholder expansion is used. An explicit
application may launch with `targetKind == NONE`. `dmui::Link` independently
chooses `kCopyTarget` or `kOpenExternal`, so copy links never become launches
implicitly. External launch success means Windows accepted the dispatch or
created the process, not that its window rendered.

Require `DMUI_HOST_SERVICE_VIRTUAL_FILE_TARGETS` for the explicit
`VIRTUAL_FILE` and `VIRTUAL_FILE_PARENT` target kinds. Both take an absolute
path to an existing file visible to the host process. The former opens that
file's physical backing file; the latter opens its physical containing
folder. Application overrides and arguments work as above. Ordinary `FILE`
and `DIRECTORY` targets are never implicitly resolved, and copy links still
copy the supplied target without resolution.

Resolution is read-only and synchronous. The mapped-file resolver supports
readable, nonempty loose files, including the winning file exposed through
MO2/USVFS. Empty files, directories, archive interiors, and backing namespaces
without a supported external path are not resolved. An unresolved target
never falls back to its virtual name, an ancestor folder, or a guessed mod
directory. `EXTERNAL_RESOLUTION_FAILED` reports a resolution failure;
`EXTERNAL_RESOLUTION_UNSUPPORTED` reports a known unsupported case.
`EXTERNAL_OPEN_FAILED` means resolution succeeded but Windows rejected the
launch. `nativeError` preserves the relevant Windows error.

This finds an existing read winner, not a future write or Overwrite
destination. Opening the backing file may edit an installed mod directly;
creating a user override remains client policy. Files can change between
resolution and the external program reopening the path, and launching does
not opt out of the mod manager's child-process injection.

New drawing calls are valid only on the render thread while the owning page
callback is active. Image import, CPU creation, and CPU update instead require
a ready backend and the bound render thread, so they are valid from a frame
observer without forcing a UI draw or frame demand. Image queries require no
draw phase. Image release and notification posting are thread-safe. Dialog
request and polling run in an owning render-thread callback;
submission resolution and cancellation may be
returned from another thread. Registered callbacks and their user data remain
process-lifetime except for hotkeys, which retain their documented explicit
unregister operation.

`DMUI_D3D11ImageDescriptor::shaderResourceView` must point to a live
`ID3D11ShaderResourceView` during import. The host accepts same-device,
single-sample Texture2D views with one array slice and normalized or floating
sampleable formats. It retains a COM reference and a separate lease for every
queued draw through `RenderDrawData`. A handle is owner-scoped and carries the
device generation; device replacement invalidates it, while ordinary
same-device backbuffer resize does not. Releasing a handle does not invalidate
an already queued draw. Released and invalidated handles remain queryable only
until their storage slot is reused; reuse advances a per-slot generation, so an
older handle returns `STALE_HANDLE` and never aliases the replacement. A slot
whose generation is exhausted is retired rather than wrapped.

`DMUI_ImageDescriptor` supplies decoded ordinary pixels without exposing D3D
or ImGui types. The initial format is `DMUI_PIXEL_FORMAT_RGBA8_UNORM`, with
bytes ordered R, G, B, A. Alpha is straight, not premultiplied. The host does
no alpha premultiplication, color-space conversion, decoding, or tonemapping.
`rowPitch` must be at least `width * 4` and fit the D3D11 pitch field.
`accessibleByteCount` must cover exactly the bytes the host may read:
`(height - 1) * rowPitch + width * 4`; padding after the final row is not
required, including the one-row case. Arithmetic, format, reserved fields,
dimensions, device state, thread, owner, handle generation, and provenance
are validated before pixel memory or GPU creation. The caller keeps `pixels`
valid only for the call; the host synchronously copies all referenced pixel
bytes before returning and never retains the pointer.

`createImage` publishes a new ordinary image handle. `updateImage` accepts only
handles created from CPU pixels; imported SRVs return `UNSUPPORTED_RESOURCE`.
An update creates a replacement Texture2D/SRV first and commits it to the same
handle only after rechecking owner, handle, device, and generation. Width and
height may change. Validation or allocation failure leaves the old view,
dimensions, status, and generation unchanged. Draws queued before the commit
retain the old SRV through submission, while later draws use the replacement.
Device replacement invalidates CPU and imported handles alike; rebinding the
same device for a backbuffer resize preserves them. Imported pixels remain
producer-owned and are not copied, so imported producers must order GPU writes
before sampling and supply any required conversion.

Managed overlays are opt-in per existing overlay page. Coordinates, offsets,
and constraints are logical ImGui/backbuffer coordinates and are scaled by
the host scale; `contentScale` is a continuous multiplier in the inclusive
0.5..3 range. Anchored overlays are host-positioned. Free overlays can move
only while the host menu owns input and `allowArrangement` is set. Outside
that mode they use `NoInputs`, never request a cursor, and never consume
gameplay input. `queryOverlay` reports position, size, change generation, and
an arrangement-completed edge so the client remains the sole persistence
owner.

Notifications copy at most 1024 message bytes, use a clamped 250..30000 ms
duration (zero means 4000 ms), and use one latest-message-wins slot. They
independently demand frames and render without input outside the modal menu.
Annotated plots consume the sample and reference arrays during the call,
accept at most 1,000,000 samples and 64 reference lines, and reject non-finite
samples, lines, sizes, or ranges. Empty plots are valid. New array counts,
offsets, and dialog buffer capacities use fixed-width `uint32_t` ABI fields.

Dialogs copy all descriptor strings and permit one active request. Requests
require a visible host menu. Text capacity is bytes including the NUL, from 1
through 4096; the current contract does not split or validate UTF-8
codepoints. `SUBMITTED` carries a monotonic submission ID. Resolving success
produces `COMPLETED`; rejection copies an error, preserves the entered text,
and returns to `PENDING`. The rejection error is optional, so `nullptr` and an
empty string both mean no displayed error. A small poll buffer reports the required capacity
without truncating or consuming the event. Escape/menu close cancels only a
not-yet-submitted dialog; submitted work remains pending until resolved.

Hotkey chords additionally accept ASCII A-Z and 0-9, canonically uppercased.
`HOST_INPUT_INACTIVE` yields while the host menu, its dialogs, or text editing
owns input. `GAMEPLAY_UNOBSTRUCTED` additionally requires the current input
message's safe engine UI snapshot to report no active menu mode; an unavailable
snapshot yields to the game. Dynamic
enablement is thread-safe and checked before a press is consumed. Once a
press is owned, its release remains owned across policy/enable changes.
Focus loss synthesizes the queued release before forgetting ownership.

Declarative settings continue calling `binding.set` for every live edit.
`SettingDescriptor::onEdit` separately reports `changed` and `completed`
immediately after the value widget. Checkbox and choice edits complete
immediately; scalar/text controls use `IsItemDeactivatedAfterEdit`. An effective
reset is one discrete edit with both flags set, while disabled and already
default settings do not write or emit an edit event.
`TextSettingControl::multiline` uses an ordinary stable client-owned string
buffer and a three-line editor; completion does not imply persistence.

See the [ABI and lifecycle documentation](https://github.com/Dear-Modding-FO4/DearModdingUI/blob/main/include/DearModdingUI/README.md) for discovery, registration, compatibility, callback, and example details.

## Continuous integration

The `xmake` workflow compiles the public headers and runs the commonlibf4 sync integration tests for pushes and pull requests targeting `main`, as well as manual runs. Pull request jobs only build and test; they never receive the publishing credential.

After a successful build of `main` in the canonical API repository, the workflow advances `lib/dearmoddingui-api` on `Dear-Modding-FO4/commonlibf4` to the exact API commit built by that run. The sync is an explicit no-op when the pointer already matches and an explicit successful skip when a newer API commit is already published. Invalid gitlinks, candidates outside API `main`, divergent history, credential failures, and non-race push failures stop the job. Concurrent non-fast-forward updates are retried at most three times from the latest commonlibf4 `main`, preserving unrelated changes.

Publishing requires a fine-grained personal access token stored as the `COMMONLIBF4_TOKEN` Actions secret. Grant the token access only to `Dear-Modding-FO4/commonlibf4` with repository **Contents: Read and write** permission. To retry or intentionally publish the current API `main`, run the `xmake` workflow manually from the Actions tab on the `main` branch; the same build, provenance, ancestry, and push checks apply.

## License

DearModdingUI API is licensed under GPL-3.0. Including these headers makes the consuming plugin a derivative work and requires the plugin to comply with GPL-3.0, including its source-distribution requirements when conveyed.

`ImGuiFingerprint.h` derives compatibility information from MIT-licensed Dear ImGui declarations. `ImGuiForward.h` derives API declarations from MIT-licensed cimgui. See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
