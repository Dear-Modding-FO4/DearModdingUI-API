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

The legacy C++ client and fingerprint builder require the Dear ImGui headers and Windows. `ImGuiForward.h` requires Windows but no Dear ImGui installation; it forwards its ImGui-compatible calls to the loaded host DLL. The C ABI in `API.h` is independent of commonlibf4 and the host binary.

See the [ABI and lifecycle documentation](https://github.com/Dear-Modding-FO4/DearModdingUI/blob/main/include/DearModdingUI/README.md) for discovery, registration, compatibility, callback, and example details.

## License

DearModdingUI API is licensed under GPL-3.0. Including these headers makes the consuming plugin a derivative work and requires the plugin to comply with GPL-3.0, including its source-distribution requirements when conveyed.

`ImGuiFingerprint.h` derives compatibility information from MIT-licensed Dear ImGui declarations. `ImGuiForward.h` derives API declarations from MIT-licensed cimgui. See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
