#include <DearModdingUI/API.h>

#include <cstddef>

// Keep these offsets synchronized with the host's ABI layout guard.
#if UINTPTR_MAX == UINT64_MAX
static_assert(sizeof(DMUI_HostAPI) == 216);
static_assert(offsetof(DMUI_HostAPI, structSize) == 0);
static_assert(offsetof(DMUI_HostAPI, apiVersion) == 4);
static_assert(offsetof(DMUI_HostAPI, imguiFingerprint) == 8);
static_assert(offsetof(DMUI_HostAPI, registerClient) == 16);
static_assert(offsetof(DMUI_HostAPI, registerPage) == 24);
static_assert(offsetof(DMUI_HostAPI, queryState) == 32);
static_assert(offsetof(DMUI_HostAPI, requestFrame) == 40);
static_assert(offsetof(DMUI_HostAPI, releaseFrame) == 48);
static_assert(offsetof(DMUI_HostAPI, isMenuVisible) == 56);
static_assert(offsetof(DMUI_HostAPI, selectPage) == 64);
static_assert(offsetof(DMUI_HostAPI, attachSwapChain) == 72);
static_assert(offsetof(DMUI_HostAPI, registerAction) == 80);
static_assert(offsetof(DMUI_HostAPI, setStatus) == 88);
static_assert(offsetof(DMUI_HostAPI, getThemeColors) == 96);
static_assert(offsetof(DMUI_HostAPI, pushFont) == 104);
static_assert(offsetof(DMUI_HostAPI, popFont) == 112);
static_assert(offsetof(DMUI_HostAPI, drawSectionHeader) == 120);
static_assert(offsetof(DMUI_HostAPI, drawSearchInput) == 128);
static_assert(offsetof(DMUI_HostAPI, drawCollapsingSectionHeader) == 136);
static_assert(offsetof(DMUI_HostAPI, drawSettingsActionButton) == 144);
static_assert(offsetof(DMUI_HostAPI, settingsActionButtonWidth) == 152);
static_assert(offsetof(DMUI_HostAPI, settingsActionButtonExtent) == 160);
static_assert(offsetof(DMUI_HostAPI, registerFrameObserver) == 168);
static_assert(offsetof(DMUI_HostAPI, queryVideoMemory) == 176);
static_assert(offsetof(DMUI_HostAPI, drawBulletText) == 184);
static_assert(offsetof(DMUI_HostAPI, registerHotkeyAction) == 192);
static_assert(offsetof(DMUI_HostAPI, queryHotkeyBinding) == 200);
static_assert(offsetof(DMUI_HostAPI, unregisterHotkeyAction) == 208);
static_assert(DMUI_HOST_API_REGISTER_HOTKEY_ACTION_SIZE == 200);
static_assert(DMUI_HOST_API_QUERY_HOTKEY_BINDING_SIZE == 208);
static_assert(DMUI_HOST_API_UNREGISTER_HOTKEY_ACTION_SIZE == sizeof(DMUI_HostAPI));
#endif
