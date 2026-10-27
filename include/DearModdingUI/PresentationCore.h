#pragma once

#include <DearModdingUI/API.h>
#include <DearModdingUI/StyledText.h>

#include <concepts>
#include <cstdint>
#include <optional>
#include <string>

namespace dmui
{
	template <class Value>
	struct ChoiceOption
	{
		Value value;
		std::string label;
		std::string key;
		bool enabled{ true };
		// Shown as the option's tooltip.
		std::string description;
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
