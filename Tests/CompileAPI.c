#include <DearModdingUI/API.h>

_Static_assert(DMUI_ABI_VERSION == 2u, "ABI 2");

DMUI_ImageDescriptor image_descriptor = {
	1u,
	1u,
	DMUI_PIXEL_FORMAT_RGBA8_UNORM,
	0u,
	4u,
	4u,
	0
};

DMUI_CreateImageFn create_image;
DMUI_LoadImageFileFn load_image_file;
DMUI_UpdateImageFn update_image;
DMUI_ResolveIconGlyphFn resolve_icon_glyph;
DMUI_DrawSearchInputBufferFn draw_search_input_buffer;
DMUI_ResizeTextBufferFn resize_text_buffer;

DMUI_TextBuffer text_buffer = {
	0,
	1u,
	0,
	0
};

DMUI_IconResolutionRequest icon_request = {
	"wrench",
	"Graphics Settings",
	"General"
};

DMUI_ExternalOpenDescriptor virtual_file = {
	DMUI_EXTERNAL_TARGET_VIRTUAL_FILE,
	"C:\\game\\Data\\settings.ini"
};

DMUI_ExternalOpenDescriptor virtual_file_parent = {
	DMUI_EXTERNAL_TARGET_VIRTUAL_FILE_PARENT,
	"C:\\game\\Data\\settings.ini"
};


DMUI_CategoryDescriptor lighting_category = {
	"lighting",
	"Lighting",
	0,
	0,
	"sun-horizon"
};

DMUI_PageDescriptor lighting_page = {
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
