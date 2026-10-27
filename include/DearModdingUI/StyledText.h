#pragma once

#include <DearModdingUI/API.h>

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace dmui
{
	enum class TextTone : uint32_t
	{
		kInherit = DMUI_TEXT_TONE_INHERIT,
		kMuted = DMUI_TEXT_TONE_MUTED,
		kAccent = DMUI_TEXT_TONE_ACCENT,
		kAccentMuted = DMUI_TEXT_TONE_ACCENT_MUTED,
		kInfo = DMUI_TEXT_TONE_INFO,
		kSuccess = DMUI_TEXT_TONE_SUCCESS,
		kWarning = DMUI_TEXT_TONE_WARNING,
		kError = DMUI_TEXT_TONE_ERROR,
		kDisabled = DMUI_TEXT_TONE_DISABLED,
		kRestartNeeded = DMUI_TEXT_TONE_RESTART_NEEDED,
		kCurrentHotkey = DMUI_TEXT_TONE_CURRENT_HOTKEY
	};

	// A byte range of one shared text; DMUI_TextSpan defines the rules.
	using TextSpan = DMUI_TextSpan;

	// A span's look without its range; Span or StyledText::Append places it.
	struct TextStyle : DMUI_TextSpan
	{
		constexpr TextStyle() noexcept : DMUI_TextSpan{} {}

		[[nodiscard]] constexpr TextStyle Tone(TextTone a_tone) const noexcept
		{
			auto style = *this;
			style.foreground = static_cast<DMUI_TextTone>(a_tone);
			return style;
		}

		[[nodiscard]] constexpr TextStyle Background(TextTone a_tone) const noexcept
		{
			auto style = *this;
			style.background = static_cast<DMUI_TextTone>(a_tone);
			return style;
		}

		// An explicit color wins over the tone.
		[[nodiscard]] constexpr TextStyle Color(DMUI_Vec4 a_color) const noexcept
		{
			auto style = WithFlag(DMUI_TEXT_SPAN_CUSTOM_FOREGROUND, true);
			style.foregroundColor = a_color;
			return style;
		}

		[[nodiscard]] constexpr TextStyle BackgroundColor(DMUI_Vec4 a_color) const noexcept
		{
			auto style = WithFlag(DMUI_TEXT_SPAN_CUSTOM_BACKGROUND, true);
			style.backgroundColor = a_color;
			return style;
		}

		[[nodiscard]] constexpr TextStyle Bold(bool a_enabled = true) const noexcept
		{
			return WithFlag(DMUI_TEXT_SPAN_BOLD, a_enabled);
		}

		[[nodiscard]] constexpr TextStyle Italic(bool a_enabled = true) const noexcept
		{
			return WithFlag(DMUI_TEXT_SPAN_ITALIC, a_enabled);
		}

		[[nodiscard]] constexpr TextStyle Underline(bool a_enabled = true) const noexcept
		{
			return WithFlag(DMUI_TEXT_SPAN_UNDERLINE, a_enabled);
		}

		[[nodiscard]] constexpr TextStyle Strikethrough(bool a_enabled = true) const noexcept
		{
			return WithFlag(DMUI_TEXT_SPAN_STRIKETHROUGH, a_enabled);
		}

		// Text views report hover and click events with this id.
		[[nodiscard]] constexpr TextStyle Interactive(uint64_t a_userId) const noexcept
		{
			auto style = WithFlag(DMUI_TEXT_SPAN_INTERACTIVE, true);
			style.userId = a_userId;
			return style;
		}

		// Text only; a text view or field uses the monospace role.
		[[nodiscard]] constexpr TextStyle Role(DMUI_FontRole a_role) const noexcept
		{
			auto style = WithFlag(DMUI_TEXT_SPAN_HAS_ROLE, true);
			style.role = a_role;
			return style;
		}

		[[nodiscard]] constexpr TextSpan Span(size_t a_offset, size_t a_length) const noexcept
		{
			TextSpan span = *this;
			span.offset = a_offset;
			span.length = a_length;
			return span;
		}

	private:
		[[nodiscard]] constexpr TextStyle WithFlag(
			DMUI_TextSpanFlags a_flag, bool a_enabled) const noexcept
		{
			auto style = *this;
			style.flags = a_enabled ? (flags | a_flag) : (flags & ~a_flag);
			return style;
		}
	};

	static_assert(sizeof(TextStyle) == sizeof(DMUI_TextSpan));
	static_assert(std::is_standard_layout_v<TextStyle>);

	// Owns the text and the spans over it; append runs in order. Allocation may throw.
	class StyledText
	{
	public:
		StyledText& Append(std::string_view a_text)
		{
			text_.append(a_text);
			return *this;
		}

		StyledText& Append(std::string_view a_text, const TextStyle& a_style)
		{
			if (!a_text.empty())
				spans_.push_back(a_style.Span(text_.size(), a_text.size()));
			text_.append(a_text);
			return *this;
		}

		void Clear() noexcept
		{
			text_.clear();
			spans_.clear();
		}

		[[nodiscard]] std::string_view Text() const noexcept { return text_; }
		[[nodiscard]] std::span<const TextSpan> Spans() const noexcept { return spans_; }

	private:
		std::string text_;
		std::vector<TextSpan> spans_;
	};

	struct TextOptions
	{
		// Wraps at the content region, or at wrapWidth when it is positive.
		bool wrap{};
		bool bullet{};
		float wrapWidth{};

		[[nodiscard]] constexpr DMUI_TextOptions Abi() const noexcept
		{
			return {
				(wrap ? DMUI_TEXT_OPTION_WRAP : 0u) |
					(bullet ? DMUI_TEXT_OPTION_BULLET : 0u),
				wrapWidth
			};
		}
	};
}
