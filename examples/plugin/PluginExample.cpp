#include <DearModdingUI/Client.h>
#include <DearModdingUI/IconGlyphs.h>
#include <DearModdingUI/UI.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace
{
	// Plugin settings state.
	struct LightingSettings
	{
		bool enableGodRays{ true };
		bool interiorFog{ true };
		float godRayIntensity{ 1.0f };
		float shadowSoftness{ 0.75f };
		int activePresetIndex{ 0 };
	};

	LightingSettings g_settings;
	const LightingSettings g_defaults;

	// Global client instance. Must outlive the game process.
	dmui::Client g_client{
		"demo_lighting_overhaul",
		"Commonwealth Lighting",
		dmui::Version{ 1, 0 },
		"sun-horizon"
	};

	// Frame time history for telemetry visualization.
	std::array<float, 60> g_frameTimeHistory{};
	size_t g_frameHistoryOffset{ 0 };

	// Preset options for choice controls.
	const std::array g_presetOptions{
		dmui::ChoiceOption<int>{ 0, "Vanilla Enhanced", "vanilla", true },
		dmui::ChoiceOption<int>{ 1, "Bleak Wasteland", "bleak", true },
		dmui::ChoiceOption<int>{ 2, "Vibrant Commonwealth", "vibrant", true },
		dmui::ChoiceOption<int>{ 3, "Cinematic Noir", "noir", true }
	};

	// Client categories with icons and sort keys.
	constexpr dmui::CategoryDescriptor kLightingCategory{
		.id = "lighting",
		.displayName = "Lighting & Atmosphere",
		.sortKey = 0,
		.iconName = "sun-horizon"
	};

	constexpr dmui::CategoryDescriptor kPresetsCategory{
		.id = "presets",
		.displayName = "Color Presets",
		.sortKey = 1,
		.iconName = "palette"
	};

	constexpr dmui::CategoryDescriptor kToolsCategory{
		.id = "tools",
		.displayName = "Diagnostics & Tools",
		.sortKey = 2,
		.iconName = "chart-line"
	};

	// Page 1: Overview & Status
	void DrawOverviewPage(dmui::Client& a_client)
	{
		(void)a_client.DrawSectionHeader("Active Environment");

		(void)dmui::DrawStyledText(
			a_client,
			"Lighting runtime operating normally with hardware acceleration.",
			{
				.tone = dmui::TextTone::kSuccess,
				.wrapped = true
			});

		dmui::ui::Spacing();

		(void)dmui::DrawLabeledValue(
			a_client,
			"Volumetric Engine",
			"Active (Direct3D 11)",
			{
				.valueStyle = { .tone = dmui::TextTone::kSuccess },
				.spacingScale = 2.0f
			});

		(void)dmui::DrawLabeledValue(
			a_client,
			"Selected Preset",
			g_presetOptions[static_cast<size_t>(g_settings.activePresetIndex)].label.c_str(),
			{
				.valueStyle = { .tone = dmui::TextTone::kAccent },
				.spacingScale = 2.0f
			});

		(void)dmui::DrawLabeledValue(
			a_client,
			"Interior Occlusion",
			g_settings.interiorFog ? "Enabled" : "Disabled",
			{
				.valueStyle = {
					.tone = g_settings.interiorFog ?
						dmui::TextTone::kSuccess :
						dmui::TextTone::kMuted
				},
				.spacingScale = 2.0f
			});

		dmui::ui::Spacing();
		dmui::ui::Separator();
		dmui::ui::Spacing();

		if (dmui::ui::Button("Cycle Active Preset"))
		{
			g_settings.activePresetIndex = static_cast<int>(
				(static_cast<size_t>(g_settings.activePresetIndex) + 1) %
				g_presetOptions.size());

			(void)a_client.PostNotification(
				DMUI_STATUS_SEVERITY_SUCCESS,
				"Weather preset switched successfully.",
				3000);
		}
	}

	// Page 2: Two-column settings table
	void DrawLightingSettingsPage(dmui::Client& a_client)
	{
		(void)a_client.DrawSectionHeader("Volumetrics and Shading");

		dmui::SettingsTableScope table{ a_client, "lighting_table" };
		if (!table.Visible())
			return;

		// Setting 1: God rays checkbox
		{
			dmui::FieldScope row{
				a_client,
				"god_rays",
				"God Rays (Sunshafts)",
				"Enables volumetric light shafts cast by the sun and moon."
			};

			if (row.Visible())
			{
				(void)dmui::ui::Checkbox("##god_rays", &g_settings.enableGodRays);
				const auto resetClicked = row.End(
					true,
					g_settings.enableGodRays != g_defaults.enableGodRays);

				if (resetClicked.value_or(false))
					g_settings.enableGodRays = g_defaults.enableGodRays;
			}
		}

		// Setting 2: Interior fog checkbox
		{
			dmui::FieldScope row{
				a_client,
				"interior_fog",
				"Interior Atmospheric Fog",
				"Enables volumetric dust and fog scattering inside interiors."
			};

			if (row.Visible())
			{
				(void)dmui::ui::Checkbox("##interior_fog", &g_settings.interiorFog);
				const auto resetClicked = row.End(
					true,
					g_settings.interiorFog != g_defaults.interiorFog);

				if (resetClicked.value_or(false))
					g_settings.interiorFog = g_defaults.interiorFog;
			}
		}

		// Setting 3: God ray intensity slider
		{
			dmui::FieldScope row{
				a_client,
				"ray_intensity",
				"Light Shaft Intensity",
				"Scales brightness multiplier for directional light scattering."
			};

			if (row.Visible())
			{
				float minVal{ 0.1f };
				float maxVal{ 3.0f };
				(void)dmui::ui::SliderScalar(
					"##ray_intensity",
					&g_settings.godRayIntensity,
					&minVal,
					&maxVal,
					"%.2fx");

				const auto resetClicked = row.End(
					true,
					g_settings.godRayIntensity != g_defaults.godRayIntensity);

				if (resetClicked.value_or(false))
					g_settings.godRayIntensity = g_defaults.godRayIntensity;
			}
		}

		// Setting 4: Shadow softness slider
		{
			dmui::FieldScope row{
				a_client,
				"shadow_softness",
				"Ambient Shadow Softness",
				"Controls penumbra blur filter radius on contact shadows."
			};

			if (row.Visible())
			{
				float minVal{ 0.0f };
				float maxVal{ 1.0f };
				(void)dmui::ui::SliderScalar(
					"##shadow_softness",
					&g_settings.shadowSoftness,
					&minVal,
					&maxVal,
					"%.2f");

				const auto resetClicked = row.End(
					true,
					g_settings.shadowSoftness != g_defaults.shadowSoftness);

				if (resetClicked.value_or(false))
					g_settings.shadowSoftness = g_defaults.shadowSoftness;
			}
		}

		(void)table.End();
	}

	// Page 3: Presets dropdown
	void DrawPresetsPage(dmui::Client& a_client)
	{
		(void)a_client.DrawSectionHeader("Preset Profiles");

		dmui::ui::TextUnformatted(
			"Choose a curated lighting preset for the active worldspace.");
		dmui::ui::Spacing();

		auto choice = dmui::DrawChoice(
			"lighting_preset_choice",
			g_settings.activePresetIndex,
			g_presetOptions,
			"Custom",
			"Active Color Grade");

		if (choice.changed && choice.selected)
		{
			g_settings.activePresetIndex = *choice.selected;
			(void)a_client.PostNotification(
				DMUI_STATUS_SEVERITY_INFO,
				"Selected profile applied to local cache.",
				2500);
		}

		dmui::ui::Spacing();
		dmui::ui::Separator();
		dmui::ui::Spacing();

		switch (g_settings.activePresetIndex)
		{
		case 0:
			(void)dmui::DrawStyledText(
				a_client,
				"Vanilla Enhanced: True to original art direction with improved volumetric depth.",
				{ .tone = dmui::TextTone::kMuted, .wrapped = true });
			break;
		case 1:
			(void)dmui::DrawStyledText(
				a_client,
				"Bleak Wasteland: Desaturated color tones with cold ambient fog.",
				{ .tone = dmui::TextTone::kMuted, .wrapped = true });
			break;
		case 2:
			(void)dmui::DrawStyledText(
				a_client,
				"Vibrant Commonwealth: Warmer highlights, deeper greens, and vibrant sky gradients.",
				{ .tone = dmui::TextTone::kMuted, .wrapped = true });
			break;
		case 3:
			(void)dmui::DrawStyledText(
				a_client,
				"Cinematic Noir: Crushed blacks with high contrast directional key lights.",
				{ .tone = dmui::TextTone::kMuted, .wrapped = true });
			break;
		default:
			break;
		}
	}

	// Page 4: Telemetry & diagnostics
	void DrawDiagnosticsPage(dmui::Client& a_client)
	{
		(void)a_client.DrawSectionHeader("Performance & Telemetry");

		dmui::ui::TextUnformatted("Frame duration samples (last 60 frames, ms):");
		dmui::ui::PlotLines(
			"##frametime_plot",
			g_frameTimeHistory.data(),
			static_cast<uint32_t>(g_frameTimeHistory.size()),
			static_cast<uint32_t>(g_frameHistoryOffset),
			nullptr,
			0.0f,
			33.3f,
			dmui::ui::Vec2{ 0.0f, 80.0f });

		dmui::ui::Spacing();

		if (dmui::ui::Button("Trigger Test Warning"))
		{
			(void)a_client.SetStatus(
				DMUI_STATUS_SEVERITY_WARNING,
				"Ambient occlusion resolution clamped due to VRAM threshold.");
		}

		dmui::ui::SameLine();

		if (dmui::ui::Button("Clear Status Message"))
		{
			(void)a_client.SetStatus(
				DMUI_STATUS_SEVERITY_INFO,
				"All systems operating within nominal limits.");
		}
	}
}

