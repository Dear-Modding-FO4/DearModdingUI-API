#include <DearModdingUI/API.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

// Compiler-checked DMUI_HostAPI layout for ABI 2; the host compiles this file too.
static_assert(DMUI_ABI_MAJOR == 2);
// Released ABI 2.0 clients request the bare major.
static_assert(DMUI_MAKE_ABI_VERSION(2, 0) == 2u);
static_assert(DMUI_ABI_VERSION_MINOR(DMUI_ABI_VERSION) == DMUI_ABI_MINOR);
// The export is the one entry point outside the manifest-guarded tables.
static_assert(std::is_same_v<
	decltype(&DMUI_GetAPI),
	const DMUI_HostAPI* (DMUI_CALL*)(uint32_t) noexcept>);
#if UINTPTR_MAX == UINT64_MAX
static_assert(offsetof(DMUI_HostAPI, abiMajor) == 0);
static_assert(offsetof(DMUI_HostAPI, ui) == 8);
static_assert(offsetof(DMUI_HostAPI, registerClient) == 16);
static_assert(offsetof(DMUI_HostAPI, resetOverlay) ==
	offsetof(DMUI_HostAPI, queryOverlay) + sizeof(void*));
static_assert(offsetof(DMUI_HostAPI, drawSearchInputBuffer) == 448);
static_assert(offsetof(DMUI_HostAPI, loadImageFile) == 456);
static_assert(offsetof(DMUI_HostAPI, requestOverlayFocus) == 464);
static_assert(offsetof(DMUI_HostAPI, queryOverlayFocus) == 480);
static_assert(offsetof(DMUI_HostAPI, resolveText) == 488);
static_assert(sizeof(DMUI_OverlayFocusInfo) == 16);
#endif
