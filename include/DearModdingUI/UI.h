#pragma once

#include <DearModdingUI/CUIAPI.h>
#include <DearModdingUI/Abi.h>
#include <DearModdingUI/Button.h>
#include <DearModdingUI/Choice.h>
#include <DearModdingUI/ColorEdit.h>
#include <DearModdingUI/KeyCapture.h>
#include <DearModdingUI/Numeric.h>
#include <DearModdingUI/Section.h>
#include <DearModdingUI/Spinner.h>
#include <DearModdingUI/StyledText.h>
#include <DearModdingUI/TextInput.h>
#include <DearModdingUI/Toggle.h>

#include <cfloat>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <initializer_list>
#include <limits>
#include <new>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace dmui
{
	struct ImageHandle
	{
		DMUI_ImageHandle value{ DMUI_INVALID_IMAGE_HANDLE };

		[[nodiscard]] explicit operator bool() const noexcept
		{
			return value != DMUI_INVALID_IMAGE_HANDLE;
		}
	};
}

namespace dmui::ui::detail
{
	struct Context
	{
		const DMUI_UIAPI* api{};
		DMUI_ClientHandle client{ DMUI_INVALID_CLIENT_HANDLE };
		DMUI_Result result{ DMUI_RESULT_OK };
		uint32_t abiMinor{ DMUI_ABI_MINOR };
		DMUI_Result lastResult{ DMUI_RESULT_OK };
	};

	inline thread_local Context* currentContext{};

	class ScopedContext
	{
	public:
		ScopedContext(
			const DMUI_UIAPI* a_api,
			DMUI_ClientHandle a_client,
			uint32_t a_abiMinor = DMUI_ABI_MINOR) noexcept :
			m_previous(currentContext),
			m_context{ a_api, a_client, DMUI_RESULT_OK, a_abiMinor }
		{
			currentContext = &m_context;
		}

		~ScopedContext() noexcept
		{
			currentContext = m_previous;
		}

		ScopedContext(const ScopedContext&) = delete;
		ScopedContext(ScopedContext&&) = delete;
		ScopedContext& operator=(const ScopedContext&) = delete;
		ScopedContext& operator=(ScopedContext&&) = delete;

		[[nodiscard]] DMUI_Result Result() const noexcept
		{
			return m_context.result;
		}

	private:
		Context* m_previous;
		Context m_context;
	};

	inline void Record(DMUI_Result a_result) noexcept
	{
		if (currentContext && a_result != DMUI_RESULT_OK)
			currentContext->lastResult = a_result;
		// Unsupported features are soft failures so callers can probe and fall back.
		if (a_result == DMUI_RESULT_UNSUPPORTED_ABI)
			return;
		if (a_result != DMUI_RESULT_OK &&
			currentContext &&
			currentContext->result == DMUI_RESULT_OK)
			currentContext->result = a_result;
	}

	[[nodiscard]] inline DMUI_Result LastResult() noexcept
	{
		return currentContext ?
			(currentContext->result != DMUI_RESULT_OK ?
				currentContext->result : currentContext->lastResult) :
			DMUI_RESULT_HOST_NOT_READY;
	}

	inline void ClearError() noexcept
	{
		if (currentContext)
		{
			currentContext->result = DMUI_RESULT_OK;
			currentContext->lastResult = DMUI_RESULT_OK;
		}
	}

	template <auto Slot, class... Arguments>
	[[nodiscard]] DMUI_Result Invoke(
		Arguments... a_arguments) noexcept
	{
		auto* context = currentContext;
		if (!context ||
			!context->api ||
			context->client == DMUI_INVALID_CLIENT_HANDLE)
			return DMUI_RESULT_HOST_NOT_READY;
		context->lastResult = DMUI_RESULT_OK;
		if (context->abiMinor < dmui::detail::SlotAbiMinor<Slot>)
			return DMUI_RESULT_UNSUPPORTED_ABI;
		const auto function = context->api->*Slot;
		return function(context->client, a_arguments...);
	}

	[[nodiscard]] inline const char* TextData(std::string_view a_text) noexcept
	{
		static constexpr char empty[]{ "" };
		return a_text.empty() ? empty : a_text.data();
	}

	[[nodiscard]] inline bool TextRange(
		const char* a_begin,
		const char* a_end,
		const char*& a_text,
		size_t& a_length) noexcept
	{
		if (!a_begin)
			return false;
		if (!a_end)
		{
			a_text = a_begin;
			a_length = std::char_traits<char>::length(a_begin);
			return true;
		}
		if (a_end < a_begin)
			return false;
		a_text = a_begin;
		a_length = static_cast<size_t>(a_end - a_begin);
		return true;
	}

