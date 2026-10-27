#pragma once

#include <DearModdingUI/API.h>

#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>

namespace dmui
{
	// The storage classes shared by settings values and the host numeric primitive.
	template <class T>
	concept NumericSettingValue =
		std::same_as<std::remove_cvref_t<T>, double> ||
		std::same_as<std::remove_cvref_t<T>, int64_t> ||
		std::same_as<std::remove_cvref_t<T>, uint64_t>;

	template <NumericSettingValue T>
	struct NumericSettingRange
	{
		std::optional<T> minimum;
		std::optional<T> maximum;
	};

	template <NumericSettingValue T>
	struct NumericQuantization
	{
		T interval;
		T origin;
	};

	template <NumericSettingValue T>
	[[nodiscard]] inline T ClampSettingNumber(
		T a_value,
		T a_default,
		const std::optional<NumericSettingRange<T>>& a_range) noexcept
	{
		if constexpr (std::same_as<T, double>)
		{
			if (!std::isfinite(a_value))
				a_value = std::isfinite(a_default) ? a_default : 0.0;
		}
		if (!a_range)
			return a_value;

		auto minimum = a_range->minimum;
		auto maximum = a_range->maximum;
		if constexpr (std::same_as<T, double>)
		{
			if (minimum && !std::isfinite(*minimum))
				minimum.reset();
			if (maximum && !std::isfinite(*maximum))
				maximum.reset();
		}
		if (minimum && maximum && *maximum < *minimum)
			std::swap(minimum, maximum);
		if (minimum)
			a_value = (std::max)(a_value, *minimum);
		if (maximum)
			a_value = (std::min)(a_value, *maximum);
		return a_value;
	}

	template <NumericSettingValue T>
	[[nodiscard]] inline T QuantizeSettingNumber(
		T a_value,
		const std::optional<NumericQuantization<T>>& a_quantization) noexcept
	{
		if (!a_quantization)
			return a_value;
		const auto interval = a_quantization->interval;
		const auto origin = a_quantization->origin;
		if constexpr (std::same_as<T, double>)
		{
			if (!std::isfinite(a_value) ||
				!std::isfinite(interval) ||
				!std::isfinite(origin) ||
				interval <= 0.0)
				return a_value;
			const auto quantized =
				origin + std::round((a_value - origin) / interval) * interval;
			return std::isfinite(quantized) ? quantized : a_value;
		}
		else
		{
			if (interval <= T{})
				return a_value;
			using Unsigned = std::make_unsigned_t<T>;
			const auto unsignedInterval = static_cast<Unsigned>(interval);
			const auto above = a_value >= origin;
			const auto distance = above ?
				static_cast<Unsigned>(a_value) - static_cast<Unsigned>(origin) :
				static_cast<Unsigned>(origin) - static_cast<Unsigned>(a_value);
			auto steps = distance / unsignedInterval;
			const auto remainder = distance % unsignedInterval;
			if (remainder >= unsignedInterval - remainder)
				++steps;
			const auto room = above ?
				static_cast<Unsigned>((std::numeric_limits<T>::max)()) -
					static_cast<Unsigned>(origin) :
				static_cast<Unsigned>(origin) -
					static_cast<Unsigned>((std::numeric_limits<T>::min)());
			if (steps > room / unsignedInterval)
			{
				return above ?
					(std::numeric_limits<T>::max)() :
					(std::numeric_limits<T>::min)();
			}
			const auto offset = steps * unsignedInterval;
			return above ?
				static_cast<T>(static_cast<Unsigned>(origin) + offset) :
				static_cast<T>(static_cast<Unsigned>(origin) - offset);
		}
	}

	enum class NumericType : uint32_t
	{
		kS8 = DMUI_NUMERIC_TYPE_S8,
		kU8 = DMUI_NUMERIC_TYPE_U8,
		kS16 = DMUI_NUMERIC_TYPE_S16,
		kU16 = DMUI_NUMERIC_TYPE_U16,
		kS32 = DMUI_NUMERIC_TYPE_S32,
		kU32 = DMUI_NUMERIC_TYPE_U32,
		kS64 = DMUI_NUMERIC_TYPE_S64,
		kU64 = DMUI_NUMERIC_TYPE_U64,
		kFloat = DMUI_NUMERIC_TYPE_FLOAT,
		kDouble = DMUI_NUMERIC_TYPE_DOUBLE
	};

