#pragma once

#include <DearModdingUI/UI.h>

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

	inline DMUI_Result GetThemeColors(DMUI_ThemeColors& a_colors) noexcept
	{
		const auto result = checked::GetThemeColors(&a_colors);
		detail::Record(result);
		return result;
	}

	[[nodiscard]] inline DMUI_ThemeColors GetThemeColors() noexcept
	{
		DMUI_ThemeColors colors{};
		(void)GetThemeColors(colors);
		return colors;
	}

	[[nodiscard]] inline Vec4 GetThemeColor(Vec4 DMUI_ThemeColors::*a_role) noexcept
	{
		if (!a_role)
		{
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return {};
		}
		return GetThemeColors().*a_role;
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

	[[nodiscard]] inline Color32 GetColorU32(
		Vec4 DMUI_ThemeColors::*a_role, float a_alphaMultiplier = 1.0f) noexcept
	{
		return GetColorU32(GetThemeColor(a_role), a_alphaMultiplier);
	}
}
