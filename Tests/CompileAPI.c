#include <DearModdingUI/API.h>

_Static_assert(
	DMUI_HOST_SERVICE_NAVIGATION_ICONS == (UINT64_C(1) << 11u),
	"navigation icon service bit changed");

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
DMUI_HostServices navigation_icon_service = DMUI_HOST_SERVICE_NAVIGATION_ICONS;

DMUI_CategoryDescriptor lighting_category = {
	DMUI_CATEGORY_DESCRIPTOR_ICON_SIZE,
	"lighting",
	"Lighting",
	0,
	0,
	"sun-horizon"
};

DMUI_PageDescriptor lighting_page = {
	DMUI_PAGE_DESCRIPTOR_ICON_SIZE,
	"lighting",
	"Lighting",
	"lighting",
	0,
	0,
	DMUI_PAGE_KIND_SETTINGS,
	0,
	0,
	"sun"
};
