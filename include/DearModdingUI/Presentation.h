#pragma once

#include <DearModdingUI/PresentationCore.h>
#include <DearModdingUI/UI.h>

#include <concepts>
#include <cstddef>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>

namespace dmui
{
	class DisabledScope
	{
	public:
		explicit DisabledScope(bool a_disabled = true) noexcept
		{
			ui::BeginDisabled(a_disabled);
		}

		~DisabledScope() noexcept
		{
			ui::EndDisabled();
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
			ui::HoveredFlags a_hoverFlags =
				ui::HoveredFlags::kDelayNormal) noexcept :
			m_hovered(ui::IsItemHovered(a_hoverFlags)),
			m_visible(m_hovered && ui::BeginTooltip())
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
			ui::EndTooltip();
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
				ui::PushTextWrapPos(0.0f);
			ui::TextUnformatted(begin, end);
			if (a_wrapped)
				ui::PopTextWrapPos();
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
			ui::PushStyleColor(
				ui::Color::kText,
				ui::Vec4{
					color.color->x,
					color.color->y,
					color.color->z,
					color.color->w
				});
		presentation_detail::DrawUnformatted(a_text, a_style.wrapped);
		if (color.color)
			ui::PopStyleColor();
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
		ui::PushID(idBegin, idBegin + a_id.size());
		{
			const DisabledScope emptyScope{ a_options.empty() };
			if (ui::BeginCombo("##Choice", preview))
			{
				for (size_t index = 0; index < a_options.size(); ++index)
				{
					const auto& option = a_options[index];
					if (!option.key.empty())
					{
						ui::PushID(
							option.key.data(),
							option.key.data() + option.key.size());
					}
					else if constexpr (std::same_as<Value, std::string>)
					{
						ui::PushID(
							option.value.data(),
							option.value.data() + option.value.size());
					}
					else
						ui::PushID(static_cast<int>(index));
					{
						const DisabledScope disabled{ !option.enabled };
						const auto selected = selectedOption == &option;
						const auto start = ui::GetCursorScreenPos();
						const auto pressed = ui::Selectable(
							"##Option",
							selected);
						if (selected)
							ui::SetItemDefaultFocus();
						const auto after = ui::GetCursorScreenPos();
						ui::SetCursorScreenPos(start);
						const auto label = optionLabel(option);
						presentation_detail::DrawUnformatted(label, false);
						ui::SetCursorScreenPos(after);
						if (pressed && option.enabled && !selected)
							activatedOption = &option;
					}
					ui::PopID();
				}
				// Commit the restored cursor before ending the popup.
				ui::Dummy({ 0.0f, 0.0f });
				ui::EndCombo();
			}
		}
		if (!a_label.empty())
		{
			ui::SameLine();
			presentation_detail::DrawUnformatted(a_label, false);
		}
		ui::PopID();
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
