#include <DearModdingUI/CUIAPI.h>

_Static_assert(sizeof(DMUI_Vec2) == sizeof(float) * 2, "DMUI_Vec2 layout");
_Static_assert(sizeof(DMUI_Vec4) == sizeof(float) * 4, "DMUI_Vec4 layout");
_Static_assert(DMUI_ABI_VERSION == 2u, "ABI 2");

static DMUI_Result DMUI_CALL CompileImage(
	DMUI_ClientHandle client, DMUI_ImageHandle image,
	const DMUI_ImageDrawOptions* options, uint32_t* drawn)
{
	(void)client;
	(void)image;
	(void)options;
	*drawn = 0;
	return DMUI_RESULT_OK;
}

DMUI_UIImageFn CompileImagePointer(void)
{
	return &CompileImage;
}
