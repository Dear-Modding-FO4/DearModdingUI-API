#pragma once

#include <DearModdingUI/UI.h>

#include <optional>

namespace dmui::ui
{
	[[nodiscard]] constexpr Color32 ColorConvertFloat4ToU32(Vec4 a_color) noexcept
	{
		const auto byte = [](float a_value) constexpr -> Color32 {
			return !(a_value > 0.0f) ? 0u :
				a_value >= 1.0f ? 255u : static_cast<Color32>(a_value * 255.0f + 0.5f);
		};
		return (byte(a_color.x) << 24u) | (byte(a_color.y) << 16u) |
			(byte(a_color.z) << 8u) | byte(a_color.w);
	}

	// Empty on failure; the result is also recorded for ui::LastResult.
	[[nodiscard]] inline std::optional<DMUI_ThemeTokens> GetThemeTokens() noexcept
	{
		DMUI_ThemeTokens tokens{};
		const auto result = checked::GetThemeTokens(&tokens);
		detail::Record(result);
		if (result != DMUI_RESULT_OK)
			return std::nullopt;
		return tokens;
	}

	[[nodiscard]] inline Color32 GetColorU32(Vec4 a_color, float a_alphaMultiplier = 1.0f) noexcept
	{
		DMUI_StyleMetrics metrics{};
		(void)GetStyleMetrics(metrics);
		a_color.w *= metrics.alpha * a_alphaMultiplier;
		return ColorConvertFloat4ToU32(a_color);
	}

	[[nodiscard]] inline Color32 GetColorU32(Color a_color, float a_alphaMultiplier = 1.0f) noexcept
	{
		return GetColorU32(GetStyleColor(a_color), a_alphaMultiplier);
	}

	// Transparent when the tokens are unavailable; see ui::LastResult.
	[[nodiscard]] inline Color32 GetColorU32(
		Vec4 DMUI_ThemeTokens::*a_token, float a_alphaMultiplier = 1.0f) noexcept
	{
		if (!a_token)
		{
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return {};
		}
		const auto tokens = GetThemeTokens();
		return tokens ?
			GetColorU32(tokens.value().*a_token, a_alphaMultiplier) :
			Color32{};
	}
}