namespace
{
	// Page 5: A focused overlay console with history, Tab completion, and a caret-anchored popup.
	constexpr std::array<const char*, 5> kCommands{
		"player.additem", "player.setav", "player.placeatme", "tgm", "tcl"
	};

	struct ConsoleState
	{
		std::string input;
		std::vector<std::string> history;
		size_t historyIndex{};
		std::vector<const char*> matches;
		size_t selected{};
		bool popupOpen{};
		dmui::ui::TextEditFlags reload{};
		size_t reloadCursor{};
		DMUI_PageHandle page{ DMUI_INVALID_PAGE_HANDLE };
		bool open{};
		bool activate{};
	};

	ConsoleState g_console;

	void OpenConsole()
	{
		if (g_console.open || !g_client.RequestFrame(g_console.page))
			return;
		if (!g_client.RequestOverlayFocus(g_console.page))
		{
			(void)g_client.ReleaseFrame(g_console.page);
			return;
		}
		g_console.open = g_console.activate = true;
	}

	void CloseConsole()
	{
		if (!g_console.open)
			return;
		// Releasing the last frame demand also ends focus.
		(void)g_client.ReleaseFrame(g_console.page);
		g_console.open = false;
		g_console.popupOpen = false;
	}

	void ReplaceInput(std::string_view a_text)
	{
		// Applied to the active field next frame, before that frame's typing.
		g_console.input = a_text;
		g_console.reload = dmui::ui::TextEditFlags::kReload;
		g_console.reloadCursor = g_console.input.size();
	}

