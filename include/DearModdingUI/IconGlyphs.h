#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

namespace DearModdingUI
{
	enum class IconKind : uint32_t
	{
		kCategory,
		kClient
	};

	namespace PhosphorGlyph
	{
		inline constexpr char32_t kFirstPrivateUse{ 0xE000 };
		inline constexpr char32_t kLastPrivateUse{ 0xEE82 };
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

	struct IconGlyphMapping
	{
		std::string_view slug;
		char32_t glyph;
	};
}

#include "PhosphorGlyphs.generated.h"

namespace DearModdingUI
{
	struct IconConceptMapping
	{
		std::string_view slug;
		std::string_view icon;
	};

	struct IconConceptMatch
	{
		std::string_view slug;
		char32_t glyph{};

		[[nodiscard]] explicit operator bool() const noexcept
		{
			return glyph != char32_t{};
		}
	};

	inline constexpr std::array kIconConcepts{
		IconConceptMapping{ "ai", "brain" },
		IconConceptMapping{ "armor", "shield" },
		IconConceptMapping{ "audio", "speaker-high" },
		IconConceptMapping{ "building", "buildings" },
		IconConceptMapping{ "camera", "camera" },
		IconConceptMapping{ "combat", "sword" },
		IconConceptMapping{ "compatibility", "puzzle-piece" },
		IconConceptMapping{ "controls", "sliders-horizontal" },
		IconConceptMapping{ "crafting", "hammer" },
		IconConceptMapping{ "debug", "bug" },
		IconConceptMapping{ "dev-tools", "terminal-window" },
		IconConceptMapping{ "diagnostics", "stethoscope" },
		IconConceptMapping{ "dialogue", "chat-circle-text" },
		IconConceptMapping{ "difficulty", "gauge" },
		IconConceptMapping{ "economy", "coins" },
		IconConceptMapping{ "gameplay", "game-controller" },
		IconConceptMapping{ "general", "gear" },
		IconConceptMapping{ "graphics", "image" },
		IconConceptMapping{ "hud", "crosshair-simple" },
		IconConceptMapping{ "input", "keyboard" },
		IconConceptMapping{ "interface", "monitor" },
		IconConceptMapping{ "inventory", "backpack" },
		IconConceptMapping{ "leveling", "trend-up" },
		IconConceptMapping{ "lighting", "lightbulb" },
		IconConceptMapping{ "logging", "file-text" },
		IconConceptMapping{ "map", "map-trifold" },
		IconConceptMapping{ "memory", "memory" },
		IconConceptMapping{ "misc", "dots-three-circle" },
		IconConceptMapping{ "network", "wifi-high" },
		IconConceptMapping{ "npc", "user" },
		IconConceptMapping{ "other", "dots-three-circle" },
		IconConceptMapping{ "overlay", "app-window" },
		IconConceptMapping{ "performance", "speedometer" },
		IconConceptMapping{ "perks", "medal" },
		IconConceptMapping{ "physics", "atom" },
		IconConceptMapping{ "post-process", "magic-wand" },
		IconConceptMapping{ "power-armor", "robot" },
		IconConceptMapping{ "quest", "scroll" },
		IconConceptMapping{ "radio", "radio" },
		IconConceptMapping{ "save", "floppy-disk" },
		IconConceptMapping{ "settlement", "house" },
		IconConceptMapping{ "skills", "student" },
		IconConceptMapping{ "stability", "shield-check" },
		IconConceptMapping{ "stealth", "eye-slash" },
		IconConceptMapping{ "survival", "campfire" },
		IconConceptMapping{ "ui", "layout" },
		IconConceptMapping{ "unloaded", "archive" },
		IconConceptMapping{ "vats", "crosshair" },
		IconConceptMapping{ "video", "video-camera" },
		IconConceptMapping{ "visuals", "palette" },
		IconConceptMapping{ "weapons", "crosshair" },
		IconConceptMapping{ "weather", "cloud-sun" }
	};

	[[nodiscard]] inline std::string SlugifyIconName(std::string_view a_name)
	{
		std::string slug;
		slug.reserve(a_name.size());
		bool separatorPending = false;
		bool previousLowerOrDigit = false;
		bool previousUpper = false;
		for (size_t index = 0; index < a_name.size(); ++index)
		{
			const auto character = static_cast<unsigned char>(a_name[index]);
			const auto next = index + 1 < a_name.size() ?
				static_cast<unsigned char>(a_name[index + 1]) :
				static_cast<unsigned char>(0);
			const auto nextLower = next >= 'a' && next <= 'z';
			if ((character >= 'a' && character <= 'z') ||
				(character >= '0' && character <= '9'))
			{
				if (separatorPending && !slug.empty())
					slug.push_back('-');
				slug.push_back(static_cast<char>(character));
				separatorPending = false;
				previousLowerOrDigit = true;
				previousUpper = false;
			}
			else if (character >= 'A' && character <= 'Z')
			{
				if ((separatorPending || previousLowerOrDigit ||
						(previousUpper && nextLower)) &&
					!slug.empty() && slug.back() != '-')
					slug.push_back('-');
				slug.push_back(static_cast<char>(character - 'A' + 'a'));
				separatorPending = false;
				previousLowerOrDigit = false;
				previousUpper = true;
			}
			else
			{
				separatorPending = !slug.empty();
				previousLowerOrDigit = false;
				previousUpper = false;
			}
		}
		return slug;
	}