	[[nodiscard]] inline bool Format(
		const char* a_format,
		std::va_list a_arguments,
		std::vector<char>& a_text) noexcept
	{
		if (!a_format)
		{
			Record(DMUI_RESULT_INVALID_ARGUMENT);
			return false;
		}
		try
		{
			std::va_list measure;
			va_copy(measure, a_arguments);
			const auto length = std::vsnprintf(nullptr, 0, a_format, measure);
			va_end(measure);
			if (length < 0)
			{
				Record(DMUI_RESULT_INVALID_ARGUMENT);
				return false;
			}
			a_text.resize(static_cast<size_t>(length) + 1u);
			std::va_list write;
			va_copy(write, a_arguments);
			const auto written = std::vsnprintf(
				a_text.data(),
				a_text.size(),
				a_format,
				write);
			va_end(write);
			if (written != length)
			{
				Record(DMUI_RESULT_CALLBACK_FAILED);
				return false;
			}
			return true;
		}
		catch (const std::bad_alloc&)
		{
			Record(DMUI_RESULT_RESOURCE_EXHAUSTED);
			return false;
		}
		catch (const std::length_error&)
		{
			Record(DMUI_RESULT_RESOURCE_EXHAUSTED);
			return false;
		}
	}
}

#include <DearModdingUI/UIChecked.generated.h>

namespace dmui::ui
{
	// Deduction keeps Image(handle, {width, height}) unambiguous.
	template <class Options>
		requires std::is_same_v<Options, DMUI_ImageDrawOptions>
	[[nodiscard]] inline bool Image(
		ImageHandle a_image, const Options& a_options) noexcept
	{
		uint32_t drawn{};
		detail::Record(checked::Image(a_image.value, &a_options, &drawn));
		return drawn != 0;
	}

	[[nodiscard]] inline bool Image(ImageHandle a_image, Vec2 a_size) noexcept
	{
		const DMUI_ImageDrawOptions options{
			a_size, { 0.0f, 0.0f }, { 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }, 1u
		};
		return Image(a_image, options);
	}

	inline void PlotAnnotated(
		const char* a_id, const DMUI_AnnotatedPlotDescriptor& a_plot) noexcept
	{
		detail::Record(checked::PlotAnnotated(a_id, &a_plot));
	}

	[[nodiscard]] inline Vec2 GetCursorPos() noexcept
	{
		Vec2 position{};
		detail::Record(checked::GetCursorPos(&position));
		return position;
	}

	inline void SetCursorPos(Vec2 a_position) noexcept
	{
		detail::Record(checked::SetCursorPos(a_position));
	}

	[[nodiscard]] inline float GetCursorPosX() noexcept { return GetCursorPos().x; }
	[[nodiscard]] inline float GetCursorPosY() noexcept { return GetCursorPos().y; }
	inline void SetCursorPosX(float a_x) noexcept { SetCursorPos({ a_x, GetCursorPosY() }); }
	inline void SetCursorPosY(float a_y) noexcept { SetCursorPos({ GetCursorPosX(), a_y }); }

	inline void TextAligned(float a_alignX, float a_width, const char* a_text, size_t a_length) noexcept
	{
		detail::Record(checked::TextAligned(a_alignX, a_width, a_text, a_length));
	}

	inline void TextAligned(float a_alignX, float a_width, std::string_view a_text) noexcept
	{
		TextAligned(a_alignX, a_width, detail::TextData(a_text), a_text.size());
	}

	namespace detail
	{
		template <class T>
		concept TextLike = std::is_convertible_v<const T&, std::string_view>;
	}

	// Lays the text out inline as one item with styled ranges; the DMUI_TextSpan
	// comment and the specification define the rules and limits.
	// A template so a string literal beats the printf-style overload.
	template <detail::TextLike T>
	inline void Text(
		const T& a_string,
		std::span<const TextSpan> a_spans,
		TextOptions a_options = {}) noexcept
	{
		const std::string_view a_text{ a_string };
		if (a_spans.size() > (std::numeric_limits<uint32_t>::max)())
		{
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return;
		}
		const auto options = a_options.Abi();
		detail::Record(checked::Text(
			detail::TextData(a_text), a_text.size(),
			a_spans.data(), static_cast<uint32_t>(a_spans.size()), &options));
	}

	// One style over the whole text.
	template <detail::TextLike T>
	inline void Text(
		const T& a_string,
		const TextStyle& a_style,
		TextOptions a_options = {}) noexcept
	{
		const std::string_view a_text{ a_string };
		const auto span = a_style.Span(0, a_text.size());
		Text(a_text, std::span<const TextSpan>{ &span, 1 }, a_options);
	}

	inline void Text(const StyledText& a_text, TextOptions a_options = {}) noexcept
	{
		Text(a_text.Text(), a_text.Spans(), a_options);
	}

	class ListClipper
	{
	public:
		ListClipper() noexcept = default;
		~ListClipper() noexcept { End(); }

		ListClipper(const ListClipper&) = delete;
		ListClipper(ListClipper&&) = delete;
		ListClipper& operator=(const ListClipper&) = delete;
		ListClipper& operator=(ListClipper&&) = delete;