	void RefreshMatches()
	{
		g_console.matches.clear();
		for (const auto* command : kCommands)
		{
			if (!g_console.input.empty() &&
				std::string_view{ command }.starts_with(g_console.input))
				g_console.matches.push_back(command);
		}
		g_console.selected = 0;
		g_console.popupOpen = !g_console.matches.empty();
	}

	void DrawConsolePage(dmui::Client& a_client)
	{
		using Flags = dmui::ui::TextEditFlags;
		using Events = dmui::ui::TextEditEvents;

		// Escape, the shell, a load, or the host can end focus; any loss closes the console.
		const auto focus = a_client.QueryOverlayFocus(g_console.page);
		if (!focus || !focus->focused)
		{
			CloseConsole();
			return;
		}
		const auto activate = std::exchange(g_console.activate, false) ? Flags::kRequestFocus : Flags::kNone;

		for (const auto& line : g_console.history)
			dmui::ui::TextUnformatted(line);

		auto flags = Flags::kHistoryKeys | Flags::kCompletionKey |
			Flags::kKeepFocusOnSubmit | activate | g_console.reload;
		// Escape closes the popup here; otherwise the host leaves the field, then ends focus.
		if (g_console.popupOpen)
			flags |= Flags::kCaptureCancel;
		dmui::ui::TextEditState state{};
		const auto edited = dmui::ui::InputTextEditor(
			"##command", "Type a command", g_console.input, flags,
			g_console.reloadCursor, state);
		g_console.reload = Flags::kNone;

		const auto has = [&](Events a_event) {
			return dmui::ui::HasTextEditEvent(state, a_event);
		};
		if (edited)
			RefreshMatches();
		if (has(Events::kCompletion))
		{
			if (g_console.popupOpen)
			{
				ReplaceInput(g_console.matches[g_console.selected]);
				g_console.popupOpen = false;
			}
			else
				RefreshMatches();
		}
		const auto step = has(Events::kHistoryPrevious) ? -1 : has(Events::kHistoryNext) ? 1 : 0;
		if (step != 0 && g_console.popupOpen)
		{
			const auto count = g_console.matches.size();
			g_console.selected = (g_console.selected + count + step) % count;
		}
		else if (step != 0 && !g_console.history.empty())
		{
			const auto last = g_console.history.size();
			g_console.historyIndex = step < 0 ?
				(g_console.historyIndex > 0 ? g_console.historyIndex - 1 : 0) :
				(std::min)(g_console.historyIndex + 1, last);
			ReplaceInput(g_console.historyIndex < last ? g_console.history[g_console.historyIndex] : "");
		}
		if (has(Events::kCanceled))
			g_console.popupOpen = false;
		if (has(Events::kSubmitted) && !g_console.input.empty())
		{
			g_console.history.push_back(g_console.input);
			g_console.historyIndex = g_console.history.size();
			ReplaceInput("");
			g_console.popupOpen = false;
		}

		// Tooltips never take focus, so typing continues in the field.
		if (g_console.popupOpen && state.active &&
			dmui::ui::BeginTooltipAt({ state.caretPosition.x, state.caretPosition.y + state.lineHeight }))
		{
			for (size_t index = 0; index < g_console.matches.size(); ++index)
				(void)dmui::ui::Selectable(g_console.matches[index], index == g_console.selected);
			dmui::ui::EndTooltip();
		}
	}
}