	[[nodiscard]] inline std::string NormalizeIconOwnerName(std::string_view a_name)
	{
		std::string normalized;
		normalized.reserve(a_name.size());
		for (const auto value : a_name)
		{
			const auto character = static_cast<unsigned char>(value);
			if ((character >= 'a' && character <= 'z') ||
				(character >= '0' && character <= '9'))
			{
				normalized.push_back(static_cast<char>(character));
			}
			else if (character >= 'A' && character <= 'Z')
			{
				normalized.push_back(static_cast<char>(character - 'A' + 'a'));
			}
		}
		return normalized;
	}

	[[nodiscard]] inline char32_t FindPhosphorSlugGlyphOrZero(
		std::string_view a_slug) noexcept
	{
		size_t first{};
		size_t last = kPhosphorIconGlyphs.size();
		while (first < last)
		{
			const auto middle = first + (last - first) / 2;
			if (kPhosphorIconGlyphs[middle].slug < a_slug)
				first = middle + 1;
			else
				last = middle;
		}
		if (first < kPhosphorIconGlyphs.size() &&
			kPhosphorIconGlyphs[first].slug == a_slug)
			return kPhosphorIconGlyphs[first].glyph;
		return {};
	}

	[[nodiscard]] inline char32_t FindPhosphorIconGlyphOrZero(
		std::string_view a_name)
	{
		return FindPhosphorSlugGlyphOrZero(SlugifyIconName(a_name));
	}

	[[nodiscard]] inline bool PreferIconConceptMatch(
		const IconConceptMatch& a_candidate,
		const IconConceptMatch& a_current) noexcept
	{
		if (!a_candidate)
			return false;
		if (!a_current)
			return true;
		if (a_candidate.slug.size() != a_current.slug.size())
			return a_candidate.slug.size() > a_current.slug.size();
		return a_candidate.slug < a_current.slug;
	}

	[[nodiscard]] inline bool ContainsWholeIconConcept(
		std::string_view a_name,
		std::string_view a_conceptSlug) noexcept
	{
		size_t position{};
		while ((position = a_name.find(a_conceptSlug, position)) !=
			std::string_view::npos)
		{
			const auto startsOnBoundary =
				position == 0 || a_name[position - 1] == '-';
			const auto end = position + a_conceptSlug.size();
			const auto endsOnBoundary =
				end == a_name.size() || a_name[end] == '-';
			if (startsOnBoundary && endsOnBoundary)
				return true;
			++position;
		}
		return false;
	}

	[[nodiscard]] inline IconConceptMatch FindIconConceptMatch(
		std::string_view a_name,
		bool a_wholeWords)
	{
		const auto slug = SlugifyIconName(a_name);
		IconConceptMatch best;
		for (const auto& mapping : kIconConcepts)
		{
			const auto matches = a_wholeWords ?
				ContainsWholeIconConcept(slug, mapping.slug) :
				slug == mapping.slug;
			if (!matches)
				continue;
			const IconConceptMatch candidate{
				mapping.slug,
				FindPhosphorSlugGlyphOrZero(mapping.icon)
			};
			if (PreferIconConceptMatch(candidate, best))
				best = candidate;
		}
		return best;
	}

	[[nodiscard]] inline char32_t ResolveNamedIconGlyphOrZero(
		std::string_view a_name)
	{
		if (const auto glyph = FindPhosphorIconGlyphOrZero(a_name))
			return glyph;
		return FindIconConceptMatch(a_name, false).glyph;
	}

	[[nodiscard]] inline char32_t ResolveIconGlyph(
		IconKind,
		std::string_view a_name) noexcept
	{
		try
		{
			if (const auto glyph = ResolveNamedIconGlyphOrZero(a_name))
				return glyph;
			return PhosphorGlyph::kQuestion;
		}
		catch (...)
		{
			return PhosphorGlyph::kQuestion;
		}
	}

	[[nodiscard]] inline char32_t ResolveClientIconGlyph(
		std::string_view a_iconName,
		std::string_view a_category,
		std::string_view a_displayName) noexcept
	{
		try
		{
			if (const auto glyph = ResolveNamedIconGlyphOrZero(a_iconName))
				return glyph;
			if (const auto glyph = ResolveNamedIconGlyphOrZero(a_category))
				return glyph;
			if (const auto match = FindIconConceptMatch(a_displayName, true))
				return match.glyph;
			return PhosphorGlyph::kQuestion;
		}
		catch (...)
		{
			return PhosphorGlyph::kQuestion;
		}
	}

	[[nodiscard]] inline char32_t ResolveIconGlyph(
		IconKind a_kind,
		std::string_view a_iconName,
		std::string_view a_fallbackName) noexcept
	{
		if (a_kind == IconKind::kClient)
			return ResolveClientIconGlyph(a_iconName, {}, a_fallbackName);
		return ResolveIconGlyph(a_kind, a_iconName);
	}

	[[nodiscard]] inline char32_t ResolveCategoryIconGlyph(
		std::string_view a_category,
		std::string_view a_clientDisplayName,
		std::string_view a_clientId,
		std::string_view a_clientIconName = {}) noexcept
	{
		try
		{
			const auto category = NormalizeIconOwnerName(a_category);
			if (!category.empty() &&
				(category == NormalizeIconOwnerName(a_clientDisplayName) ||
					category == NormalizeIconOwnerName(a_clientId)))
				return ResolveClientIconGlyph(
					a_clientIconName,
					a_category,
					a_clientDisplayName);
			return ResolveIconGlyph(IconKind::kCategory, a_category);
		}
		catch (...)
		{
			return PhosphorGlyph::kQuestion;
		}
	}

	[[nodiscard]] inline char32_t ResolveActionIconGlyph(
		std::string_view a_name) noexcept
	{
		try
		{
			return ResolveNamedIconGlyphOrZero(a_name);
		}
		catch (...)
		{
			return {};
		}
	}
}
