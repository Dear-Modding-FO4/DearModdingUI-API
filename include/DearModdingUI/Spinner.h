#pragma once

#include <DearModdingUI/API.h>

namespace dmui
{
	// Non-owning; the DMUI_SpinnerDescriptor comment defines each field.
	struct SpinnerOptions
	{
		const char* id{};
		// Diameter in pixels; zero uses the frame height.
		float size{};

		[[nodiscard]] constexpr DMUI_SpinnerDescriptor Abi() const noexcept
		{
			return { id, size, 0u };
		}
	};
}
