#pragma once

#include <DearModdingUI/API.h>

#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>

namespace dmui
{
	// One option of ui::Choice; the strings are borrowed for the call.
	struct ChoiceItem : DMUI_ChoiceOption
	{
		constexpr ChoiceItem(
			const char* a_label,
			const char* a_description = nullptr,
			bool a_enabled = true) noexcept :
			DMUI_ChoiceOption{ a_label, a_description, a_enabled ? 1u : 0u, 0u }
		{}
	};

	static_assert(sizeof(ChoiceItem) == sizeof(DMUI_ChoiceOption));
	static_assert(std::is_standard_layout_v<ChoiceItem>);

	// Non-owning; the DMUI_ChoiceDescriptor comment defines each field.
	struct ChoiceOptions
	{
		const char* id{};
		std::span<const ChoiceItem> items;
		// An index into items; any other value is unmatched.
		int32_t selected{ -1 };
		const char* unmatchedLabel{};
		float width{};

		[[nodiscard]] constexpr DMUI_ChoiceDescriptor Abi() const noexcept
		{
			return {
				id,
				// The ABI reads items by DMUI_ChoiceOption stride; the sizes above guarantee it.
				static_cast<const DMUI_ChoiceOption*>(items.data()),
				unmatchedLabel,
				static_cast<uint32_t>(items.size()),
				selected,
				width,
				0u
			};
		}
	};

	struct ChoiceResult
	{
		int32_t selected{ -1 };
		bool changed{};
		bool completed{};
	};
}
