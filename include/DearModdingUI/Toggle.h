#pragma once

#include <DearModdingUI/API.h>

#include <cstdint>

namespace dmui
{
	// Non-owning; the DMUI_ToggleDescriptor comment defines each field.
	struct ToggleOptions
	{
		const char* id{};
		const char* label{};
		bool value{};

		[[nodiscard]] constexpr DMUI_ToggleDescriptor Abi() const noexcept
		{
			return { id, label, value ? 1u : 0u, 0u };
		}
	};

	struct ToggleResult
	{
		bool value{};
		bool changed{};
	};
}
