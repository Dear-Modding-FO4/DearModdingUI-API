#include <DearModdingUI/Client.h>

#include <cstring>
#include <limits>
#include <string>
#include <string_view>

namespace
{
	DMUI_Result DMUI_CALL RegisterClientStub(
		const DMUI_ClientDescriptor*,
		DMUI_ClientHandle*) noexcept
	{
		return DMUI_RESULT_OK;
	}

	bool TestTextInputBuffer()
	{
		dmui::TextInputBuffer growable{ "seed" };
		auto* growableBuffer = growable.Get();
		if (growable.Result() != DMUI_RESULT_OK ||
			!growableBuffer ||
			!growableBuffer->resize ||
			growableBuffer->capacity < 64)
			return false;

		auto* data = growableBuffer->data;
		auto capacity = growableBuffer->capacity;
		if (growableBuffer->resize(
				growableBuffer->userData,
				128,
				&data,
				&capacity) != DMUI_RESULT_OK ||
			capacity < 128 ||
			std::string_view{ data, 4 } != "seed")
			return false;
		std::memcpy(data, "grown", 6);
		std::string committed{ "original" };
		if (growable.CommitTo(committed) != DMUI_RESULT_OK ||
			committed != "grown")
			return false;

		dmui::TextInputBuffer fixed{ "query", 512 };
		const auto* fixedBuffer = fixed.Get();
		if (fixed.Result() != DMUI_RESULT_OK ||
			!fixedBuffer ||
			fixedBuffer->capacity != 513 ||
			fixedBuffer->resize)
			return false;

		constexpr char embeddedNul[]{ 'a', '\0', 'b' };
		dmui::TextInputBuffer invalid{
			std::string_view{ embeddedNul, sizeof(embeddedNul) }
		};
		if (invalid.Result() != DMUI_RESULT_INVALID_ARGUMENT)
			return false;

		std::string original{ "stable" };
		dmui::TextInputBuffer failedGrowth{ original };
		auto* failedBuffer = failedGrowth.Get();
		auto* previousData = failedBuffer->data;
		auto previousCapacity = failedBuffer->capacity;
		auto* callbackData = previousData;
		auto callbackCapacity = previousCapacity;
		const auto tooLarge =
			static_cast<size_t>((std::numeric_limits<int>::max)()) + 1;
		return failedBuffer->resize(
				   failedBuffer->userData,
				   tooLarge,
				   &callbackData,
				   &callbackCapacity) ==
				DMUI_RESULT_INVALID_ARGUMENT &&
			callbackData == previousData &&
			callbackCapacity == previousCapacity &&
			std::string_view{ previousData, 6 } == "stable" &&
			failedGrowth.Result() == DMUI_RESULT_INVALID_ARGUMENT &&
			failedGrowth.CommitTo(original) ==
				DMUI_RESULT_INVALID_ARGUMENT &&
			original == "stable";
	}
}

int main()
{
	const DMUI_ImageDescriptor descriptor{
		sizeof(DMUI_ImageDescriptor),
		1,
		1,
		DMUI_PIXEL_FORMAT_RGBA8_UNORM,
		0,
		4,
		4,
		nullptr
	};
	DMUI_HostAPI api{};
	api.createImage = nullptr;
	api.updateImage = nullptr;
	api.resolveIconGlyph = nullptr;
	api.beginSettingsRow = nullptr;
	api.endSettingsRow = nullptr;
	api.beginSettingsRowEx = nullptr;
	api.beginField = nullptr;
	api.setFieldFeedback = nullptr;
	api.endField = nullptr;
	api.drawTextView = nullptr;
	api.drawSearchInputBuffer = nullptr;
	const DMUI_IconResolutionRequest iconRequest{
		sizeof(DMUI_IconResolutionRequest),
		"wrench",
		"Graphics Settings",
		"General"
	};
	const DMUI_CategoryDescriptor category{
		DMUI_CATEGORY_DESCRIPTOR_ICON_SIZE,
		"lighting",
		"Lighting",
		0,
		0,
		"sun-horizon"
	};
	const DMUI_PageDescriptor page{
		DMUI_PAGE_DESCRIPTOR_ICON_SIZE,
		"settings",
		"Settings",
		"lighting",
		nullptr,
		0,
		DMUI_PAGE_KIND_SETTINGS,
		nullptr,
		nullptr,
		"sliders-horizontal"
	};
	const size_t lineOffsets[]{ 0 };
	const DMUI_TextViewDescriptor textView{
		sizeof(DMUI_TextViewDescriptor),
		"preview",
		"",
		0,
		lineOffsets,
		1,
		nullptr,
		0,
		0,
		1,
		1,
		{ 0.0f, 320.0f }
	};
	const DMUI_TextViewState textState{
		sizeof(DMUI_TextViewState),
		1,
		1,
		DMUI_TEXT_VIEW_NO_OFFSET,
		DMUI_TEXT_VIEW_NO_OFFSET
	};
	const DMUI_TextBuffer textBuffer{
		DMUI_TEXT_BUFFER_0_2_SIZE,
		nullptr,
		1,
		nullptr,
		nullptr
	};
	DMUI_HostAPI prefixAPI{};
	prefixAPI.structSize = DMUI_HOST_API_REGISTER_CLIENT_SIZE;
	prefixAPI.hostAbiVersion = DMUI_HOST_ABI_1;
	prefixAPI.registerClient = &RegisterClientStub;
	const auto defaultPrefixResult =
		dmui::PreflightHostAPI(&prefixAPI, {});
	dmui::ClientOptions textViewOptions;
	textViewOptions.minimumHostAPISize =
		DMUI_HOST_API_DRAW_TEXT_VIEW_SIZE;
	const auto textViewPrefixResult =
		dmui::PreflightHostAPI(&prefixAPI, textViewOptions);
	dmui::ClientOptions searchBufferOptions;
	searchBufferOptions.minimumHostAPISize =
		DMUI_HOST_API_DRAW_SEARCH_INPUT_BUFFER_SIZE;
	const auto searchBufferPrefixResult =
		dmui::PreflightHostAPI(&prefixAPI, searchBufferOptions);
	return descriptor.structSize == DMUI_IMAGE_DESCRIPTOR_0_1_SIZE &&
			iconRequest.structSize == DMUI_ICON_RESOLUTION_REQUEST_0_1_SIZE &&
			category.structSize == sizeof(category) &&
			page.structSize == sizeof(page) &&
			textView.structSize == DMUI_TEXT_VIEW_DESCRIPTOR_0_2_SIZE &&
			textState.structSize == DMUI_TEXT_VIEW_STATE_0_2_SIZE &&
			textBuffer.structSize == DMUI_TEXT_BUFFER_0_2_SIZE &&
			DMUI_FONT_ROLE_MONOSPACE + 1u == DMUI_FONT_ROLE_COUNT &&
			DMUI_API_VERSION_CURRENT == DMUI_API_VERSION_0_2 &&
			defaultPrefixResult == DMUI_RESULT_UNSUPPORTED_ABI &&
			textViewPrefixResult == DMUI_RESULT_STRUCT_TOO_SMALL &&
			searchBufferPrefixResult == DMUI_RESULT_STRUCT_TOO_SMALL &&
			TestTextInputBuffer() ?
		0 :
		1;
}
