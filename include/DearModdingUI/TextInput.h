#pragma once

#include <DearModdingUI/API.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <new>
#include <optional>
#include <string>
#include <string_view>

namespace dmui
{
	class TextInputBuffer
	{
	public:
		explicit TextInputBuffer(
			std::string_view a_text,
			std::optional<size_t> a_maximumBytes = std::nullopt) noexcept
		{
			if (a_text.find('\0') != std::string_view::npos)
			{
				result_ = DMUI_RESULT_INVALID_ARGUMENT;
				return;
			}

			constexpr auto maximumCapacity =
				static_cast<size_t>((std::numeric_limits<int>::max)());
			if ((a_maximumBytes &&
					(*a_maximumBytes >= maximumCapacity ||
						a_text.size() > *a_maximumBytes)) ||
				(!a_maximumBytes && a_text.size() >= maximumCapacity))
			{
				result_ = DMUI_RESULT_INVALID_ARGUMENT;
				return;
			}

			const auto initialCapacity = a_maximumBytes ?
				*a_maximumBytes + 1 :
				(std::max)(a_text.size() + 1, kInitialCapacity);
			try
			{
				storage_.assign(a_text);
				storage_.resize(initialCapacity, '\0');
			}
			catch (const std::bad_alloc&)
			{
				result_ = DMUI_RESULT_RESOURCE_EXHAUSTED;
				return;
			}
			catch (...)
			{
				result_ = DMUI_RESULT_CALLBACK_FAILED;
				return;
			}

			buffer_ = {
				sizeof(DMUI_TextBuffer),
				storage_.data(),
				storage_.size(),
				a_maximumBytes ? nullptr : &ResizeCallback,
				a_maximumBytes ? nullptr : this
			};
		}

		TextInputBuffer(const TextInputBuffer&) = delete;
		TextInputBuffer(TextInputBuffer&&) = delete;
		TextInputBuffer& operator=(const TextInputBuffer&) = delete;
		TextInputBuffer& operator=(TextInputBuffer&&) = delete;

		[[nodiscard]] DMUI_Result Result() const noexcept
		{
			return result_;
		}

		[[nodiscard]] DMUI_TextBuffer* Get() noexcept
		{
			return result_ == DMUI_RESULT_OK ? &buffer_ : nullptr;
		}

		[[nodiscard]] DMUI_Result CommitTo(std::string& a_text) noexcept
		{
			if (result_ != DMUI_RESULT_OK)
				return result_;
			if (buffer_.structSize < DMUI_TEXT_BUFFER_0_2_SIZE ||
				buffer_.data != storage_.data() ||
				buffer_.capacity != storage_.size())
				return DMUI_RESULT_INVALID_DESCRIPTOR;

			const auto terminator =
				(std::find)(storage_.begin(), storage_.end(), '\0');
			if (terminator == storage_.end())
				return DMUI_RESULT_INVALID_DESCRIPTOR;
			storage_.resize(
				static_cast<size_t>(terminator - storage_.begin()));
			a_text.swap(storage_);
			buffer_ = {};
			return DMUI_RESULT_OK;
		}

	private:
		static constexpr size_t kInitialCapacity{ 64 };

		static DMUI_Result DMUI_CALL ResizeCallback(
			void* a_userData,
			size_t a_minimumCapacity,
			char** a_data,
			size_t* a_capacity) noexcept
		{
			if (!a_userData)
				return DMUI_RESULT_INVALID_ARGUMENT;
			return static_cast<TextInputBuffer*>(a_userData)->Resize(
				a_minimumCapacity,
				a_data,
				a_capacity);
		}

		[[nodiscard]] DMUI_Result Resize(
			size_t a_minimumCapacity,
			char** a_data,
			size_t* a_capacity) noexcept
		{
			if (result_ != DMUI_RESULT_OK)
				return result_;
			if (!a_data || !a_capacity)
				return RecordResizeFailure(DMUI_RESULT_INVALID_ARGUMENT);

			constexpr auto maximumCapacity =
				static_cast<size_t>((std::numeric_limits<int>::max)());
			if (a_minimumCapacity == 0 ||
				a_minimumCapacity > maximumCapacity)
				return RecordResizeFailure(DMUI_RESULT_INVALID_ARGUMENT);
			if (a_minimumCapacity <= storage_.size())
			{
				*a_data = storage_.data();
				*a_capacity = storage_.size();
				return DMUI_RESULT_OK;
			}

			const auto grownCapacity =
				storage_.size() > maximumCapacity / 2 ?
					maximumCapacity :
					storage_.size() * 2;
			const auto newCapacity =
				(std::max)(a_minimumCapacity, grownCapacity);
			try
			{
				storage_.resize(newCapacity, '\0');
			}
			catch (const std::bad_alloc&)
			{
				return RecordResizeFailure(DMUI_RESULT_RESOURCE_EXHAUSTED);
			}
			catch (...)
			{
				return RecordResizeFailure(DMUI_RESULT_CALLBACK_FAILED);
			}

			buffer_.data = storage_.data();
			buffer_.capacity = storage_.size();
			*a_data = buffer_.data;
			*a_capacity = buffer_.capacity;
			return DMUI_RESULT_OK;
		}

		[[nodiscard]] DMUI_Result RecordResizeFailure(
			DMUI_Result a_result) noexcept
		{
			result_ = a_result;
			return a_result;
		}

		std::string storage_;
		DMUI_TextBuffer buffer_{};
		DMUI_Result result_{ DMUI_RESULT_OK };
	};
}