		void Begin(int32_t a_itemsCount, float a_itemsHeight = -1.0f) noexcept
		{
			End();
			DisplayStart = DisplayEnd = 0;
			detail::Record(checked::ListClipperBegin(
				a_itemsCount, a_itemsHeight, &m_clipper));
		}

		[[nodiscard]] bool Step() noexcept
		{
			if (!m_clipper)
				return false;
			uint32_t stepping{};
			detail::Record(checked::ListClipperStep(
				m_clipper, &stepping, &DisplayStart, &DisplayEnd));
			if (!stepping)
				m_clipper = 0;
			return stepping != 0;
		}

		void End() noexcept
		{
			if (!m_clipper)
				return;
			detail::Record(checked::ListClipperEnd(m_clipper));
			m_clipper = 0;
		}

		int32_t DisplayStart{ 0 };
		int32_t DisplayEnd{ 0 };

	private:
		uint64_t m_clipper{ 0 };
	};

	[[nodiscard]] inline DMUI_Result LastResult() noexcept
	{
		return detail::LastResult();
	}

	inline void ClearError() noexcept
	{
		detail::ClearError();
	}

	[[nodiscard]] inline DMUI_Result GetStyleMetrics(
		DMUI_StyleMetrics& a_metrics) noexcept
	{

		a_metrics = {};
		const auto result = checked::GetStyleMetrics(&a_metrics);
		detail::Record(result);
		return result;
	}

	[[nodiscard]] inline bool BeginPanel(
		const char* a_id, Vec2 a_size = {}, PanelFlags a_flags = PanelFlags::kNone) noexcept
	{
		uint32_t visible{};
		detail::Record(checked::BeginPanel(a_id, a_size,
			static_cast<DMUI_UIPanelFlags>(a_flags), &visible));
		return visible != 0;
	}

	inline void EndPanel() noexcept { detail::Record(checked::EndPanel()); }

	class PanelScope
	{
	public:
		explicit PanelScope(const char* a_id, Vec2 a_size = {},
			PanelFlags a_flags = PanelFlags::kNone) noexcept :
			m_visible(BeginPanel(a_id, a_size, a_flags)) {}
		~PanelScope() noexcept { End(); }
		PanelScope(const PanelScope&) = delete;
		PanelScope& operator=(const PanelScope&) = delete;
		[[nodiscard]] explicit operator bool() const noexcept { return m_visible; }
		void End() noexcept
		{
			if (std::exchange(m_visible, false))
				EndPanel();
		}

	private:
		bool m_visible{};
	};

	// Draws the section header and, while it is expanded, opens its content;
	// end a begin that returned true. The host writes options.expanded when the
	// header is clicked.
	[[nodiscard]] inline bool BeginSection(const SectionOptions& a_options) noexcept
	{
		uint32_t expanded = a_options.expanded && *a_options.expanded ? 1u : 0u;
		const auto section = a_options.Abi(a_options.expanded ? &expanded : nullptr);
		uint32_t visible{};
		const auto result = checked::BeginSection(&section, &visible);
		detail::Record(result);
		if (result == DMUI_RESULT_OK && a_options.expanded)
			*a_options.expanded = expanded != 0;
		return visible != 0;
	}

	inline void EndSection() noexcept { detail::Record(checked::EndSection()); }

	class SectionScope
	{
	public:
		explicit SectionScope(const SectionOptions& a_options) noexcept :
			m_visible(BeginSection(a_options)) {}
		~SectionScope() noexcept { End(); }
		SectionScope(const SectionScope&) = delete;
		SectionScope& operator=(const SectionScope&) = delete;
		[[nodiscard]] explicit operator bool() const noexcept { return m_visible; }
		void End() noexcept
		{
			if (std::exchange(m_visible, false))
				EndSection();
		}

	private:
		bool m_visible{};
	};

	// The one dropdown; changed is set on the frame an option is chosen.
	// No label is drawn.
	[[nodiscard]] inline ChoiceResult Choice(const ChoiceOptions& a_options) noexcept
	{
		const auto choice = a_options.Abi();
		DMUI_ChoiceResult result{ a_options.selected, 0u, 0u };
		detail::Record(checked::Choice(&choice, &result));
		return { result.selected, result.changed != 0, result.completed != 0 };
	}

	inline void BeginDisabled(bool a_disabled = true) noexcept
	{
		detail::Record(checked::BeginDisabled(a_disabled ? 1u : 0u));
	}

	inline void EndDisabled() noexcept
	{
		detail::Record(checked::EndDisabled());
	}

