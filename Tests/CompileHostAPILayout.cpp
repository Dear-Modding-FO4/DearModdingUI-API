#include <DearModdingUI/API.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

// Compiler-checked DMUI_HostAPI layout for ABI 3; the host compiles this file too.
static_assert(DMUI_ABI_MAJOR == 3);
// A .0 client requests the bare major.
static_assert(DMUI_MAKE_ABI_VERSION(3, 0) == 3u);
static_assert(DMUI_ABI_VERSION_MINOR(DMUI_ABI_VERSION) == DMUI_ABI_MINOR);
// The export is the one entry point outside the manifest-guarded tables.
static_assert(std::is_same_v<
	decltype(&DMUI_GetAPI),
	const DMUI_HostAPI* (DMUI_CALL*)(uint32_t) noexcept>);
#if UINTPTR_MAX == UINT64_MAX
static_assert(offsetof(DMUI_HostAPI, abiMajor) == 0);
static_assert(offsetof(DMUI_HostAPI, ui) == 8);
static_assert(offsetof(DMUI_HostAPI, registerClient) == 16);
static_assert(offsetof(DMUI_HostAPI, registerAction) == 80);
static_assert(offsetof(DMUI_HostAPI, unregisterAction) == 88);
static_assert(offsetof(DMUI_HostAPI, setActionEnabled) == 96);
static_assert(offsetof(DMUI_HostAPI, resetOverlay) ==
	offsetof(DMUI_HostAPI, queryOverlay) + sizeof(void*));
static_assert(offsetof(DMUI_HostAPI, drawTextView) == 376);
static_assert(offsetof(DMUI_HostAPI, loadImageFile) == 384);
static_assert(offsetof(DMUI_HostAPI, loadImageMemory) == 392);
static_assert(offsetof(DMUI_HostAPI, requestOverlayFocus) == 400);
static_assert(offsetof(DMUI_HostAPI, queryOverlayFocus) == 416);
static_assert(offsetof(DMUI_HostAPI, resolveText) == 424);
static_assert(sizeof(DMUI_OverlayFocusInfo) == 16);
static_assert(sizeof(DMUI_TextSpan) == 72 && offsetof(DMUI_TextSpan, userId) == 64);
static_assert(sizeof(DMUI_TextViewDescriptor) == 128 &&
	offsetof(DMUI_TextViewDescriptor, spans) == 64 &&
	offsetof(DMUI_TextViewDescriptor, flags) == 112);
static_assert(sizeof(DMUI_TextViewState) == 64 &&
	offsetof(DMUI_TextViewState, scrollCommand) == 48);
static_assert(sizeof(DMUI_TextFieldOptions) == 64 &&
	offsetof(DMUI_TextFieldOptions, styleSpans) == 48);
static_assert(sizeof(DMUI_PointerState) == 44);
static_assert(sizeof(DMUI_Placement) == 16 && sizeof(DMUI_SpinnerDescriptor) == 16);
static_assert(sizeof(DMUI_ToggleDescriptor) == 24 && sizeof(DMUI_ToggleResult) == 8);
static_assert(sizeof(DMUI_NumericValue) == 8 && sizeof(DMUI_NumericDescriptor) == 88 &&
	sizeof(DMUI_NumericResult) == 16);
static_assert(sizeof(DMUI_ColorPreset) == 24 && sizeof(DMUI_ColorEditDescriptor) == 48 &&
	sizeof(DMUI_ColorEditResult) == 12);
static_assert(sizeof(DMUI_KeyCaptureDescriptor) == 56 && sizeof(DMUI_KeyCaptureResult) == 36);
#endif
