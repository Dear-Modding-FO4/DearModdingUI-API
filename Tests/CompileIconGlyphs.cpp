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
