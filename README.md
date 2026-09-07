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

## Forwarding-only services

Forwarding clients can declare `dmui::ClientOptions::requiredServices` and
`minimumForwardingVersion`. `Client::Connect` calls `queryServices` before
registration and returns `SERVICE_UNAVAILABLE` or
`FORWARDING_VERSION_MISMATCH` without creating a partial client. Host service
flags are availability promises and are separate from client capability
permissions such as `RENDERER_REPLACEMENT`. Callers that do not opt into
requirements retain the original 0.1 registration behavior.

The additive 0.1 table supplies official wrappers for frame demand
(`RequestFrame`/`ReleaseFrame`), swapchain attachment, contextual hotkey
enablement, D3D11 images, managed overlays, notifications, annotated plots,
submission-aware dialogs, and generic CPU-pixel images. Every prior table
offset, the original 400-byte host table prefix, and every existing
`_0_1_SIZE` boundary remain frozen. CPU producers require the separate
`DMUI_HOST_SERVICE_PIXEL_IMAGES` bit; imported-SRV availability remains
`DMUI_HOST_SERVICE_IMAGE_RESOURCES`.

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
