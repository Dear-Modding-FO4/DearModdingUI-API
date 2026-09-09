#include <DearModdingUI/API.h>
#include <DearModdingUI/CUIAPI.h>

#include <stddef.h>
#include <stdint.h>

_Static_assert(sizeof(DMUI_Vec2) == sizeof(float) * 2, "DMUI_Vec2 layout");
_Static_assert(sizeof(DMUI_Vec4) == sizeof(float) * 4, "DMUI_Vec4 layout");
_Static_assert(DMUI_UI_ABI_CURRENT == DMUI_UI_ABI_1, "UI ABI family");
_Static_assert(
	DMUI_UI_API_REQUIRED_SIZE <= DMUI_UI_API_CURRENT_SIZE,
	"required UI prefix");
_Static_assert(
	offsetof(DMUI_UIAPI, plotLines) + sizeof(DMUI_UIPlotLinesFn) ==
		DMUI_UI_API_CURRENT_SIZE,
	"UI table tail");
#if UINTPTR_MAX == UINT64_MAX
_Static_assert(offsetof(DMUI_UIAPI, getStyleMetrics) == 16, "UI slot 1");
_Static_assert(offsetof(DMUI_UIAPI, newLine) == 504, "UI slot 62");
_Static_assert(offsetof(DMUI_UIAPI, plotLines) == 512, "UI slot 63");
_Static_assert(DMUI_UI_API_REQUIRED_SIZE == 512, "required UI prefix");
_Static_assert(DMUI_UI_API_CURRENT_SIZE == 520, "current UI prefix");
_Static_assert(sizeof(DMUI_UIAPI) == 520, "current UI table layout");
#endif

static DMUI_Result DMUI_CALL CompileQueryUI(
	uint32_t requestedUIAbi,
	uint32_t minimumRevision,
	uint32_t minimumTableSize,
	DMUI_UIAPIInfo* info)
{
	(void)requestedUIAbi;
	(void)minimumRevision;
	(void)minimumTableSize;
	(void)info;
	return DMUI_RESULT_UNSUPPORTED_ABI;
}

DMUI_QueryUIAPIFn CompileCUIQueryPointer(void)
{
	return &CompileQueryUI;
}
