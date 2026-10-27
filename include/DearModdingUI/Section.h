#pragma once

#include <DearModdingUI/API.h>
#include <DearModdingUI/IconGlyphs.h>

#include <cstdint>
#include <optional>

namespace dmui
{
	static_assert(PhosphorGlyph::kNone == DMUI_GLYPH_NONE);

	// Non-owning; the DMUI_SectionDescriptor comment defines each field.
	struct SectionOptions
	{
		const char* id{};
		const char* title{};
		// Zero infers an icon from the title; PhosphorGlyph::kNone draws none.
		char32_t glyph{};
		// Holds the content in a themed panel.
		bool box{};
		// Caller-owned state; non-null makes the section collapsible.
		bool* expanded{};
		std::optional<uint32_t> count;

		[[nodiscard]] constexpr DMUI_SectionDescriptor Abi(
			uint32_t* a_expanded) const noexcept
		{
			return {
				id,
				title,
				a_expanded,
				static_cast<uint32_t>(glyph),
				(box ? DMUI_SECTION_BOX : 0u) |
					(count ? DMUI_SECTION_HAS_COUNT : 0u),
				count.value_or(0u),
				0u
			};
		}
	};
}
