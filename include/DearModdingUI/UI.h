#pragma once

#include <DearModdingUI/CUIAPI.h>

#include <cfloat>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <new>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <vector>

namespace dmui::ui::detail
{
	struct Context
	{
		const DMUI_UIAPI* api{};
		DMUI_ClientHandle client{ DMUI_INVALID_CLIENT_HANDLE };
		DMUI_Result result{ DMUI_RESULT_OK };
	};

	inline thread_local Context* currentContext{};

	class ScopedContext
	{
	public:
		ScopedContext(
			const DMUI_UIAPI* a_api,
			DMUI_ClientHandle a_client) noexcept :
			m_previous(currentContext),
			m_context{ a_api, a_client, DMUI_RESULT_OK }
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
		if (a_result != DMUI_RESULT_OK &&
			currentContext &&
			currentContext->result == DMUI_RESULT_OK)
			currentContext->result = a_result;
	}

	[[nodiscard]] inline DMUI_Result LastResult() noexcept
	{
		return currentContext ?
			currentContext->result :
			DMUI_RESULT_HOST_NOT_READY;
	}

	inline void ClearError() noexcept
	{
		if (currentContext)
			currentContext->result = DMUI_RESULT_OK;
	}

	template <class Function, class... Arguments>
	[[nodiscard]] DMUI_Result Invoke(
		uint32_t a_requiredSize,
		Function DMUI_UIAPI::*a_member,
		Arguments... a_arguments) noexcept
	{
		const auto* context = currentContext;
		if (!context ||
			!context->api ||
			context->client == DMUI_INVALID_CLIENT_HANDLE)
			return DMUI_RESULT_HOST_NOT_READY;
		if (context->api->structSize < a_requiredSize)
			return DMUI_RESULT_UNSUPPORTED_ABI;
		const auto function = context->api->*a_member;
		if (!function)
			return DMUI_RESULT_UNSUPPORTED_ABI;
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
		const auto size = a_metrics.structSize;
		a_metrics = {};
		a_metrics.structSize = size ? size : sizeof(a_metrics);
		const auto result = checked::GetStyleMetrics(&a_metrics);
		detail::Record(result);
		return result;
	}

	[[nodiscard]] inline bool BeginCombo(
		const char* a_label,
		const char* a_previewValue,
		ComboFlags a_flags = ComboFlags::kNone) noexcept
	{
		uint32_t visible{};
		detail::Record(checked::BeginCombo(
			a_label,
			a_previewValue,
			static_cast<DMUI_UIComboFlags>(a_flags),
			&visible));
		return visible != 0;
	}

