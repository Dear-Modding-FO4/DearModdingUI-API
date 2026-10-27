#pragma once

#include <DearModdingUI/Presentation/Layout.h>
#include <DearModdingUI/PresentationCore.h>
#include <DearModdingUI/UI.h>

#include <concepts>
#include <cstddef>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

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

	// Attaches rich tooltip content to the last item under the host tooltip policy.
	class TooltipScope
	{
	public:
		TooltipScope() noexcept :
			m_visible(ui::BeginItemTooltip())
		{}

		~TooltipScope() noexcept
		{
			(void)End();
		}

		TooltipScope(const TooltipScope&) = delete;
		TooltipScope(TooltipScope&&) = delete;
		TooltipScope& operator=(const TooltipScope&) = delete;
		TooltipScope& operator=(TooltipScope&&) = delete;

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
		bool m_visible{};
	};
}

namespace dmui::ui
{
	// Typed form of the one dropdown: maps the current value to an index and the
	// chosen index back to its value. An unmatched value shows unmatchedLabel and
	// is never altered; an empty list disables the control.
	template <class Value>
		requires std::equality_comparable<Value> &&
			std::copy_constructible<Value>
	[[nodiscard]] ChoiceDrawResult<Value> Choice(
		std::string_view a_id,
		const Value& a_current,
		std::span<const ChoiceOption<std::type_identity_t<Value>>> a_options,
		const char* a_unmatchedLabel = "Unavailable",
		float a_width = 0.0f)
	{
		std::vector<ChoiceItem> items;
		items.reserve(a_options.size());
		int32_t selected{ -1 };
		for (size_t index = 0; index < a_options.size(); ++index)
		{
			const auto& option = a_options[index];
			if (selected < 0 && option.value == a_current)
				selected = static_cast<int32_t>(index);
			// A blank label falls back to the value or key so rows never vanish.
			const std::string* label = &option.label;
			if (label->empty())
			{
				if constexpr (std::same_as<Value, std::string>)
					label = &option.value;
				else
					label = &option.key;
			}
			items.emplace_back(
				label->c_str(),
				option.description.c_str(),
				option.enabled);
		}
		const std::string id{ a_id };
		const auto picked = Choice(ChoiceOptions{
			.id = id.c_str(),
			.items = items,
			.selected = selected,
			.unmatchedLabel = a_unmatchedLabel,
			.width = a_width
		});
		ChoiceDrawResult<Value> result;
		if (picked.changed && picked.selected >= 0 &&
			static_cast<size_t>(picked.selected) < a_options.size())
		{
			result = presentation_detail::ResolveChoiceActivation(
				a_current,
				a_options[static_cast<size_t>(picked.selected)],
				true);
		}
		return result;
	}
}