	[[nodiscard]] inline bool BeginTable(
		const char* a_id,
		int32_t a_columns,
		TableFlags a_flags = TableFlags::kNone,
		Vec2 a_outerSize = {},
		float a_innerWidth = 0.0f) noexcept
	{
		uint32_t visible{};
		detail::Record(checked::BeginTable(
			a_id,
			a_columns,
			static_cast<DMUI_UITableFlags>(a_flags),
			a_outerSize,
			a_innerWidth,
			&visible));
		return visible != 0;
	}

	inline void EndTable() noexcept
	{
		detail::Record(checked::EndTable());
	}

	[[nodiscard]] inline bool BeginTooltip() noexcept
	{
		uint32_t visible{};
		detail::Record(checked::BeginTooltip(&visible));
		return visible != 0;
	}

	using Placement = DMUI_Placement;

	// Places the tooltip at a screen position; pivot {0, 0} anchors its top-left.
	[[nodiscard]] inline bool BeginTooltipAt(Placement a_placement) noexcept
	{
		uint32_t visible{};
		detail::Record(checked::BeginTooltipAt(a_placement, &visible));
		return visible != 0;
	}

	inline void EndTooltip() noexcept
	{
		detail::Record(checked::EndTooltip());
	}

	// Position and viewport are set on any page; the rest is zero unless the
	// calling page holds overlay focus.
	[[nodiscard]] inline DMUI_PointerState GetPointerState() noexcept
	{
		DMUI_PointerState state{};
		detail::Record(checked::GetPointerState(&state));
		return state;
	}

	inline void OpenPopup(const char* a_id) noexcept
	{
		detail::Record(checked::OpenPopup(a_id));
	}

	[[nodiscard]] inline bool BeginPopup(
		const char* a_id, WindowFlags a_flags = WindowFlags::kNone) noexcept
	{
		uint32_t visible{};
		detail::Record(checked::BeginPopup(a_id, static_cast<DMUI_UIWindowFlags>(a_flags), &visible));
		return visible != 0;
	}

	// Like BeginPopup; the placement applies on the frame the popup appears.
	[[nodiscard]] inline bool BeginPopupAt(
		const char* a_id, Placement a_placement,
		WindowFlags a_flags = WindowFlags::kNone) noexcept
	{
		uint32_t visible{};
		detail::Record(checked::BeginPopupAt(
			a_id, a_placement, static_cast<DMUI_UIWindowFlags>(a_flags), &visible));
		return visible != 0;
	}

	[[nodiscard]] inline bool BeginPopupModal(
		const char* a_id, bool& a_open, bool a_hasCloseButton = true,
		WindowFlags a_flags = WindowFlags::kAlwaysAutoResize) noexcept
	{
		uint32_t visible{}, open = a_open ? 1u : 0u;
		detail::Record(checked::BeginPopupModal(a_id, a_hasCloseButton ? 1u : 0u,
			&open, static_cast<DMUI_UIWindowFlags>(a_flags), &visible));
		a_open = open != 0;
		return visible != 0;
	}

	inline void EndPopup() noexcept { detail::Record(checked::EndPopup()); }
	inline void CloseCurrentPopup() noexcept { detail::Record(checked::CloseCurrentPopup()); }

	[[nodiscard]] inline bool IsPopupOpen(const char* a_id) noexcept
	{
		uint32_t open{};
		detail::Record(checked::IsPopupOpen(a_id, &open));
		return open != 0;
	}

	class PopupScope
	{
	public:
		explicit PopupScope(const char* a_id, WindowFlags a_flags = WindowFlags::kNone) noexcept :
			m_visible(BeginPopup(a_id, a_flags)) {}
		~PopupScope() noexcept { End(); }
		PopupScope(const PopupScope&) = delete;
		PopupScope& operator=(const PopupScope&) = delete;
		[[nodiscard]] explicit operator bool() const noexcept { return m_visible; }
		void End() noexcept
		{
			if (std::exchange(m_visible, false))
				EndPopup();
		}

	protected:
		explicit PopupScope(bool a_visible) noexcept : m_visible(a_visible) {}

	private:
		bool m_visible{};
	};

	class ModalScope : public PopupScope
	{
	public:
		ModalScope(const char* a_id, bool& a_open, bool a_hasCloseButton = true,
			WindowFlags a_flags = WindowFlags::kAlwaysAutoResize) noexcept :
			PopupScope(BeginPopupModal(a_id, a_open, a_hasCloseButton, a_flags)) {}
	};

	// The host button; true on the frame it is pressed.
	[[nodiscard]] inline bool Button(const ButtonOptions& a_options) noexcept
	{
		const auto button = a_options.Abi();
		uint32_t pressed{};
		detail::Record(checked::Button(&button, &pressed));
		return pressed != 0;
	}

	// The size Button would take with the same options at the current style.
	[[nodiscard]] inline Vec2 MeasureButton(const ButtonOptions& a_options) noexcept
	{
		const auto button = a_options.Abi();
		Vec2 size{};
		detail::Record(checked::MeasureButton(&button, &size));
		return size;
	}

