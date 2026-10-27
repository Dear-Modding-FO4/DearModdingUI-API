#pragma once

#include <DearModdingUI/API.h>
#include <DearModdingUI/UI.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <span>
#include <string>
#include <string_view>
#include <utility>

namespace dmui
{
	inline constexpr size_t kNoTextOffset{ DMUI_TEXT_VIEW_NO_OFFSET };

	// Spans use byte offsets into text; the DMUI_TextViewDescriptor comment
	// defines the scroll, tail, and selection behavior.
	struct TextViewRequest
	{
		std::string_view text;
		std::span<const size_t> lineOffsets;
		std::span<const size_t> matchByteOffsets;
		size_t matchByteLength{};
		std::span<const TextSpan> spans;
		uint64_t contentRevision{};
		uint64_t matchRevision{};
		uint64_t spanRevision{};
		// Absolute index of line 0; raise it by the lines trimmed from the front.
		uint64_t firstLineNumber{};
		// Stay pinned to the end while scrolled to it.
		bool followTail{};
		// Pointer selection and Ctrl+C copy.
		bool selectable{};
		ui::Vec2 viewport{};
	};

	enum class TextViewScroll : uint32_t
	{
		kNone = DMUI_TEXT_VIEW_SCROLL_NONE,
		kPageUp = DMUI_TEXT_VIEW_SCROLL_PAGE_UP,
		kPageDown = DMUI_TEXT_VIEW_SCROLL_PAGE_DOWN,
		kTop = DMUI_TEXT_VIEW_SCROLL_TOP,
		kBottom = DMUI_TEXT_VIEW_SCROLL_BOTTOM
	};

	struct TextViewState
	{
		uint64_t contentRevision{};
		uint64_t matchRevision{};
		size_t activeMatch{ kNoTextOffset };
		size_t revealByteOffset{ kNoTextOffset };
		// One-shot; a draw resets it to kNone.
		TextViewScroll scroll{ TextViewScroll::kNone };
		// Rewritten by every draw.
		bool atBottom{};
		bool spanHovered{};
		bool spanClicked{};
		// The hovered or clicked span; kNoTextOffset without an event.
		size_t spanIndex{ kNoTextOffset };
		uint64_t spanUserId{};
	};

	constexpr void SynchronizeTextViewState(
		const TextViewRequest& a_request,
		TextViewState& a_state) noexcept
	{
		if (a_state.contentRevision == a_request.contentRevision &&
			a_state.matchRevision == a_request.matchRevision)
			return;
		a_state.contentRevision = a_request.contentRevision;
		a_state.matchRevision = a_request.matchRevision;
		a_state.activeMatch = kNoTextOffset;
		a_state.revealByteOffset = kNoTextOffset;
	}

	[[nodiscard]] constexpr bool RevealTextOffset(
		const TextViewRequest& a_request,
		TextViewState& a_state,
		size_t a_byteOffset) noexcept
	{
		SynchronizeTextViewState(a_request, a_state);
		if (a_byteOffset > a_request.text.size() ||
			(a_byteOffset < a_request.text.size() &&
				(static_cast<unsigned char>(a_request.text[a_byteOffset]) &
					0xC0u) == 0x80u))
			return false;
		a_state.activeMatch = kNoTextOffset;
		a_state.revealByteOffset = a_byteOffset;
		return true;
	}

	[[nodiscard]] constexpr bool SelectNextTextMatch(
		const TextViewRequest& a_request,
		TextViewState& a_state) noexcept
	{
		SynchronizeTextViewState(a_request, a_state);
		if (a_request.matchByteOffsets.empty())
		{
			a_state.activeMatch = kNoTextOffset;
			a_state.revealByteOffset = kNoTextOffset;
			return false;
		}
		a_state.activeMatch =
			a_state.activeMatch == kNoTextOffset ||
					a_state.activeMatch + 1 >=
						a_request.matchByteOffsets.size() ?
				0 :
				a_state.activeMatch + 1;
		a_state.revealByteOffset =
			a_request.matchByteOffsets[a_state.activeMatch];
		return true;
	}

