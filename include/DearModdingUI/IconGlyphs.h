#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

namespace dmui
{
	namespace PhosphorGlyph
	{
		inline constexpr char32_t kFirstPrivateUse{ 0xE000 };
		inline constexpr char32_t kLastPrivateUse{ 0xEE82 };
		// Matches DMUI_GLYPH_NONE: an explicit request for no icon.
		inline constexpr char32_t kNone{ 0xFFFFFFFFu };
		inline constexpr char32_t kQuestion{ 0xE3E8 };
		inline constexpr char32_t kArchive{ 0xE00C };
		inline constexpr char32_t kAppWindow{ 0xE5DA };
		inline constexpr char32_t kArrowCounterClockwise{ 0xE038 };
		inline constexpr char32_t kArrowsClockwise{ 0xE094 };
		inline constexpr char32_t kClipboardText{ 0xE198 };
		inline constexpr char32_t kDotsThreeCircle{ 0xE200 };
		inline constexpr char32_t kFloppyDisk{ 0xE248 };
		inline constexpr char32_t kGear{ 0xE270 };
		inline constexpr char32_t kGameController{ 0xE26E };
		inline constexpr char32_t kMagnifyingGlass{ 0xE30C };
		inline constexpr char32_t kMonitor{ 0xE32E };
		inline constexpr char32_t kPalette{ 0xE6C8 };
		inline constexpr char32_t kShieldCheck{ 0xE40C };
		inline constexpr char32_t kSpeakerHigh{ 0xE44A };
		inline constexpr char32_t kSun{ 0xE472 };
		inline constexpr char32_t kTrash{ 0xE4A6 };
		inline constexpr char32_t kX{ 0xE4F6 };
		inline constexpr char32_t kPuzzlePiece{ 0xE596 };
		inline constexpr char32_t kGauge{ 0xE628 };
		inline constexpr char32_t kMagicWand{ 0xE6B6 };
		inline constexpr char32_t kTerminalWindow{ 0xEAE8 };
		inline constexpr char32_t kSquaresFour{ 0xE464 };
		inline constexpr char32_t kFiles{ 0xE710 };
	}

	[[nodiscard]] constexpr bool IsValidUnicodeScalar(
		char32_t a_glyph) noexcept
	{
		return a_glyph != char32_t{} &&
			a_glyph <= char32_t{ 0x10FFFF } &&
			(a_glyph < char32_t{ 0xD800 } ||
				a_glyph > char32_t{ 0xDFFF });
	}

	template <class Character>
	[[nodiscard]] constexpr bool IsRepresentableIconGlyph(
		char32_t a_glyph) noexcept
	{
		using Value = std::remove_cv_t<Character>;
		static_assert(std::is_integral_v<Value>);
		return IsValidUnicodeScalar(a_glyph) &&
			a_glyph <= static_cast<char32_t>(
				(std::numeric_limits<Value>::max)());
	}
}
