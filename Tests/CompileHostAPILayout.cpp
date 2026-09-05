#include <DearModdingUI/API.h>

#include <cstddef>
#include <cstdint>

// The single ABI layout guard for DMUI_HostAPI; the host compiles this file too.
#if UINTPTR_MAX == UINT64_MAX
static_assert(sizeof(DMUI_HostAPI) == 288);
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
static_assert(offsetof(DMUI_HostAPI, beginSettingsTable) == 216);
static_assert(offsetof(DMUI_HostAPI, beginSettingsRow) == 224);
static_assert(offsetof(DMUI_HostAPI, endSettingsRow) == 232);
static_assert(offsetof(DMUI_HostAPI, endSettingsTable) == 240);
static_assert(offsetof(DMUI_HostAPI, beginSettingsRowEx) == 248);
static_assert(offsetof(DMUI_HostAPI, registerPageActivityObserver) == 256);
static_assert(offsetof(DMUI_HostAPI, drawLinkRow) == 264);
static_assert(offsetof(DMUI_HostAPI, drawFaq) == 272);
static_assert(offsetof(DMUI_HostAPI, reportDiagnostic) == 280);
static_assert(DMUI_HOST_API_SELECT_PAGE_SIZE == 72);
static_assert(DMUI_HOST_API_ATTACH_SWAP_CHAIN_SIZE == 80);
static_assert(DMUI_HOST_API_REGISTER_ACTION_SIZE == 88);
static_assert(DMUI_HOST_API_SET_STATUS_SIZE == 96);
static_assert(DMUI_HOST_API_GET_THEME_COLORS_SIZE == 104);
static_assert(DMUI_HOST_API_PUSH_FONT_SIZE == 112);
static_assert(DMUI_HOST_API_POP_FONT_SIZE == 120);
static_assert(DMUI_HOST_API_DRAW_SECTION_HEADER_SIZE == 128);
static_assert(DMUI_HOST_API_DRAW_SEARCH_INPUT_SIZE == 136);
static_assert(DMUI_HOST_API_DRAW_COLLAPSING_SECTION_HEADER_SIZE == 144);
static_assert(DMUI_HOST_API_DRAW_SETTINGS_ACTION_BUTTON_SIZE == 152);
static_assert(DMUI_HOST_API_SETTINGS_ACTION_BUTTON_WIDTH_SIZE == 160);
static_assert(DMUI_HOST_API_SETTINGS_ACTION_BUTTON_EXTENT_SIZE == 168);
static_assert(DMUI_HOST_API_REGISTER_FRAME_OBSERVER_SIZE == 176);
static_assert(DMUI_HOST_API_QUERY_VIDEO_MEMORY_SIZE == 184);
static_assert(DMUI_HOST_API_DRAW_BULLET_TEXT_SIZE == 192);
static_assert(DMUI_HOST_API_REGISTER_HOTKEY_ACTION_SIZE == 200);
static_assert(DMUI_HOST_API_QUERY_HOTKEY_BINDING_SIZE == 208);
static_assert(DMUI_HOST_API_UNREGISTER_HOTKEY_ACTION_SIZE == 216);
static_assert(DMUI_HOST_API_BEGIN_SETTINGS_TABLE_SIZE == 224);
static_assert(DMUI_HOST_API_BEGIN_SETTINGS_ROW_SIZE == 232);
static_assert(DMUI_HOST_API_END_SETTINGS_ROW_SIZE == 240);
static_assert(DMUI_HOST_API_END_SETTINGS_TABLE_SIZE == 248);
static_assert(DMUI_HOST_API_BEGIN_SETTINGS_ROW_EX_SIZE == 256);
static_assert(DMUI_HOST_API_REGISTER_PAGE_ACTIVITY_OBSERVER_SIZE == 264);
static_assert(DMUI_HOST_API_DRAW_LINK_ROW_SIZE == 272);
static_assert(DMUI_HOST_API_DRAW_FAQ_SIZE == 280);
static_assert(DMUI_HOST_API_REPORT_DIAGNOSTIC_SIZE ==
	sizeof(DMUI_HostAPI));
#endif