	[[nodiscard]] inline Vec2 CalcTextSize(
		const char* a_text,
		const char* a_textEnd = nullptr,
		bool a_hideTextAfterDoubleHash = false,
		float a_wrapWidth = -1.0f) noexcept
	{
		const char* text{};
		size_t length{};
		if (!detail::TextRange(a_text, a_textEnd, text, length))
		{
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return {};
		}
		Vec2 size{};
		detail::Record(checked::CalcTextSize(
			text,
			length,
			a_hideTextAfterDoubleHash ? 1u : 0u,
			a_wrapWidth,
			&size));
		return size;
	}

	[[nodiscard]] inline Vec2 CalcTextSize(
		std::string_view a_text,
		bool a_hideTextAfterDoubleHash = false,
		float a_wrapWidth = -1.0f) noexcept
	{
		Vec2 size{};
		detail::Record(checked::CalcTextSize(
			detail::TextData(a_text),
			a_text.size(),
			a_hideTextAfterDoubleHash ? 1u : 0u,
			a_wrapWidth,
			&size));
		return size;
	}

	// The one checkbox; changed is set on the frame it is toggled. No label is drawn.
	[[nodiscard]] inline ToggleResult Toggle(const ToggleOptions& a_options) noexcept
	{
		const auto toggle = a_options.Abi();
		DMUI_ToggleResult result{ toggle.value, 0u };
		detail::Record(checked::Toggle(&toggle, &result));
		return { result.value != 0, result.changed != 0 };
	}

	// The one busy indicator; redraw it each frame while work is pending.
	inline void Spinner(const SpinnerOptions& a_options) noexcept
	{
		const auto spinner = a_options.Abi();
		detail::Record(checked::Spinner(&spinner));
	}

	// Seconds of frame time: advances only on DMUI frames, not with game pause.
	[[nodiscard]] inline double GetTime() noexcept
	{
		double seconds{};
		detail::Record(checked::GetTime(&seconds));
		return seconds;
	}

	// Seconds the previous frame took.
	[[nodiscard]] inline float GetDeltaTime() noexcept
	{
		float seconds{};
		detail::Record(checked::GetDeltaTime(&seconds));
		return seconds;
	}

	// The one key binding editor; changed is set on the frame a chord was captured
	// or cleared. No label is drawn.
	[[nodiscard]] inline KeyCaptureResult KeyCapture(const KeyCaptureOptions& a_options) noexcept
	{
		const auto capture = a_options.Abi();
		DMUI_KeyCaptureResult result{};
		detail::Record(checked::KeyCapture(&capture, &result));
		KeyCaptureResult captured;
		if (result.changed == 0)
			return captured;
		try
		{
			result.chord[DMUI_KEY_CHORD_CAPACITY - 1u] = '\0';
			captured.chord = result.chord;
			captured.changed = true;
		}
		catch (const std::bad_alloc&)
		{
			detail::Record(DMUI_RESULT_RESOURCE_EXHAUSTED);
		}
		return captured;
	}

	// The one color edit with optional preset swatches; changed is set on the
	// frame an edit changes the color. No label is drawn.
	[[nodiscard]] inline ColorEditResult ColorEdit(const ColorEditOptions& a_options) noexcept
	{
		const auto color = a_options.Abi();
		DMUI_ColorEditResult result{ color.rgba, 0u, 0u };
		detail::Record(checked::ColorEdit(&color, &result));
		return { result.rgba, result.changed != 0, result.completed != 0 };
	}

	// The one numeric input; changed is set on the frame an edit changes the value.
	// No label is drawn.
	template <NumericScalar T>
	[[nodiscard]] inline NumericResult<T> Numeric(const NumericOptions<T>& a_options) noexcept
	{
		const auto numeric = a_options.Abi();
		DMUI_NumericResult result{ numeric.value, 0u, 0u };
		detail::Record(checked::Numeric(&numeric, &result));
		return {
			FromNumericValue<T>(result.value),
			result.changed != 0,
			result.completed != 0
		};
	}

	inline void Dummy(Vec2 a_size) noexcept
	{
		detail::Record(checked::Dummy(a_size));
	}

	[[nodiscard]] inline Vec2 GetContentRegionAvail() noexcept
	{
		Vec2 size{};
		detail::Record(checked::GetContentRegionAvail(&size));
		return size;
	}

	[[nodiscard]] inline Vec2 GetCursorScreenPos() noexcept
	{
		Vec2 position{};
		detail::Record(checked::GetCursorScreenPos(&position));
		return position;
	}

	[[nodiscard]] inline float GetFontSize() noexcept
	{
		float size{};
		detail::Record(checked::GetFontSize(&size));
		return size;
	}

	[[nodiscard]] inline float GetFrameHeight() noexcept
	{
		float height{};
		detail::Record(checked::GetFrameHeight(&height));
		return height;
	}

