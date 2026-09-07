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
	return descriptor.structSize == DMUI_IMAGE_DESCRIPTOR_0_1_SIZE ? 0 : 1;
}
