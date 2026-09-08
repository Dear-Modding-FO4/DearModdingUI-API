#pragma once

#if !defined(IMGUI_VERSION) || !defined(IMGUI_VERSION_NUM)
#include <DearModdingUI/ImGuiForward.h>
#endif

#include <DearModdingUI/API.h>

#include <concepts>
#include <cstddef>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>

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

	class DisabledScope
	{
	public:
		explicit DisabledScope(bool a_disabled = true) noexcept
		{
			ImGui::BeginDisabled(a_disabled);
		}

		~DisabledScope() noexcept
		{
			ImGui::EndDisabled();
		}

		DisabledScope(const DisabledScope&) = delete;
		DisabledScope(DisabledScope&&) = delete;
		DisabledScope& operator=(const DisabledScope&) = delete;
		DisabledScope& operator=(DisabledScope&&) = delete;
	};

	class TooltipScope
	{
	public:
		explicit TooltipScope(
			ImGuiHoveredFlags a_hoverFlags =
				ImGuiHoveredFlags_DelayNormal) noexcept :
			m_hovered(ImGui::IsItemHovered(a_hoverFlags)),
			m_visible(m_hovered && ImGui::BeginTooltip())
		{}

		~TooltipScope() noexcept
		{
			(void)End();
		}

		TooltipScope(const TooltipScope&) = delete;
		TooltipScope(TooltipScope&&) = delete;
		TooltipScope& operator=(const TooltipScope&) = delete;
		TooltipScope& operator=(TooltipScope&&) = delete;

		[[nodiscard]] bool Hovered() const noexcept
		{
			return m_hovered;
		}

		[[nodiscard]] bool Visible() const noexcept
		{
			return m_visible;
		}

		[[nodiscard]] bool End() noexcept
		{
			if (!m_visible)
				return false;
			m_visible = false;
			ImGui::EndTooltip();
			return true;
		}

	private:
		bool m_hovered{};
		bool m_visible{};
	};

	namespace presentation_detail
	{
		[[nodiscard]] inline const char* TextBegin(
			std::string_view a_text) noexcept
		{
			static constexpr char empty[]{ "" };
			return a_text.empty() ? empty : a_text.data();
		}

		inline void DrawUnformatted(
			std::string_view a_text,
			bool a_wrapped) noexcept
		{
			const auto* begin = TextBegin(a_text);
			const auto* end = begin + a_text.size();
			if (a_wrapped)
				ImGui::PushTextWrapPos(0.0f);
			ImGui::TextUnformatted(begin, end);
			if (a_wrapped)
				ImGui::PopTextWrapPos();
		}

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

	[[nodiscard]] inline DMUI_Result DrawStyledText(
		std::string_view a_text,
		const DMUI_ThemeColors& a_theme,
		TextStyle a_style = {}) noexcept
	{
		if (a_style.fontRole)
			return DMUI_RESULT_INVALID_ARGUMENT;
		const auto color = ResolveTextColor(a_theme, a_style.tone);
		if (!color)
			return color.result;
		if (color.color)
			ImGui::PushStyleColor(
				ImGuiCol_Text,
				ImVec4{
					color.color->x,
					color.color->y,
					color.color->z,
					color.color->w
				});
		presentation_detail::DrawUnformatted(a_text, a_style.wrapped);
		if (color.color)
			ImGui::PopStyleColor();
		return DMUI_RESULT_OK;
	}

	template <class Value>
		requires std::equality_comparable<Value> &&
			std::copy_constructible<Value>
	[[nodiscard]] ChoiceDrawResult<Value> DrawChoice(
		std::string_view a_id,
		const Value& a_current,
		std::span<const ChoiceOption<std::type_identity_t<Value>>> a_options,
		const char* a_unavailablePreview = "Unavailable",
		std::string_view a_label = {})
	{
		ChoiceDrawResult<Value> result;
		const ChoiceOption<Value>* selectedOption{};
		const ChoiceOption<Value>* activatedOption{};
		for (const auto& option : a_options)
		{
			if (option.value == a_current)
			{
				selectedOption = &option;
				break;
			}
		}
		const auto optionLabel = [](const ChoiceOption<Value>& a_option)
			-> std::string_view {
			if (!a_option.label.empty())
				return a_option.label;
			if constexpr (std::same_as<Value, std::string>)
				return a_option.value;
			return a_option.key;
		};
		const auto previewLabel = selectedOption ?
			optionLabel(*selectedOption) :
			std::string_view{};
		const auto* preview = selectedOption && !previewLabel.empty() ?
			previewLabel.data() :
			a_unavailablePreview;
		if (!preview)
			preview = "";

		const auto* idBegin = presentation_detail::TextBegin(a_id);
		ImGui::PushID(idBegin, idBegin + a_id.size());
		{
			const DisabledScope emptyScope{ a_options.empty() };
			if (ImGui::BeginCombo("##Choice", preview))
			{
				for (size_t index = 0; index < a_options.size(); ++index)
				{
					const auto& option = a_options[index];
					if (!option.key.empty())
					{
						ImGui::PushID(
							option.key.data(),
							option.key.data() + option.key.size());
					}
					else if constexpr (std::same_as<Value, std::string>)
					{
						ImGui::PushID(
							option.value.data(),
							option.value.data() + option.value.size());
					}
					else
						ImGui::PushID(static_cast<int>(index));
					{
						const DisabledScope disabled{ !option.enabled };
						const auto selected = selectedOption == &option;
						const auto start = ImGui::GetCursorScreenPos();
						const auto pressed = ImGui::Selectable(
							"##Option",
							selected);
						if (selected)
							ImGui::SetItemDefaultFocus();
						const auto after = ImGui::GetCursorScreenPos();
						ImGui::SetCursorScreenPos(start);
						const auto label = optionLabel(option);
						presentation_detail::DrawUnformatted(label, false);
						ImGui::SetCursorScreenPos(after);
						if (pressed && option.enabled && !selected)
							activatedOption = &option;
					}
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}
		}
		if (!a_label.empty())
		{
			ImGui::SameLine();
			presentation_detail::DrawUnformatted(a_label, false);
		}
		ImGui::PopID();
		if (activatedOption)
		{
			result = presentation_detail::ResolveChoiceActivation(
				a_current,
				*activatedOption,
				true);
		}
		return result;
	}
}