	[[nodiscard]] inline Vec4 GetStyleColor(Color a_color) noexcept
	{
		Vec4 value{};
		detail::Record(checked::GetStyleColor(
			static_cast<DMUI_UIColor>(a_color),
			&value));
		return value;
	}

	[[nodiscard]] inline float GetTextLineHeightWithSpacing() noexcept
	{
		float height{};
		detail::Record(checked::GetTextLineHeightWithSpacing(&height));
		return height;
	}

	inline void Indent(float a_width = 0.0f) noexcept
	{
		detail::Record(checked::Indent(a_width));
	}

	enum class TextFieldStyle : uint32_t
	{
		kPlain = DMUI_TEXT_FIELD_STYLE_PLAIN,
		// The host search frame with a magnifier.
		kSearch = DMUI_TEXT_FIELD_STYLE_SEARCH
	};

	// Called after each frame's edits with the field text. The spans it returns
	// must outlive the InputText call; an exception fails the call.
	using TextStyler = std::function<std::span<const TextSpan>(std::string_view)>;

	namespace detail
	{
		inline DMUI_Result DMUI_CALL StyleTextField(
			void* a_userData,
			const char* a_text,
			size_t a_length,
			const DMUI_TextSpan** a_spans,
			size_t* a_count) noexcept
		{
			try
			{
				const auto spans = (*static_cast<const TextStyler*>(a_userData))(
					std::string_view{ a_text, a_length });
				*a_spans = spans.data();
				*a_count = spans.size();
				return DMUI_RESULT_OK;
			}
			catch (...)
			{
				return DMUI_RESULT_CALLBACK_FAILED;
			}
		}
	}

	// Non-owning; the DMUI_TextFieldOptions comment defines each field.
	struct TextFieldOptions
	{
		const char* hint{};
		TextFieldStyle style{ TextFieldStyle::kPlain };
		InputTextFlags flags{};
		TextEditFlags editFlags{};
		// Zero is unlimited.
		uint32_t maxBytes{};
		// Zero with a zero size.y is a single line.
		uint32_t lines{};
		Vec2 size{};
		size_t cursor{};
		// Single line only; colors the text in the monospace role.
		const TextStyler* styler{};

		[[nodiscard]] constexpr DMUI_TextFieldOptions Abi() const noexcept
		{
			return {
				hint,
				cursor,
				static_cast<DMUI_TextFieldStyle>(style),
				static_cast<uint32_t>(flags),
				static_cast<uint32_t>(editFlags),
				maxBytes,
				lines,
				0u,
				size,
				styler ? &detail::StyleTextField : nullptr,
				const_cast<TextStyler*>(styler)
			};
		}
	};

	using TextEditState = DMUI_TextEditState;

	[[nodiscard]] constexpr bool HasTextEditEvent(
		const TextEditState& a_state,
		TextEditEvents a_event) noexcept
	{
		return (a_state.events & static_cast<uint32_t>(a_event)) != 0;
	}

	// The one text field over a client buffer; returns whether the text was edited
	// and reports the per-frame edit events in a_state.
	[[nodiscard]] inline bool InputText(
		const char* a_id,
		DMUI_TextBuffer& a_buffer,
		const TextFieldOptions& a_options,
		TextEditState& a_state) noexcept
	{
		a_state = {};
		const auto options = a_options.Abi();
		detail::Record(checked::InputText(a_id, &a_buffer, &options, &a_state));
		return HasTextEditEvent(a_state, TextEditEvents::kEdited);
	}

	[[nodiscard]] inline bool InputText(
		const char* a_id,
		std::string& a_text,
		const TextFieldOptions& a_options,
		TextEditState& a_state) noexcept
	{
		a_state = {};
		TextInputBuffer buffer{ a_text };
		auto* const data = buffer.Get();
		if (!data)
		{
			detail::Record(buffer.Result());
			return false;
		}
		const auto edited = InputText(a_id, *data, a_options, a_state);
		detail::Record(buffer.CommitTo(a_text));
		return edited;
	}

	// The convenient form: edits a string that grows as needed.
	[[nodiscard]] inline bool InputText(
		const char* a_id,
		std::string& a_text,
		const TextFieldOptions& a_options = {}) noexcept
	{
		TextEditState state;
		return InputText(a_id, a_text, a_options, state);
	}

	// True on the frame the keyboard chord ("Ctrl+S", host hotkey grammar) is
	// pressed, only for the page whose window has keyboard focus. An active text
	// field keeps its own keys, and chords a host hotkey acts on never fire.
	[[nodiscard]] inline bool Shortcut(
		const char* a_chord,
		ShortcutFlags a_flags = ShortcutFlags::kNone) noexcept
	{
		uint32_t pressed{};
		detail::Record(checked::Shortcut(
			a_chord, static_cast<DMUI_UIShortcutFlags>(a_flags), &pressed));
		return pressed != 0;
	}

