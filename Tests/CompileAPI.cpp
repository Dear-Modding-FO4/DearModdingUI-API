#include <DearModdingUI/API.h>

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
	return descriptor.structSize == DMUI_IMAGE_DESCRIPTOR_0_1_SIZE &&
			iconRequest.structSize == DMUI_ICON_RESOLUTION_REQUEST_0_1_SIZE &&
			category.structSize == sizeof(category) &&
			page.structSize == sizeof(page) ?
		0 :
		1;
}
