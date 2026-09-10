#include <DearModdingUI/IconGlyphs.h>

static_assert(DearModdingUI::IsValidUnicodeScalar(U'\uE472'));
static_assert(!DearModdingUI::IsValidUnicodeScalar(char32_t{}));
static_assert(!DearModdingUI::IsValidUnicodeScalar(char32_t{ 0xD800 }));
static_assert(!DearModdingUI::IsValidUnicodeScalar(char32_t{ 0x110000 }));
static_assert(
	DearModdingUI::IsRepresentableIconGlyph<uint16_t>(U'\uE472'));
static_assert(
	!DearModdingUI::IsRepresentableIconGlyph<uint16_t>(
		char32_t{ 0x1E472 }));

namespace
{
	[[maybe_unused]] const bool kIconResolverCompiles = [] {
		using namespace DearModdingUI;
		const std::array primary{ std::string_view{ "Wrench Mod" } };
		const std::array secondary{ std::string_view{ "General" } };
		const auto selection = IconResolver::Resolve({
			.explicitName = {},
			.primaryMetadata = primary,
			.secondaryMetadata = secondary
		});
		return selection.HasSelection() &&
			selection.glyph == FindPhosphorIconGlyphOrZero("wrench") &&
			NormalizeIconName("DearModdingUI") == "dear-modding-ui";
	}();
}
