#include <DearModdingUI/API.h>

DMUI_ImageDescriptor image_descriptor = {
	DMUI_IMAGE_DESCRIPTOR_0_1_SIZE,
	1u,
	1u,
	DMUI_PIXEL_FORMAT_RGBA8_UNORM,
	0u,
	4u,
	4u,
	0
};

DMUI_CreateImageFn create_image;
DMUI_UpdateImageFn update_image;