// Plugin entry point called during F4SE kPostPostLoad.
void InitializeDearModdingUI()
{
	// Attempt connection to host menu.
	if (!g_client.Connect())
		return;

	// Register categories for navigation organization.
	(void)g_client.AddCategory(kLightingCategory);
	(void)g_client.AddCategory(kPresetsCategory);
	(void)g_client.AddCategory(kToolsCategory);

	// Register individual pages under their respective categories.
	(void)g_client.AddPage(
		{
			.id = "overview",
			.displayName = "Overview",
			.categoryId = kLightingCategory.id,
			.summary = "Status and active lighting summary.",
			.sortKey = 0,
			.iconName = "info"
		},
		[] {
			DrawOverviewPage(g_client);
		});

	(void)g_client.AddPage(
		{
			.id = "settings",
			.displayName = "Volumetrics & Sun",
			.categoryId = kLightingCategory.id,
			.summary = "Adjust light scattering and shadow parameters.",
			.sortKey = 1,
			.iconName = "sliders"
		},
		[] {
			DrawLightingSettingsPage(g_client);
		});

	(void)g_client.AddPage(
		{
			.id = "presets",
			.displayName = "Color Grading",
			.categoryId = kPresetsCategory.id,
			.summary = "Select atmospheric and color grading profiles.",
			.sortKey = 0,
			.iconName = "palette"
		},
		[] {
			DrawPresetsPage(g_client);
		});

	(void)g_client.AddPage(
		{
			.id = "diagnostics",
			.displayName = "Telemetry",
			.categoryId = kToolsCategory.id,
			.summary = "Performance charts and health observations.",
			.sortKey = 0,
			.iconName = "activity"
		},
		[] {
			DrawDiagnosticsPage(g_client);
		});

	// The console is a standalone overlay that takes input only while focused.
	if (const auto console = g_client.AddPage(
			{
				.id = "console",
				.displayName = "Console",
				.summary = "Command input with history and completion.",
				.kind = DMUI_PAGE_KIND_OVERLAY,
				.iconName = "terminal-window"
			},
			[] {
				DrawConsolePage(g_client);
			}))
	{
		g_console.page = *console;
		(void)g_client.ConfigureOverlay(g_console.page, {
			.anchor = DMUI_OVERLAY_ANCHOR_FREE,
			.offset = { 32.0f, 32.0f },
			.size = { 720.0f, 320.0f },
			.minimumSize = { 360.0f, 160.0f },
			.opacity = 0.94f,
			.contentScale = 1.0f,
			.backgroundVisible = 1,
			.borderVisible = 1,
			.allowArrangement = 1
		});
		(void)g_client.AddHotkeyAction(
			"toggle_console",
			"Toggle console",
			"Grave",
			[](bool a_pressed) {
				if (a_pressed)
					g_console.open ? CloseConsole() : OpenConsole();
			});
	}

	// Register a quick action in the host command palette or action list.
	(void)g_client.AddAction(
		"reload_presets",
		"Reload Lighting Presets",
		"arrow-clockwise",
		"Reloads INI configurations and color LUTs from disk.",
		[] {
			(void)g_client.PostNotification(
				DMUI_STATUS_SEVERITY_SUCCESS,
				"Lighting presets reloaded from disk.",
				3000);
		});

	// Register a frame observer to sample frametimes while playing.
	(void)g_client.AddFrameObserver(
		[] {
			static auto s_lastTime = std::chrono::steady_clock::now();
			const auto now = std::chrono::steady_clock::now();
			const float deltaMs =
				std::chrono::duration<float, std::milli>(now - s_lastTime).count();
			s_lastTime = now;

			g_frameTimeHistory[g_frameHistoryOffset] = deltaMs;
			g_frameHistoryOffset =
				(g_frameHistoryOffset + 1) % g_frameTimeHistory.size();
		});
}