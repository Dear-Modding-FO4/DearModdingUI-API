#pragma once

#include <DearModdingUI/API.h>

#include <concepts>
#include <cstdint>
#include <optional>
#include <string>

namespace dmui
{
	enum class TextTone : uint8_t
	{
		kInherit,
		kAccent,
		kAccentMuted,
		kMuted,
		kSuccess,
		kWarning,
		kError,
		kInfo,
		kStatusDisable,
		kStatusError,
		kStatusWarning,
		kStatusRestartNeeded,
		kStatusCurrentHotkey,
		kStatusSuccess,
		kStatusInfo
	};

	[[nodiscard]] constexpr bool IsValidTextTone(
		TextTone a_tone) noexcept
	{
		return static_cast<uint8_t>(a_tone) <=
			static_cast<uint8_t>(TextTone::kStatusInfo);
	}

	struct TextStyle
	{
		std::optional<DMUI_FontRole> fontRole;
		TextTone tone{ TextTone::kInherit };
		bool wrapped{};
	};

	struct TextColorResolution
	{
		DMUI_Result result{ DMUI_RESULT_OK };
		const DMUI_Vec4* color{};

		[[nodiscard]] constexpr explicit operator bool() const noexcept
		{
			return result == DMUI_RESULT_OK;
		}
	};

	[[nodiscard]] constexpr TextColorResolution ResolveTextColor(
		const DMUI_ThemeColors& a_theme,
		TextTone a_tone) noexcept
	{
		if (!IsValidTextTone(a_tone))
			return { DMUI_RESULT_INVALID_ARGUMENT, nullptr };
		if (a_tone == TextTone::kInherit)
			return {};
		if (a_theme.structSize < DMUI_THEME_COLORS_0_1_SIZE)
			return { DMUI_RESULT_STRUCT_TOO_SMALL, nullptr };
		switch (a_tone)
		{
		case TextTone::kAccent:
			return { DMUI_RESULT_OK, &a_theme.accent };
		case TextTone::kAccentMuted:
			return { DMUI_RESULT_OK, &a_theme.accentMuted };
		case TextTone::kMuted:
			return { DMUI_RESULT_OK, &a_theme.muted };
		case TextTone::kSuccess:
			return { DMUI_RESULT_OK, &a_theme.success };
		case TextTone::kWarning:
			return { DMUI_RESULT_OK, &a_theme.warning };
		case TextTone::kError:
			return { DMUI_RESULT_OK, &a_theme.error };
		case TextTone::kInfo:
			return { DMUI_RESULT_OK, &a_theme.info };
		case TextTone::kStatusDisable:
			return { DMUI_RESULT_OK, &a_theme.statusDisable };
		case TextTone::kStatusError:
			return { DMUI_RESULT_OK, &a_theme.statusError };
		case TextTone::kStatusWarning:
			return { DMUI_RESULT_OK, &a_theme.statusWarning };
		case TextTone::kStatusRestartNeeded:
			return { DMUI_RESULT_OK, &a_theme.statusRestartNeeded };
		case TextTone::kStatusCurrentHotkey:
			return { DMUI_RESULT_OK, &a_theme.statusCurrentHotkey };
		case TextTone::kStatusSuccess:
			return { DMUI_RESULT_OK, &a_theme.statusSuccess };
		case TextTone::kStatusInfo:
			return { DMUI_RESULT_OK, &a_theme.statusInfo };
		default:
			return { DMUI_RESULT_INVALID_ARGUMENT, nullptr };
		}
	}

	[[nodiscard]] constexpr const DMUI_Vec4* ThemeToneColor(
		const DMUI_ThemeColors& a_theme,
		TextTone a_tone) noexcept
	{
		const auto resolved = ResolveTextColor(a_theme, a_tone);
		return resolved ? resolved.color : nullptr;
	}

	template <class Value>
	struct ChoiceOption
	{
		Value value;
		std::string label;
		std::string key;
		bool enabled{ true };
	};

	template <class Value>
	struct ChoiceDrawResult
	{
		std::optional<Value> selected;
		bool changed{};
		bool completed{};
	};

	namespace presentation_detail
	{
		template <class Value>
			requires std::equality_comparable<Value> &&
				std::copy_constructible<Value>
		[[nodiscard]] ChoiceDrawResult<Value> ResolveChoiceActivation(
			const Value& a_current,
			const ChoiceOption<Value>& a_option,
			bool a_activated)
		{
			if (!a_activated ||
				!a_option.enabled ||
				a_option.value == a_current)
				return {};
			return {
				a_option.value,
				true,
				true
			};
		}
	}
}
