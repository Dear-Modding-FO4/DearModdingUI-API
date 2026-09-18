#include <DearModdingUI/TextView.h>

#include <array>
#include <span>
#include <string_view>
#include <utility>

namespace
{
	constexpr bool ResetsStaleTextViewState()
	{
		constexpr std::array<size_t, 2> lines{ 0, 4 };
		constexpr std::array<size_t, 1> matches{ 4 };
		const dmui::TextViewRequest request{
			.text = "one\ntwo",
			.lineOffsets = lines,
			.matchByteOffsets = matches,
			.matchByteLength = 3,
			.contentRevision = 7,
			.matchRevision = 9
		};
		dmui::TextViewState state{
			.contentRevision = 6,
			.matchRevision = 8,
			.activeMatch = 0,
			.revealByteOffset = 4
		};
		dmui::SynchronizeTextViewState(request, state);
		return state.contentRevision == 7 &&
			state.matchRevision == 9 &&
			state.activeMatch == dmui::kNoTextOffset &&
			state.revealByteOffset == dmui::kNoTextOffset;
	}

	constexpr bool SupportsEmptyMatchResults()
	{
		constexpr std::array<size_t, 1> lines{ 0 };
		const dmui::TextViewRequest request{
			.text = "text",
			.lineOffsets = lines,
			.matchByteLength = 5,
			.contentRevision = 1,
			.matchRevision = 2
		};
		dmui::TextViewState state;
		return !dmui::SelectNextTextMatch(request, state) &&
			state.contentRevision == 1 &&
			state.matchRevision == 2 &&
			state.activeMatch == dmui::kNoTextOffset;
	}

	constexpr bool DirectRevealClearsMatchSelection()
	{
		constexpr std::array<size_t, 1> lines{ 0 };
		constexpr std::array<size_t, 1> matches{ 0 };
		const dmui::TextViewRequest request{
			.text = "text",
			.lineOffsets = lines,
			.matchByteOffsets = matches,
			.matchByteLength = 4,
			.contentRevision = 1,
			.matchRevision = 1
		};
		dmui::TextViewState state{
			.contentRevision = 1,
			.matchRevision = 1,
			.activeMatch = 0
		};
		return dmui::RevealTextOffset(request, state, 2) &&
			state.activeMatch == dmui::kNoTextOffset &&
			state.revealByteOffset == 2;
	}

	static_assert(ResetsStaleTextViewState());
	static_assert(SupportsEmptyMatchResults());
	static_assert(DirectRevealClearsMatchSelection());
	static_assert(
		dmui::text_view_detail::NextUTF8Boundary("\xC3\xA9x", 0) == 2);
	static_assert(
		dmui::text_view_detail::PreviousUTF8Boundary("\xC3\xA9x", 1) == 0);
}

void CompileTextViewNavigation()
{
	struct Section
	{
		std::string_view label;
		size_t byteOffset;
	};
	const std::array sections{
		Section{ "Overview##literal", 0 },
		Section{ "Details", 9 }
	};
	const std::array<size_t, 2> lines{ 0, 9 };
	const dmui::TextViewRequest request{
		.text = "Overview\nDetails",
		.lineOffsets = lines,
		.contentRevision = 1,
		.matchRevision = 1
	};
	dmui::TextViewState state;
	DMUI_StyleMetrics metrics{};
	metrics.structSize = sizeof(metrics);
	(void)dmui::DrawTextViewNavigation(
		"sections",
		std::span<const Section>{ sections },
		metrics,
		request,
		state,
		[](const Section& a_section) {
			return std::pair{ a_section.label, a_section.byteOffset };
		});
}
