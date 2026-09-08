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

DMUI_ExternalOpenDescriptor virtual_file = {
	DMUI_EXTERNAL_OPEN_DESCRIPTOR_0_1_SIZE,
	DMUI_EXTERNAL_TARGET_VIRTUAL_FILE,
	"C:\\game\\Data\\settings.ini"
};

DMUI_ExternalOpenDescriptor virtual_file_parent = {
	DMUI_EXTERNAL_OPEN_DESCRIPTOR_0_1_SIZE,
	DMUI_EXTERNAL_TARGET_VIRTUAL_FILE_PARENT,
	"C:\\game\\Data\\settings.ini"
};

DMUI_HostServices virtual_file_service = DMUI_HOST_SERVICE_VIRTUAL_FILE_TARGETS;
