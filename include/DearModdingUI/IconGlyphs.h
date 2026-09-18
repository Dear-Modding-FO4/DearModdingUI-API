#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>

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

	struct IconPhraseMapping
	{
		std::string_view phrase;
		char32_t glyph;
	};
}

#include "PhosphorGlyphs.generated.h"

namespace DearModdingUI
{
	enum class IconSelectionStatus : uint32_t
	{
		kNoMatch,
		kSelected,
		kInvalidRawGlyph
	};

	struct IconSelection
	{
		IconSelectionStatus status{ IconSelectionStatus::kNoMatch };
		char32_t glyph{};

		[[nodiscard]] constexpr bool HasSelection() const noexcept
		{
			return status == IconSelectionStatus::kSelected;
		}

		[[nodiscard]] constexpr char32_t GlyphOr(
			char32_t a_fallback) const noexcept
		{
			return status == IconSelectionStatus::kSelected ||
					status == IconSelectionStatus::kInvalidRawGlyph ?
				glyph :
				a_fallback;
		}
	};

	struct IconResolutionRequest
	{
		std::optional<char32_t> explicitGlyph;
		std::string_view explicitName;
		std::span<const std::string_view> primaryMetadata;
		std::span<const std::string_view> secondaryMetadata;
	};

	[[nodiscard]] inline std::string NormalizeIconName(std::string_view a_name)
	{
		std::string normalized;
		normalized.reserve(a_name.size());
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
				if (separatorPending && !normalized.empty())
					normalized.push_back('-');
				normalized.push_back(static_cast<char>(character));
				separatorPending = false;
				previousLowerOrDigit = true;
				previousUpper = false;
			}
			else if (character >= 'A' && character <= 'Z')
			{
				if ((separatorPending || previousLowerOrDigit ||
						(previousUpper && nextLower)) &&
					!normalized.empty() && normalized.back() != '-')
					normalized.push_back('-');
				normalized.push_back(
					static_cast<char>(character - 'A' + 'a'));
				separatorPending = false;
				previousLowerOrDigit = false;
				previousUpper = true;
			}
			else
			{
				separatorPending = !normalized.empty();
				previousLowerOrDigit = false;
				previousUpper = false;
			}
		}
		return normalized;
	}

	[[nodiscard]] inline std::string SlugifyIconName(std::string_view a_name)
	{
		return NormalizeIconName(a_name);
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

	namespace IconResolverDetail
	{
		struct MatchRank
		{
			uint32_t tier{};
			size_t wordCount{};
			uint32_t authority{};

			[[nodiscard]] friend constexpr bool operator<(
				const MatchRank& a_left,
				const MatchRank& a_right) noexcept
			{
				if (a_left.tier != a_right.tier)
					return a_left.tier < a_right.tier;
				if (a_left.wordCount != a_right.wordCount)
					return a_left.wordCount < a_right.wordCount;
				return a_left.authority < a_right.authority;
			}
		};

		struct BestMatch
		{
			MatchRank rank;
			char32_t glyph{};

			void Add(char32_t a_glyph, MatchRank a_rank)
			{
				if (!a_glyph)
					return;
				if (!glyph || rank < a_rank)
				{
					rank = a_rank;
					glyph = a_glyph;
				}
				else if (!(a_rank < rank) && a_glyph < glyph)
					glyph = a_glyph;
			}
		};

		template <size_t Size>
		[[nodiscard]] inline std::pair<size_t, size_t> EqualRange(
			const std::array<IconPhraseMapping, Size>& a_mappings,
			std::string_view a_phrase) noexcept
		{
			const auto first = std::ranges::lower_bound(
				a_mappings,
				a_phrase,
				{},
				&IconPhraseMapping::phrase);
			const auto last = std::ranges::upper_bound(
				first,
				a_mappings.end(),
				a_phrase,
				{},
				&IconPhraseMapping::phrase);
			return {
				static_cast<size_t>(first - a_mappings.begin()),
				static_cast<size_t>(last - a_mappings.begin())
			};
		}

		template <size_t Size>
		void AddMatches(
			BestMatch& a_match,
			const std::array<IconPhraseMapping, Size>& a_mappings,
			std::string_view a_phrase,
			MatchRank a_rank)
		{
			const auto [first, last] = EqualRange(a_mappings, a_phrase);
			for (auto index = first; index < last; ++index)
				a_match.Add(a_mappings[index].glyph, a_rank);
		}

		[[nodiscard]] inline size_t WordCount(
			std::string_view a_phrase) noexcept
		{
			return a_phrase.empty() ?
				0 :
				static_cast<size_t>(
					std::ranges::count(a_phrase, '-')) +
					1;
		}

		[[nodiscard]] inline bool HasMetadataTerm(
			std::string_view a_term) noexcept
		{
			for (const auto& mappings : {
					std::span<const IconPhraseMapping>{ kPhosphorIconGlyphs },
					std::span<const IconPhraseMapping>{ kPhosphorIconAliases },
					std::span<const IconPhraseMapping>{
						kPhosphorIconDomainTerms },
					std::span<const IconPhraseMapping>{ kPhosphorIconTags } })
			{
				const auto first = std::ranges::lower_bound(
					mappings,
					a_term,
					{},
					&IconPhraseMapping::phrase);
				if (first != mappings.end() && first->phrase == a_term)
					return true;
			}
			return false;
		}

		[[nodiscard]] inline std::string SingularMetadataWord(
			std::string_view a_word)
		{
			if (a_word.size() <= 3 ||
				a_word.ends_with("ss") ||
				a_word.ends_with("us") ||
				a_word.ends_with("is"))
				return {};

			std::string candidate;
			candidate.reserve(a_word.size());
			const auto tryCandidate =
				[&](size_t a_removed, char a_replacement = '\0') {
				candidate.assign(
					a_word.substr(0, a_word.size() - a_removed));
				if (a_replacement)
					candidate.push_back(a_replacement);
				return HasMetadataTerm(candidate);
			};
			if (a_word.ends_with("ies") && tryCandidate(3, 'y'))
				return candidate;
			if ((a_word.ends_with("sses") ||
				a_word.ends_with("zzes") ||
				a_word.ends_with("ches") ||
				a_word.ends_with("shes") ||
				a_word.ends_with("xes")) &&
				tryCandidate(2))
				return candidate;
			if (a_word.ends_with('s') && tryCandidate(1))
				return candidate;
			if (a_word.ends_with("es") && tryCandidate(2))
				return candidate;
			return {};
		}

		[[nodiscard]] inline std::string SingularizeMetadataWords(
			std::string_view a_normalized)
		{
			std::string singular;
			singular.reserve(a_normalized.size());
			bool changed = false;
			for (size_t begin = 0; begin < a_normalized.size();)
			{
				const auto separator = a_normalized.find('-', begin);
				const auto end = separator == std::string::npos ?
					a_normalized.size() :
					separator;
				const auto word = a_normalized.substr(begin, end - begin);
				auto wordForm = SingularMetadataWord(word);
				if (wordForm.empty())
					singular.append(word);
				else
				{
					singular.append(wordForm);
					changed = true;
				}
				if (separator == std::string::npos)
					break;
				singular.push_back('-');
				begin = separator + 1;
			}
			return changed ? singular : std::string{};
		}

		inline void AddNormalizedMetadataMatches(
			BestMatch& a_match,
			std::string_view a_normalized)
		{
			if (a_normalized.empty())
				return;

			const auto fullWordCount = WordCount(a_normalized);
			AddMatches(
				a_match,
				kPhosphorIconGlyphs,
				a_normalized,
				{ 3, fullWordCount, 2 });
			AddMatches(
				a_match,
				kPhosphorIconAliases,
				a_normalized,
				{ 3, fullWordCount, 2 });
			AddMatches(
				a_match,
				kPhosphorIconDomainTerms,
				a_normalized,
				{ 3, fullWordCount, 1 });

			for (size_t begin = 0; begin < a_normalized.size();)
			{
				size_t end = begin;
				size_t words{};
				while (end < a_normalized.size())
				{
					const auto separator = a_normalized.find('-', end);
					end = separator == std::string::npos ?
						a_normalized.size() :
						separator;
					++words;
					const auto phrase =
						a_normalized.substr(begin, end - begin);
					if (phrase.size() > 1 || a_normalized.size() == 1)
					{
						AddMatches(
							a_match,
							kPhosphorIconGlyphs,
							phrase,
							{ 2, words, 2 });
						AddMatches(
							a_match,
							kPhosphorIconAliases,
							phrase,
							{ 2, words, 2 });
						AddMatches(
							a_match,
							kPhosphorIconDomainTerms,
							phrase,
							{ 2, words, 1 });
						AddMatches(
							a_match,
							kPhosphorIconTags,
							phrase,
							{ 1, words, 0 });
					}
					if (separator == std::string::npos)
						break;
					end = separator + 1;
				}
				const auto next = a_normalized.find('-', begin);
				if (next == std::string::npos)
					break;
				begin = next + 1;
			}
		}

		inline void AddMetadataMatches(
			BestMatch& a_match,
			std::string_view a_metadata)
		{
			AddNormalizedMetadataMatches(
				a_match,
				NormalizeIconName(a_metadata));
		}

		[[nodiscard]] inline BestMatch EvaluateGroup(
			std::span<const std::string_view> a_metadata)
		{
			BestMatch match;
			for (const auto value : a_metadata)
				AddMetadataMatches(match, value);
			if (match.glyph)
				return match;

			for (const auto value : a_metadata)
			{
				const auto normalized = NormalizeIconName(value);
				if (const auto singular =
						SingularizeMetadataWords(normalized);
					!singular.empty())
					AddNormalizedMetadataMatches(match, singular);
			}
			return match;
		}

		[[nodiscard]] inline char32_t FindExactAuthoritative(
			std::string_view a_name)
		{
			const auto normalized = NormalizeIconName(a_name);
			if (normalized.empty())
				return {};
			for (const auto& mappings : {
					std::span<const IconPhraseMapping>{ kPhosphorIconGlyphs },
					std::span<const IconPhraseMapping>{ kPhosphorIconAliases },
					std::span<const IconPhraseMapping>{
						kPhosphorIconDomainTerms } })
			{
				const auto first = std::ranges::lower_bound(
					mappings,
					normalized,
					{},
					&IconPhraseMapping::phrase);
				if (first != mappings.end() && first->phrase == normalized)
					return first->glyph;
			}
			return {};
		}
	}

	class IconResolver
	{
	public:
		[[nodiscard]] static IconSelection Resolve(
			const IconResolutionRequest& a_request)
		{
			if (a_request.explicitGlyph)
			{
				return {
					IsValidUnicodeScalar(*a_request.explicitGlyph) ?
						IconSelectionStatus::kSelected :
						IconSelectionStatus::kInvalidRawGlyph,
					*a_request.explicitGlyph
				};
			}
			if (const auto glyph =
					IconResolverDetail::FindExactAuthoritative(
						a_request.explicitName))
				return { IconSelectionStatus::kSelected, glyph };

			auto primary =
				IconResolverDetail::EvaluateGroup(a_request.primaryMetadata);
			if (primary.glyph)
				return { IconSelectionStatus::kSelected, primary.glyph };

			auto secondary =
				IconResolverDetail::EvaluateGroup(a_request.secondaryMetadata);
			if (secondary.glyph)
				return {
					IconSelectionStatus::kSelected,
					secondary.glyph
				};
			return { IconSelectionStatus::kNoMatch, {} };
		}
	};

	[[nodiscard]] inline char32_t FindPhosphorSlugGlyphOrZero(
		std::string_view a_slug) noexcept
	{
		const auto [first, last] =
			IconResolverDetail::EqualRange(kPhosphorIconGlyphs, a_slug);
		return first != last ? kPhosphorIconGlyphs[first].glyph : char32_t{};
	}

	[[nodiscard]] inline char32_t FindPhosphorIconGlyphOrZero(
		std::string_view a_name)
	{
		return FindPhosphorSlugGlyphOrZero(NormalizeIconName(a_name));
	}

	[[nodiscard]] inline IconSelection ResolveIconSelection(
		std::string_view a_explicitName,
		std::string_view a_primaryMetadata = {},
		std::string_view a_secondaryMetadata = {})
	{
		const std::array primary{ a_primaryMetadata };
		const std::array secondary{ a_secondaryMetadata };
		return IconResolver::Resolve({
			.explicitName = a_explicitName,
			.primaryMetadata = a_primaryMetadata.empty() ?
				std::span<const std::string_view>{} :
				std::span<const std::string_view>{ primary },
			.secondaryMetadata = a_secondaryMetadata.empty() ?
				std::span<const std::string_view>{} :
				std::span<const std::string_view>{ secondary }
		});
	}

	[[nodiscard]] inline char32_t ResolveNamedIconGlyphOrZero(
		std::string_view a_name)
	{
		return IconResolverDetail::FindExactAuthoritative(a_name);
	}

	[[nodiscard]] inline char32_t ResolveInferredIconGlyphOrZero(
		std::string_view a_name)
	{
		return ResolveIconSelection({}, a_name).GlyphOr({});
	}

	[[nodiscard]] inline char32_t ResolveSemanticIconGlyph(
		std::string_view a_explicitName,
		std::string_view a_primaryMetadata,
		std::string_view a_secondaryMetadata,
		char32_t a_fallback)
	{
		return ResolveIconSelection(
			a_explicitName,
			a_primaryMetadata,
			a_secondaryMetadata)
			.GlyphOr(a_fallback);
	}

	[[nodiscard]] inline char32_t ResolveIconGlyph(
		IconKind,
		std::string_view a_name)
	{
		return ResolveIconSelection(a_name).GlyphOr(
			PhosphorGlyph::kQuestion);
	}

	[[nodiscard]] inline char32_t ResolveClientIconGlyph(
		std::string_view a_iconName,
		std::string_view a_category,
		std::string_view a_displayName)
	{
		return ResolveSemanticIconGlyph(
			a_iconName,
			a_displayName,
			a_category,
			PhosphorGlyph::kQuestion);
	}

	[[nodiscard]] inline char32_t ResolveIconGlyph(
		IconKind,
		std::string_view a_iconName,
		std::string_view a_fallbackName)
	{
		return ResolveSemanticIconGlyph(
			a_iconName,
			a_fallbackName,
			{},
			PhosphorGlyph::kQuestion);
	}

	[[nodiscard]] inline char32_t ResolveCategoryIconGlyph(
		std::string_view a_category,
		std::string_view,
		std::string_view,
		std::string_view = {},
		std::string_view a_categoryIconName = {})
	{
		return ResolveSemanticIconGlyph(
			a_categoryIconName,
			a_category,
			{},
			PhosphorGlyph::kQuestion);
	}

	[[nodiscard]] inline char32_t ResolveActionIconGlyph(
		std::string_view a_name,
		std::string_view a_label = {})
	{
		return ResolveIconSelection(a_name, a_label).GlyphOr({});
	}

	[[nodiscard]] inline char32_t ResolveAutomaticIconGlyph(
		char32_t a_chosenGlyph,
		std::string_view a_label,
		char32_t a_fallback)
	{
		if (a_chosenGlyph)
		{
			return IconResolver::Resolve({
				.explicitGlyph = a_chosenGlyph
			}).GlyphOr(a_fallback);
		}
		return ResolveIconSelection({}, a_label).GlyphOr(a_fallback);
	}
}
