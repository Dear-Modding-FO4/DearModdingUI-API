<div align="center">

# DearModdingUI API

**Client-facing C ABI and header-only C++ integration library for DearModdingUI.**

DearModdingUI-API enables Fallout 4 F4SE plugins to register settings pages,
draw custom interfaces, and interact with the shared DearModdingUI host menu.

<br>

[![CI](https://img.shields.io/github/actions/workflow/status/Dear-Modding-FO4/DearModdingUI-API/xmake.yml?branch=main&style=for-the-badge&label=CI&logo=githubactions&logoColor=white)](https://github.com/Dear-Modding-FO4/DearModdingUI-API/actions/workflows/xmake.yml)
[![License](https://img.shields.io/badge/license-GPL--3.0-blue?style=for-the-badge)](LICENSE)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](xmake.lua)

<sub>[Features](#features) · [Integration](#integration) · [Quick Example](#quick-example) · [Key Concepts](#key-concepts) · [Header Guide](#header-guide) · [Specification](#full-specification) · [License](#license)</sub>

</div>

---

## Features

- **No Dear ImGui dependencies**: Client plugins do not compile Dear ImGui sources or link against ImGui libraries.
- **Header-only C++ client**: Include `<DearModdingUI/Client.h>` to handle discovery, registration, and drawing.
- **Stable C ABI**: Generated from `schema/ui-contract.json` with backward-compatibility baseline enforcement.
- **Familiar drawing facade**: Draw controls using `dmui::ui::*` functions that mirror familiar ImGui APIs.
- **Host theming and layouts**: Built-in helpers for standardized two-column settings rows, semantic color tones, and font scaling.

---

## Integration

### Using CommonLibF4

The [Dear-Modding-FO4 CommonLibF4 fork](https://github.com/Dear-Modding-FO4/commonlibf4) includes this repository as a public dependency. If your plugin uses that fork, you can include the headers directly without modifying build scripts:

```cpp
#include <DearModdingUI/Client.h>
```

### Standalone with xmake

Add this repository as an include directory or target dependency:

```lua
includes("path/to/dearmoddingui-api")
target("MyPlugin", function()
    -- ...
    add_deps("dearmoddingui-api", { public = true })
end)
```

### Other build systems

Add the `include/` directory of this repository to your compiler include search paths. C++20 or later is recommended.

---

## Quick Example

Register your mod during F4SE `kPostPostLoad` after plugins have loaded:

```cpp
#include <DearModdingUI/Client.h>

static dmui::Client g_client;
static bool g_enabled = true;
static float g_scale = 1.0f;

void InitializeUI()
{
    dmui::ClientOptions options{
        .client = {
            .id = "my_mod_id",
            .displayName = "My Mod Display Name",
        }
    };

    if (!g_client.Connect(options)) {
        return;
    }

    g_client.AddPage({
        .id = "general",
        .displayName = "General Settings",
        .onDraw = [](void*) {
            dmui::ui::TextUnformatted("Configure plugin options below:");
            dmui::ui::Checkbox("Enable feature", &g_enabled);
            dmui::ui::SliderScalar("Scale factor", &g_scale, 0.5f, 2.0f);
        }
    });
}
```

---

## Key Concepts

### Drawing Controls (`dmui::ui`)

Widgets live in the `dmui::ui` namespace:

- Standard widgets: `dmui::ui::Button`, `Checkbox`, `SliderScalar`, `InputText`, `BeginTable`, `EndTable`.
- Geometry types: `dmui::ui::Vec2` and `dmui::ui::Vec4`.
- Scoped enums: `dmui::ui::Color`, `dmui::ui::DataType`, `dmui::ui::TableFlags`.

### Standard Settings Rows

Use RAII scopes to produce uniform settings rows matching the host interface:

```cpp
dmui::SettingsTableScope table{ client, "my_table" };
if (table.Visible())
{
    dmui::SettingsRowScope row{ client, "setting_id", "Setting Label", "Description text" };
    if (row.Visible())
    {
        dmui::ui::Checkbox("##setting_id", &g_enabled);
        row.End(true, true); // (resetVisible, resetEnabled)
    }
    table.End();
}
```

### Choice Combos

Dropdown selections using type-deduced arrays or spans:

```cpp
const std::array options{
    dmui::ChoiceOption<int>{ 0, "Low", "low", true },
    dmui::ChoiceOption<int>{ 1, "High", "high", true }
};

auto choice = dmui::DrawChoice("preset", currentPreset, options, "Custom");
if (choice.changed) {
    currentPreset = *choice.selected;
}
```

### Advanced Capabilities

- **Managed overlays**: Passive in-game HUD elements requested via `RequestFrame` and `ReleaseFrame`.
- **System notifications**: Toast messages displayed through the host notification system.
- **Confirmation dialogs**: Submission-aware modal dialogs without managing custom popup loops.
- **Custom images**: Retained D3D11 shader resource views or host-uploaded RGBA8 CPU pixel buffers.

---

## Header Guide

| Header | Description |
|---|---|
| `<DearModdingUI/Client.h>` | High-level C++ client interface. Handles discovery, callbacks, and registration. |
| `<DearModdingUI/UI.h>` | Safe C++ drawing facade (`dmui::ui::*`). |
| `<DearModdingUI/Presentation.h>` | UI layout scopes, choice controls, and styled text helpers. |
| `<DearModdingUI/API.h>` | Pure C ABI declarations for host interaction. |
| `<DearModdingUI/CUIAPI.h>` | Low-level C function table for drawing primitives. |
| `<DearModdingUI/IconGlyphs.h>` | Phosphor icon glyph constants for labels and headers. |

---

## Full Specification

For detailed binary layouts, struct sizing requirements, thread affinity rules, and memory ownership contracts, consult [docs/specification.md](docs/specification.md).

---

## Verification

Build and run compile-time ABI checks:

```powershell
xmake
xmake run api-header-checks
```

---

## License

DearModdingUI-API is licensed under GPL-3.0. See [LICENSE](LICENSE).