	[[nodiscard]] inline bool IsItemDeactivatedAfterEdit() noexcept
	{
		uint32_t deactivated{};
		detail::Record(checked::IsItemDeactivatedAfterEdit(&deactivated));
		return deactivated != 0;
	}

	[[nodiscard]] inline bool IsItemHovered(
		HoveredFlags a_flags = HoveredFlags::kNone) noexcept
	{
		uint32_t hovered{};
		detail::Record(checked::IsItemHovered(
			static_cast<DMUI_UIHoveredFlags>(a_flags),
			&hovered));
		return hovered != 0;
	}

	// Opens the last item's tooltip under the host policy: after the theme hover
	// delay or on navigation focus, even while the item is disabled.
	[[nodiscard]] inline bool BeginItemTooltip() noexcept
	{
		uint32_t visible{};
		detail::Record(checked::BeginItemTooltip(&visible));
		return visible != 0;
	}

	inline void PopID() noexcept
	{
		detail::Record(checked::PopID());
	}

	inline void PopTextWrapPos() noexcept
	{
		detail::Record(checked::PopTextWrapPos());
	}

	inline void ProgressBar(
		float a_fraction,
		Vec2 a_size = { -FLT_MIN, 0.0f },
		const char* a_overlay = nullptr) noexcept
	{
		const auto overlay = a_overlay ? std::string_view{ a_overlay } : std::string_view{};
		detail::Record(checked::ProgressBar(
			a_fraction,
			a_size,
			detail::TextData(overlay),
			overlay.size()));
	}

	inline void PushID(const char* a_id) noexcept
	{
		detail::Record(checked::PushIDString(a_id));
	}

	inline void PushID(const char* a_begin, const char* a_end) noexcept
	{
		const char* text{};
		size_t length{};
		if (!detail::TextRange(a_begin, a_end, text, length))
		{
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return;
		}
		detail::Record(checked::PushIDRange(text, length));
	}

	inline void PushID(const void* a_id) noexcept
	{
		detail::Record(checked::PushIDValue(
			static_cast<uint64_t>(reinterpret_cast<uintptr_t>(a_id))));
	}

	inline void PushID(int32_t a_id) noexcept
	{
		detail::Record(checked::PushIDValue(
			static_cast<uint64_t>(static_cast<int64_t>(a_id))));
	}

	inline void PushTextWrapPos(float a_localX = 0.0f) noexcept
	{
		detail::Record(checked::PushTextWrapPos(a_localX));
	}

	inline void SameLine(
		float a_offsetFromStartX = 0.0f,
		float a_spacing = -1.0f) noexcept
	{
		detail::Record(checked::SameLine(a_offsetFromStartX, a_spacing));
	}

	[[nodiscard]] inline bool Selectable(
		const char* a_label,
		bool a_selected = false,
		SelectableFlags a_flags = SelectableFlags::kNone,
		Vec2 a_size = {}) noexcept
	{
		uint32_t pressed{};
		detail::Record(checked::Selectable(
			a_label,
			a_selected ? 1u : 0u,
			static_cast<DMUI_UISelectableFlags>(a_flags),
			a_size,
			&pressed));
		return pressed != 0;
	}

	[[nodiscard]] inline bool Selectable(
		const char* a_label,
		bool* a_selected,
		SelectableFlags a_flags = SelectableFlags::kNone,
		Vec2 a_size = {}) noexcept
	{
		if (!a_selected)
		{
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return false;
		}
		uint32_t selected = *a_selected ? 1u : 0u;
		uint32_t pressed{};
		const auto result = checked::SelectableToggle(
			a_label,
			&selected,
			static_cast<DMUI_UISelectableFlags>(a_flags),
			a_size,
			&pressed);
		detail::Record(result);
		if (result == DMUI_RESULT_OK)
			*a_selected = selected != 0;
		return pressed != 0;
	}

	inline void Separator() noexcept
	{
		detail::Record(checked::Separator());
	}

	inline void SetClipboardText(std::string_view a_text) noexcept
	{
		detail::Record(checked::SetClipboardText(
			detail::TextData(a_text),
			a_text.size()));
	}

	inline void SetClipboardText(const char* a_text) noexcept
	{
		if (!a_text)
		{
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return;
		}
		SetClipboardText(std::string_view{ a_text });
	}

	inline void SetCursorScreenPos(Vec2 a_position) noexcept
	{
		detail::Record(checked::SetCursorScreenPos(a_position));
	}

	[[nodiscard]] inline float GetCursorScreenPosX() noexcept { return GetCursorScreenPos().x; }
	[[nodiscard]] inline float GetCursorScreenPosY() noexcept { return GetCursorScreenPos().y; }
	inline void SetCursorScreenPosX(float a_x) noexcept
	{
		SetCursorScreenPos({ a_x, GetCursorScreenPosY() });
	}
	inline void SetCursorScreenPosY(float a_y) noexcept
	{
		SetCursorScreenPos({ GetCursorScreenPosX(), a_y });
	}

