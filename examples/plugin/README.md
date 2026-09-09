# Example F4SE Plugin Integration

This example demonstrates how an F4SE mod plugin integrates with DearModdingUI using the header-only C++ client (`<DearModdingUI/Client.h>`).

## Features Demonstrated

- **Host Connection**: Discovers `DearModdingUI.dll` at runtime during F4SE `kPostPostLoad` without linking against host binaries.
- **Categorized Navigation**: Groups related pages under structured categories (`kLightingCategory`, `kPresetsCategory`, `kToolsCategory`) with Phosphor icons and sort order keys.
- **Multi-Page Layout**:
  - **Overview & Status**: Displays system summary, formatted status banners (`DrawStyledText`), and live key-value readouts (`DrawLabeledValue`).
  - **Volumetrics & Sun**: Two-column settings table using `SettingsTableScope` and `SettingsRowScope`, complete with reset button handling.
  - **Color Grading**: Type-safe combo dropdown via `DrawChoice` with custom selection handling.
  - **Telemetry**: Live frame time plotting via `dmui::ui::PlotLines` and host status reporting (`SetStatus`).
- **Command Palette Action**: Registers a callable global action (`AddAction`) accessible from the host menu search or action list.
- **Frame Lifecycle Observer**: Samples frame deltas safely on the render thread via `AddFrameObserver`.

## Building the Example

The example builds directly via xmake:

```powershell
xmake build example-plugin
```
