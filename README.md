<div align="center">

# DearModdingUI API

**Client-facing C ABI and header-only C++ integration library for DearModdingUI.**

DearModdingUI-API enables Fallout 4 F4SE plugins to register settings pages,
draw custom interfaces, and interact with the shared DearModdingUI host menu.

<br>

[![CI](https://img.shields.io/github/actions/workflow/status/Dear-Modding-FO4/DearModdingUI-API/xmake.yml?branch=main&style=for-the-badge&label=CI&logo=githubactions&logoColor=white)](https://github.com/Dear-Modding-FO4/DearModdingUI-API/actions/workflows/xmake.yml)
[![License](https://img.shields.io/badge/license-GPL--3.0-blue?style=for-the-badge)](LICENSE)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](xmake.lua)

<sub>[Features](#features) · [Integration](#integration) · [Quick Example](#quick-example) · [Complete Example](#complete-example) · [Documentation](#documentation) · [Header Guide](#header-guide) · [License](#license)</sub>

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

static dmui::Client g_client{
    "my_mod_id",
    "My Mod Display Name",
    dmui::Version{ 1, 0 },
    "sliders" // Optional Phosphor icon name
};

static bool g_enabled = true;
static float g_scale = 1.0f;

void InitializeUI()
{
    if (!g_client.Connect()) {
        return;
    }

    g_client.AddPage({
        .id = "general",
        .displayName = "General Settings",
        .iconName = "gear"
    },
    [] {
        dmui::ui::TextUnformatted("Configure plugin options below:");
        dmui::ui::Checkbox("Enable feature", &g_enabled);
        dmui::ui::SliderScalar("Scale factor", &g_scale, 0.5f, 2.0f);
    });
}
```

---

## Complete Example

A fully featured, compilable sample plugin is provided in [`examples/plugin/`](examples/plugin/):

- **Multiple categories & icons**: Organizes pages under structured headings with custom Phosphor icon glyphs.
- **Two-column settings tables**: Standardized layouts with row descriptions and reset buttons via `SettingsTableScope` and `SettingsRowScope`.
- **Dropdown choices**: Typed combo selectors using `DrawChoice`.
- **Status & telemetry**: Status banners with `DrawStyledText`, live key-value readouts via `DrawLabeledValue`, and real-time graphs with `dmui::ui::PlotLines`.
- **Global actions & notifications**: Registers palette commands and triggers toast notifications.

Build the example directly:

```powershell
xmake build example-plugin
```

### Host-owned automatic icons

Use the client query when procedural drawing needs the host's current icon
vocabulary:

```cpp
bool DrawDisplayHeading(dmui::Client& client)
{
    const auto glyph = client.ResolveIconGlyph(
        "Display Settings", nullptr, "Graphics");
    if (!glyph)
        return false;
    return client.DrawSectionHeader(
        "Display Settings",
        *glyph ? *glyph : DearModdingUI::PhosphorGlyph::kQuestion);
}
```

An engaged zero means the host found no match. A missing optional host entry or
other failure returns `std::nullopt`; the wrapper never falls back to its local
header vocabulary. `SettingGroup` performs this query automatically whenever
its `glyph` is zero. Mods need one rebuild to adopt this path, then later host
vocabulary updates apply without rebuilding the mod. Explicit nonzero glyphs
and divider groups bypass automatic resolution. The underlying C query is
thread-safe and performs no rendering. The C++ wrapper has no render-thread
requirement, but calls sharing one `Client` must be serialized because they
update its `LastResult()`.

---

## Documentation

- **[Controls Guide](docs/controls-guide.md)**: Visual guide and code snippets for `dmui::ui` widgets, settings tables, choice dropdowns, styled text, font roles, and notifications.
- **[Full Specification](docs/specification.md)**: Deep dive into binary memory layouts, structure sizes, thread affinity rules, and ownership contracts.

---

## Header Guide

| Header | Description |
|---|---|
| `<DearModdingUI/Client.h>` | High-level C++ client interface. Handles discovery, callbacks, and registration. |
| `<DearModdingUI/UI.h>` | Safe C++ drawing facade (`dmui::ui::*`). |
| `<DearModdingUI/Presentation.h>` | UI layout scopes, choice controls, and styled text helpers. |
| `<DearModdingUI/API.h>` | Pure C ABI declarations for host interaction. |
| `<DearModdingUI/CUIAPI.h>` | Low-level C function table for drawing primitives. |
| `<DearModdingUI/IconGlyphs.h>` | Phosphor glyph constants and offline catalog snapshot utilities. Prefer the host query for automatic client drawing. |

---

## Verification

Build the test suites and example plugin:

```powershell
xmake
xmake run api-header-checks
xmake build example-plugin
```

The checked-in Phosphor vocabulary is regenerated offline from pinned source
snapshots:

```powershell
python Tools/GeneratePhosphorGlyphs.py --check
```

---

## License

DearModdingUI-API is licensed under GPL-3.0. See [LICENSE](LICENSE).