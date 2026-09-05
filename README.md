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

See the [ABI and lifecycle documentation](https://github.com/Dear-Modding-FO4/DearModdingUI/blob/main/include/DearModdingUI/README.md) for discovery, registration, compatibility, callback, and example details.

## Continuous integration

The `xmake` workflow compiles the public headers and runs the commonlibf4 sync integration tests for pushes and pull requests targeting `main`, as well as manual runs. Pull request jobs only build and test; they never receive the publishing credential.

After a successful build of `main` in the canonical API repository, the workflow advances `lib/dearmoddingui-api` on `Dear-Modding-FO4/commonlibf4` to the exact API commit built by that run. The sync is an explicit no-op when the pointer already matches and an explicit successful skip when a newer API commit is already published. Invalid gitlinks, candidates outside API `main`, divergent history, credential failures, and non-race push failures stop the job. Concurrent non-fast-forward updates are retried at most three times from the latest commonlibf4 `main`, preserving unrelated changes.

Publishing requires a fine-grained personal access token stored as the `COMMONLIBF4_TOKEN` Actions secret. Grant the token access only to `Dear-Modding-FO4/commonlibf4` with repository **Contents: Read and write** permission. To retry or intentionally publish the current API `main`, run the `xmake` workflow manually from the Actions tab on the `main` branch; the same build, provenance, ancestry, and push checks apply.

## License

DearModdingUI API is licensed under GPL-3.0. Including these headers makes the consuming plugin a derivative work and requires the plugin to comply with GPL-3.0, including its source-distribution requirements when conveyed.

`ImGuiFingerprint.h` derives compatibility information from MIT-licensed Dear ImGui declarations. `ImGuiForward.h` derives API declarations from MIT-licensed cimgui. See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
