#include <DearModdingUI/Client.h>

namespace
{
	DMUI_Result DMUI_CALL RegisterClientStub(
		const DMUI_ClientDescriptor*,
		DMUI_ClientHandle*) noexcept
	{
		return DMUI_RESULT_OK;
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
	return descriptor.structSize == DMUI_IMAGE_DESCRIPTOR_0_1_SIZE &&
			iconRequest.structSize == DMUI_ICON_RESOLUTION_REQUEST_0_1_SIZE &&
			category.structSize == sizeof(category) &&
			page.structSize == sizeof(page) &&
			textView.structSize == DMUI_TEXT_VIEW_DESCRIPTOR_0_2_SIZE &&
			textState.structSize == DMUI_TEXT_VIEW_STATE_0_2_SIZE &&
			DMUI_FONT_ROLE_MONOSPACE + 1u == DMUI_FONT_ROLE_COUNT &&
			DMUI_API_VERSION_CURRENT == DMUI_API_VERSION_0_2 &&
			defaultPrefixResult == DMUI_RESULT_UNSUPPORTED_ABI &&
			textViewPrefixResult == DMUI_RESULT_STRUCT_TOO_SMALL ?
		0 :
		1;
}
