#pragma once

#include <DearModdingUI/API.h>

#include <cstdint>

namespace dmui
{
	enum class ButtonStyle : uint32_t
	{
		kDefault = DMUI_BUTTON_STYLE_DEFAULT,
		kPrimary = DMUI_BUTTON_STYLE_PRIMARY,
		kSubtle = DMUI_BUTTON_STYLE_SUBTLE
	};

	// Non-owning; the DMUI_ButtonDescriptor comment defines each field.
	struct ButtonOptions
	{
		const char* id{};
		const char* label{};
		char32_t glyph{};
		const char* tooltip{};
		bool enabled{ true };
		ButtonStyle style{ ButtonStyle::kDefault };
		DMUI_Vec2 size{};

		[[nodiscard]] constexpr DMUI_ButtonDescriptor Abi() const noexcept
		{
			return {
				id,
				label,
				static_cast<uint32_t>(glyph),
				enabled ? 1u : 0u,
				static_cast<DMUI_ButtonStyle>(style),
				0u,
				tooltip,
				size
			};
		}
	};
}