	[[nodiscard]] constexpr bool SelectPreviousTextMatch(
		const TextViewRequest& a_request,
		TextViewState& a_state) noexcept
	{
		SynchronizeTextViewState(a_request, a_state);
		if (a_request.matchByteOffsets.empty())
		{
			a_state.activeMatch = kNoTextOffset;
			a_state.revealByteOffset = kNoTextOffset;
			return false;
		}
		a_state.activeMatch =
			a_state.activeMatch == kNoTextOffset ||
					a_state.activeMatch == 0 ||
					a_state.activeMatch >= a_request.matchByteOffsets.size() ?
				a_request.matchByteOffsets.size() - 1 :
				a_state.activeMatch - 1;
		a_state.revealByteOffset =
			a_request.matchByteOffsets[a_state.activeMatch];
		return true;
	}

	namespace text_view_detail
	{
		[[nodiscard]] constexpr size_t NextUTF8Boundary(
			std::string_view a_text,
			size_t a_offset) noexcept
		{
			auto next = (std::min)(a_offset + 1, a_text.size());
			while (next < a_text.size() &&
				(static_cast<unsigned char>(a_text[next]) & 0xC0u) == 0x80u)
				++next;
			return next;
		}

		[[nodiscard]] constexpr size_t PreviousUTF8Boundary(
			std::string_view a_text,
			size_t a_offset) noexcept
		{
			auto boundary = (std::min)(a_offset, a_text.size());
			while (boundary > 0 && boundary < a_text.size() &&
				(static_cast<unsigned char>(a_text[boundary]) & 0xC0u) == 0x80u)
				--boundary;
			return boundary;
		}

		[[nodiscard]] inline std::string_view FitClippedNavigationLabel(
			std::string_view a_label,
			float a_availableWidth) noexcept
		{
			if (a_availableWidth <= 0.0f || a_label.empty())
				return {};

			size_t fittedBytes{};
			size_t excludedBytes{ a_label.size() };
			while (NextUTF8Boundary(a_label, fittedBytes) < excludedBytes)
			{
				auto candidate = PreviousUTF8Boundary(
					a_label,
					fittedBytes + (excludedBytes - fittedBytes) / 2);
				if (candidate <= fittedBytes)
					candidate = NextUTF8Boundary(a_label, fittedBytes);
				if (ui::CalcTextSize(a_label.substr(0, candidate)).x <=
					a_availableWidth)
					fittedBytes = candidate;
				else
					excludedBytes = candidate;
			}
			return a_label.substr(0, fittedBytes);
		}
	}

	// Projection returns a pair-like label and byte offset for each caller-owned item.
	template <class Item, class Projection>
	[[nodiscard]] bool DrawTextViewNavigation(
		const char* a_id,
		std::span<const Item> a_items,
		const DMUI_StyleMetrics& a_metrics,
		const TextViewRequest& a_request,
		TextViewState& a_state,
		Projection&& a_projection)
	{
		if (a_items.size() >
			static_cast<size_t>((std::numeric_limits<int32_t>::max)()))
			return false;

		const auto availableWidth =
			(std::max)(ui::GetContentRegionAvail().x, 0.0f);
		const auto spacing = (std::max)(a_metrics.itemSpacing.x, 0.0f);
		float lineWidth{};
		bool drewOnLine{};
		bool selected{};

		for (size_t index = 0; index < a_items.size(); ++index)
		{
			auto [labelValue, byteOffset] =
				std::invoke(a_projection, a_items[index]);
			const std::string label{ std::string_view{ labelValue } };
			const auto natural = ui::MeasureButton({ .label = label.c_str() });
			const auto padding = natural.x - ui::CalcTextSize(label).x;
			const auto buttonWidth = (std::min)(natural.x, availableWidth);
			const auto labelClipped = natural.x > buttonWidth;
			const std::string fittedLabel{
				labelClipped ?
					text_view_detail::FitClippedNavigationLabel(
						label,
						(std::max)(buttonWidth - padding, 0.0f)) :
					std::string_view{ label }
			};
			if (drewOnLine &&
				lineWidth + spacing + buttonWidth <= availableWidth)
			{
				ui::SameLine();
				lineWidth += spacing;
			}
			else
			{
				lineWidth = 0.0f;
				drewOnLine = false;
			}

			ui::PushID(a_id);
			ui::PushID(static_cast<int32_t>(index));
			const auto pressed = ui::Button({
				.id = "##TextViewNavigation",
				.label = fittedLabel.c_str(),
				.tooltip = labelClipped ? label.c_str() : nullptr,
				.size = { buttonWidth, natural.y }
			});
			if (pressed &&
				RevealTextOffset(a_request, a_state, byteOffset))
				selected = true;
			ui::PopID();
			ui::PopID();

			lineWidth += buttonWidth;
			drewOnLine = true;
		}
		return selected;
	}
}