	inline void SetItemDefaultFocus() noexcept
	{
		detail::Record(checked::SetItemDefaultFocus());
	}

	inline void SetNextItemWidth(float a_width) noexcept
	{
		detail::Record(checked::SetNextItemWidth(a_width));
	}

	namespace detail
	{
		[[nodiscard]] inline DMUI_Result PlainText(
			const char* a_text, size_t a_length) noexcept
		{
			return checked::Text(a_text, a_length, nullptr, 0u, nullptr);
		}

		inline void FormattedText(
			const char* a_format,
			std::va_list a_arguments) noexcept
		{
			std::vector<char> text;
			if (!Format(a_format, a_arguments, text))
				return;
			Record(PlainText(text.data(), text.size() - 1u));
		}
	}

	inline void Spacing() noexcept
	{
		detail::Record(checked::Spacing());
	}

	inline void TableHeadersRow() noexcept
	{
		detail::Record(checked::TableHeadersRow());
	}

	[[nodiscard]] inline bool TableNextColumn() noexcept
	{
		uint32_t visible{};
		detail::Record(checked::TableNextColumn(&visible));
		return visible != 0;
	}

	inline void TableNextRow(
		TableRowFlags a_flags = TableRowFlags::kNone,
		float a_minimumHeight = 0.0f) noexcept
	{
		detail::Record(checked::TableNextRow(
			static_cast<DMUI_UITableRowFlags>(a_flags),
			a_minimumHeight));
	}

	[[nodiscard]] inline bool TableSetColumnIndex(int32_t a_column) noexcept
	{
		uint32_t visible{};
		detail::Record(checked::TableSetColumnIndex(a_column, &visible));
		return visible != 0;
	}

	inline void TableSetupColumn(
		const char* a_label,
		TableColumnFlags a_flags = TableColumnFlags::kNone,
		float a_initialWidthOrWeight = 0.0f,
		ID a_userId = 0) noexcept
	{
		detail::Record(checked::TableSetupColumn(
			a_label,
			static_cast<DMUI_UITableColumnFlags>(a_flags),
			a_initialWidthOrWeight,
			a_userId));
	}

	inline void TableSetupScrollFreeze(
		int32_t a_columns,
		int32_t a_rows) noexcept
	{
		detail::Record(checked::TableSetupScrollFreeze(a_columns, a_rows));
	}

	inline void TextUnformatted(
		std::string_view a_text) noexcept
	{
		detail::Record(detail::PlainText(detail::TextData(a_text), a_text.size()));
	}

	inline void TextUnformatted(
		const char* a_text,
		const char* a_textEnd = nullptr) noexcept
	{
		const char* text{};
		size_t length{};
		if (!detail::TextRange(a_text, a_textEnd, text, length))
		{
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return;
		}
		detail::Record(detail::PlainText(text, length));
	}

	inline void Text(const char* a_format, ...) noexcept
	{
		std::va_list arguments;
		va_start(arguments, a_format);
		detail::FormattedText(a_format, arguments);
		va_end(arguments);
	}

	inline void Unindent(float a_width = 0.0f) noexcept
	{
		detail::Record(checked::Unindent(a_width));
	}

	inline void PushStyleVar(StyleVar a_styleVar, float a_value) noexcept
	{
		detail::Record(checked::PushStyleVarFloat(
			static_cast<DMUI_UIStyleVar>(a_styleVar),
			a_value));
	}

	inline void PushStyleVar(StyleVar a_styleVar, Vec2 a_value) noexcept
	{
		detail::Record(checked::PushStyleVarVec2(
			static_cast<DMUI_UIStyleVar>(a_styleVar),
			a_value));
	}

	inline void PopStyleVar(int32_t a_count = 1) noexcept
	{
		detail::Record(checked::PopStyleVar(a_count));
	}

	inline void NewLine() noexcept
	{
		detail::Record(checked::NewLine());
	}

	inline void PlotLines(
		const char* a_label,
		const float* a_values,
		int32_t a_valueCount,
		int32_t a_valueOffset = 0,
		const char* a_overlay = nullptr,
		float a_scaleMinimum = FLT_MAX,
		float a_scaleMaximum = FLT_MAX,
		Vec2 a_size = {},
		uint32_t a_strideBytes = sizeof(float)) noexcept
	{
		const auto overlay = a_overlay ? std::string_view{ a_overlay } : std::string_view{};
		detail::Record(checked::PlotLines(
			a_label,
			a_values,
			a_valueCount,
			a_valueOffset,
			detail::TextData(overlay),
			overlay.size(),
			a_scaleMinimum,
			a_scaleMaximum,
			a_size,
			a_strideBytes));
	}
}

#include <DearModdingUI/UI/Colors.h>
#include <DearModdingUI/UI/DrawList.h>