	inline void EndCombo() noexcept
	{
		detail::Record(checked::EndCombo());
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

	inline void EndTooltip() noexcept
	{
		detail::Record(checked::EndTooltip());
	}

	[[nodiscard]] inline bool Button(
		const char* a_label,
		Vec2 a_size = {}) noexcept
	{
		uint32_t pressed{};
		detail::Record(checked::Button(a_label, a_size, &pressed));
		return pressed != 0;
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

	[[nodiscard]] inline bool Checkbox(
		const char* a_label,
		bool* a_value) noexcept
	{
		if (!a_value)
		{
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return false;
		}
		uint32_t value = *a_value ? 1u : 0u;
		uint32_t changed{};
		const auto result = checked::Checkbox(a_label, &value, &changed);
		detail::Record(result);
		if (result == DMUI_RESULT_OK)
			*a_value = value != 0;
		return changed != 0;
	}

	[[nodiscard]] inline bool CollapsingHeader(
		const char* a_label,
		TreeNodeFlags a_flags = TreeNodeFlags::kNone) noexcept
	{
		uint32_t open{};
		detail::Record(checked::CollapsingHeader(
			a_label,
			static_cast<DMUI_UITreeNodeFlags>(a_flags),
			&open));
		return open != 0;
	}

	[[nodiscard]] inline bool CollapsingHeader(
		const char* a_label,
		bool* a_visible,
		TreeNodeFlags a_flags = TreeNodeFlags::kNone) noexcept
	{
		if (!a_visible)
		{
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return false;
		}
		uint32_t visible = *a_visible ? 1u : 0u;
		uint32_t open{};
		const auto result = checked::CollapsingHeaderVisible(
			a_label,
			&visible,
			static_cast<DMUI_UITreeNodeFlags>(a_flags),
			&open);
		detail::Record(result);
		if (result == DMUI_RESULT_OK)
			*a_visible = visible != 0;
		return open != 0;
	}

	namespace detail
	{
		template <class T>
		struct ScalarType;

#define DMUI_UI_SCALAR(cpp_type, ui_type) \
	template <>                            \
	struct ScalarType<cpp_type>            \
	{                                      \
		static constexpr DataType value{ DataType::ui_type }; \
	}

		DMUI_UI_SCALAR(int8_t, kS8);
		DMUI_UI_SCALAR(uint8_t, kU8);
		DMUI_UI_SCALAR(int16_t, kS16);
		DMUI_UI_SCALAR(uint16_t, kU16);
		DMUI_UI_SCALAR(int32_t, kS32);
		DMUI_UI_SCALAR(uint32_t, kU32);
		DMUI_UI_SCALAR(int64_t, kS64);
		DMUI_UI_SCALAR(uint64_t, kU64);
		DMUI_UI_SCALAR(float, kFloat);
		DMUI_UI_SCALAR(double, kDouble);

#undef DMUI_UI_SCALAR

		template <class T>
		concept Scalar =
			requires { ScalarType<std::remove_cv_t<T>>::value; };

		template <Scalar T>
		[[nodiscard]] constexpr uint32_t ScalarSize() noexcept
		{
			return static_cast<uint32_t>(sizeof(T));
		}
	}

	template <detail::Scalar T>
	[[nodiscard]] inline bool DragScalar(
		const char* a_label,
		T* a_data,
		float a_speed = 1.0f,
		const T* a_minimum = nullptr,
		const T* a_maximum = nullptr,
		const char* a_format = nullptr,
		SliderFlags a_flags = SliderFlags::kNone) noexcept
	{
		uint32_t changed{};
		detail::Record(checked::DragScalar(
			a_label,
			static_cast<DMUI_UIDataType>(
				detail::ScalarType<std::remove_cv_t<T>>::value),
			a_data,
			detail::ScalarSize<T>(),
			a_speed,
			a_minimum,
			a_minimum ? detail::ScalarSize<T>() : 0u,
			a_maximum,
			a_maximum ? detail::ScalarSize<T>() : 0u,
			a_format,
			static_cast<DMUI_UISliderFlags>(a_flags),
			&changed));
		return changed != 0;
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

	template <detail::Scalar T>
	[[nodiscard]] inline bool InputScalar(
		const char* a_label,
		T* a_data,
		const T* a_step = nullptr,
		const T* a_fastStep = nullptr,
		const char* a_format = nullptr,
		InputTextFlags a_flags = InputTextFlags::kNone) noexcept
	{
		uint32_t changed{};
		detail::Record(checked::InputScalar(
			a_label,
			static_cast<DMUI_UIDataType>(
				detail::ScalarType<std::remove_cv_t<T>>::value),
			a_data,
			detail::ScalarSize<T>(),
			a_step,
			a_step ? detail::ScalarSize<T>() : 0u,
			a_fastStep,
			a_fastStep ? detail::ScalarSize<T>() : 0u,
			a_format,
			static_cast<DMUI_UIInputTextFlags>(a_flags),
			&changed));
		return changed != 0;
	}

	namespace detail
	{
		[[nodiscard]] inline bool InputCapacity(
			size_t a_capacity,
			uint32_t& a_result) noexcept
		{
			if (a_capacity == 0 ||
				a_capacity > (std::numeric_limits<uint32_t>::max)())
			{
				Record(DMUI_RESULT_INVALID_ARGUMENT);
				return false;
			}
			a_result = static_cast<uint32_t>(a_capacity);
			return true;
		}
	}

	[[nodiscard]] inline bool InputText(
		const char* a_label,
		char* a_buffer,
		size_t a_capacity,
		InputTextFlags a_flags = InputTextFlags::kNone) noexcept
	{
		uint32_t capacity{};
		if (!detail::InputCapacity(a_capacity, capacity))
			return false;
		uint32_t changed{};
		detail::Record(checked::InputText(
			a_label,
			a_buffer,
			capacity,
			static_cast<DMUI_UIInputTextFlags>(a_flags),
			&changed));
		return changed != 0;
	}

	[[nodiscard]] inline bool InputTextMultiline(
		const char* a_label,
		char* a_buffer,
		size_t a_capacity,
		Vec2 a_size = {},
		InputTextFlags a_flags = InputTextFlags::kNone) noexcept
	{
		uint32_t capacity{};
		if (!detail::InputCapacity(a_capacity, capacity))
			return false;
		uint32_t changed{};
		detail::Record(checked::InputTextMultiline(
			a_label,
			a_buffer,
			capacity,
			a_size,
			static_cast<DMUI_UIInputTextFlags>(a_flags),
			&changed));
		return changed != 0;
	}

	[[nodiscard]] inline bool InputTextWithHint(
		const char* a_label,
		const char* a_hint,
		char* a_buffer,
		size_t a_capacity,
		InputTextFlags a_flags = InputTextFlags::kNone) noexcept
	{
		uint32_t capacity{};
		if (!detail::InputCapacity(a_capacity, capacity))
			return false;
		uint32_t changed{};
		detail::Record(checked::InputTextWithHint(
			a_label,
			a_hint,
			a_buffer,
			capacity,
			static_cast<DMUI_UIInputTextFlags>(a_flags),
			&changed));
		return changed != 0;
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

	inline void PopID() noexcept
	{
		detail::Record(checked::PopID());
	}

	inline void PopStyleColor(int32_t a_count = 1) noexcept
	{
		detail::Record(checked::PopStyleColor(a_count));
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

	inline void PushStyleColor(Color a_color, Color32 a_rgba) noexcept
	{
		detail::Record(checked::PushStyleColorU32(
			static_cast<DMUI_UIColor>(a_color),
			a_rgba));
	}

	inline void PushStyleColor(Color a_color, Vec4 a_value) noexcept
	{
		detail::Record(checked::PushStyleColor(
			static_cast<DMUI_UIColor>(a_color),
			a_value));
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
		inline void FormattedText(
			DMUI_Result (*a_function)(const char*, size_t) noexcept,
			const char* a_format,
			std::va_list a_arguments) noexcept
		{
			std::vector<char> text;
			if (!Format(a_format, a_arguments, text))
				return;
			Record(a_function(text.data(), text.size() - 1u));
		}
	}

	inline void SetTooltip(const char* a_format, ...) noexcept
	{
		std::va_list arguments;
		va_start(arguments, a_format);
		detail::FormattedText(&checked::SetTooltipText, a_format, arguments);
		va_end(arguments);
	}

	template <detail::Scalar T>
	[[nodiscard]] inline bool SliderScalar(
		const char* a_label,
		T* a_data,
		const T* a_minimum,
		const T* a_maximum,
		const char* a_format = nullptr,
		SliderFlags a_flags = SliderFlags::kNone) noexcept
	{
		uint32_t changed{};
		detail::Record(checked::SliderScalar(
			a_label,
			static_cast<DMUI_UIDataType>(
				detail::ScalarType<std::remove_cv_t<T>>::value),
			a_data,
			detail::ScalarSize<T>(),
			a_minimum,
			a_minimum ? detail::ScalarSize<T>() : 0u,
			a_maximum,
			a_maximum ? detail::ScalarSize<T>() : 0u,
			a_format,
			static_cast<DMUI_UISliderFlags>(a_flags),
			&changed));
		return changed != 0;
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
		detail::Record(checked::Text(
			detail::TextData(a_text),
			a_text.size()));
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
		detail::Record(checked::Text(text, length));
	}

	namespace detail
	{
		inline void FormattedColoredText(
			Vec4 a_color,
			const char* a_format,
			std::va_list a_arguments) noexcept
		{
			std::vector<char> text;
			if (!Format(a_format, a_arguments, text))
				return;
			Record(checked::TextColored(
				a_color,
				text.data(),
				text.size() - 1u));
		}
	}

	inline void Text(const char* a_format, ...) noexcept
	{
		std::va_list arguments;
		va_start(arguments, a_format);
		detail::FormattedText(&checked::Text, a_format, arguments);
		va_end(arguments);
	}

	inline void TextColored(
		Vec4 a_color,
		const char* a_format,
		...) noexcept
	{
		std::va_list arguments;
		va_start(arguments, a_format);
		detail::FormattedColoredText(a_color, a_format, arguments);
		va_end(arguments);
	}

	inline void TextDisabled(const char* a_format, ...) noexcept
	{
		std::va_list arguments;
		va_start(arguments, a_format);
		detail::FormattedText(&checked::TextDisabled, a_format, arguments);
		va_end(arguments);
	}

	inline void TextWrapped(const char* a_format, ...) noexcept
	{
		std::va_list arguments;
		va_start(arguments, a_format);
		detail::FormattedText(&checked::TextWrapped, a_format, arguments);
		va_end(arguments);
	}

	inline void Unindent(float a_width = 0.0f) noexcept
	{
		detail::Record(checked::Unindent(a_width));
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
