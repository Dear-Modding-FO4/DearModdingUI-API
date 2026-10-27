#pragma once

#include <DearModdingUI/API.h>

#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>

namespace dmui
{
	// One swatch of ui::ColorEdit; the strings are borrowed for the call.
	struct ColorPreset : DMUI_ColorPreset
	{
		constexpr ColorPreset(
			const char* a_name,
			uint32_t a_rgba,
			const char* a_description = nullptr) noexcept :
			DMUI_ColorPreset{ a_name, a_description, a_rgba, 0u }
		{}
	};

	static_assert(sizeof(ColorPreset) == sizeof(DMUI_ColorPreset));
	static_assert(std::is_standard_layout_v<ColorPreset>);

	// Non-owning; the DMUI_ColorEditDescriptor comment defines each field.
	struct ColorEditOptions
	{
		const char* id{};
		// Packed 0xRRGGBBAA.
		uint32_t rgba{ 0x000000FFu };
		// Edits the alpha channel too.
		bool alpha{};
		std::span<const ColorPreset> presets;
		const char* presetsLabel{};
		float width{};

		[[nodiscard]] constexpr DMUI_ColorEditDescriptor Abi() const noexcept
		{
			return {
				id,
				// The ABI reads presets by DMUI_ColorPreset stride; the sizes above guarantee it.
				static_cast<const DMUI_ColorPreset*>(presets.data()),
				presetsLabel,
				static_cast<uint32_t>(presets.size()),
				rgba,
				alpha ? DMUI_COLOR_EDIT_ALPHA : 0u,
				0u,
				width
			};
		}
	};

	struct ColorEditResult
	{
		uint32_t rgba{};
		bool changed{};
		bool completed{};
	};
}