	// Chosen by the caller; nothing is inferred from which bounds are set.
	enum class NumericPresentation : uint32_t
	{
		// Needs both bounds.
		kSlider = DMUI_NUMERIC_PRESENTATION_SLIDER,
		kDrag = DMUI_NUMERIC_PRESENTATION_DRAG,
		kInput = DMUI_NUMERIC_PRESENTATION_INPUT
	};

	template <class T>
	concept NumericScalar =
		std::same_as<T, int8_t> || std::same_as<T, uint8_t> ||
		std::same_as<T, int16_t> || std::same_as<T, uint16_t> ||
		std::same_as<T, int32_t> || std::same_as<T, uint32_t> ||
		std::same_as<T, int64_t> || std::same_as<T, uint64_t> ||
		std::same_as<T, float> || std::same_as<T, double>;

	template <NumericScalar T>
	[[nodiscard]] constexpr NumericType NumericTypeOf() noexcept
	{
		if constexpr (std::same_as<T, int8_t>)
			return NumericType::kS8;
		else if constexpr (std::same_as<T, uint8_t>)
			return NumericType::kU8;
		else if constexpr (std::same_as<T, int16_t>)
			return NumericType::kS16;
		else if constexpr (std::same_as<T, uint16_t>)
			return NumericType::kU16;
		else if constexpr (std::same_as<T, int32_t>)
			return NumericType::kS32;
		else if constexpr (std::same_as<T, uint32_t>)
			return NumericType::kU32;
		else if constexpr (std::same_as<T, int64_t>)
			return NumericType::kS64;
		else if constexpr (std::same_as<T, uint64_t>)
			return NumericType::kU64;
		else if constexpr (std::same_as<T, float>)
			return NumericType::kFloat;
		else
			return NumericType::kDouble;
	}

	template <NumericScalar T>
	[[nodiscard]] constexpr DMUI_NumericValue ToNumericValue(T a_value) noexcept
	{
		DMUI_NumericValue value{};
		if constexpr (std::floating_point<T>)
			value.f = static_cast<double>(a_value);
		else if constexpr (std::is_signed_v<T>)
			value.i = static_cast<int64_t>(a_value);
		else
			value.u = static_cast<uint64_t>(a_value);
		return value;
	}

	template <NumericScalar T>
	[[nodiscard]] constexpr T FromNumericValue(const DMUI_NumericValue& a_value) noexcept
	{
		if constexpr (std::floating_point<T>)
			return static_cast<T>(a_value.f);
		else if constexpr (std::is_signed_v<T>)
			return static_cast<T>(a_value.i);
		else
			return static_cast<T>(a_value.u);
	}

	// Non-owning; the DMUI_NumericDescriptor comment defines each field.
	template <NumericScalar T>
	struct NumericOptions
	{
		const char* id{};
		NumericPresentation presentation{ NumericPresentation::kInput };
		T value{};
		std::optional<T> minimum;
		std::optional<T> maximum;
		// Results snap to stepOrigin + n * step.
		std::optional<T> step;
		T stepOrigin{};
		const char* format{};
		const char* unit{};
		// Keeps every result inside the present bounds.
		bool clamp{};
		float width{};
		float dragSpeed{};

		[[nodiscard]] DMUI_NumericDescriptor Abi() const noexcept
		{
			DMUI_NumericDescriptor descriptor{};
			descriptor.id = id;
			descriptor.format = format;
			descriptor.unit = unit;
			descriptor.type = static_cast<DMUI_NumericType>(NumericTypeOf<T>());
			descriptor.presentation = static_cast<DMUI_NumericPresentation>(presentation);
			descriptor.flags =
				(minimum ? DMUI_NUMERIC_HAS_MINIMUM : 0u) |
				(maximum ? DMUI_NUMERIC_HAS_MAXIMUM : 0u) |
				(step ? DMUI_NUMERIC_HAS_STEP : 0u) |
				(clamp ? DMUI_NUMERIC_CLAMP : 0u);
			descriptor.width = width;
			descriptor.dragSpeed = dragSpeed;
			descriptor.value = ToNumericValue<T>(value);
			descriptor.minimum = ToNumericValue<T>(minimum.value_or(T{}));
			descriptor.maximum = ToNumericValue<T>(maximum.value_or(T{}));
			descriptor.step = ToNumericValue<T>(step.value_or(T{}));
			descriptor.stepOrigin = ToNumericValue<T>(stepOrigin);
			return descriptor;
		}
	};

	template <NumericScalar T>
	struct NumericResult
	{
		T value{};
		bool changed{};
		bool completed{};
	};
}
