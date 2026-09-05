#pragma once

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
#define DMUI_EXTERN_C extern "C"
#define DMUI_NOEXCEPT noexcept
#else
#define DMUI_EXTERN_C extern
#define DMUI_NOEXCEPT
#endif

#if defined(_MSC_VER)
#define DMUI_CALL __cdecl
#else
#define DMUI_CALL
#endif

#if defined(_WIN32) && defined(DMUI_HOST_EXPORTS)
#define DMUI_EXPORT DMUI_EXTERN_C __declspec(dllexport)
#else
#define DMUI_EXPORT DMUI_EXTERN_C
#endif

#define DMUI_MAKE_VERSION(major, minor) ((((uint32_t)(major)) << 16u) | ((uint32_t)(minor)))
#define DMUI_VERSION_MAJOR(version) ((uint32_t)(version) >> 16u)
#define DMUI_VERSION_MINOR(version) ((uint32_t)(version) & 0xFFFFu)

#define DMUI_API_VERSION_0_1 DMUI_MAKE_VERSION(0u, 1u)
#define DMUI_API_VERSION_CURRENT DMUI_API_VERSION_0_1
#define DMUI_IMGUI_UPSTREAM_COMMIT "9acdfbf46810c0c74ab281ce04122c4149ae8bd1"
#define DMUI_IMGUI_VERSION_NUM 19291u
#define DMUI_IMGUI_FINGERPRINT_DOCKING 0x00000001u
#define DMUI_IMGUI_FINGERPRINT_WCHAR32 0x00000002u
#define DMUI_IMGUI_FINGERPRINT_CUSTOM_TEXTURE_ID 0x00000004u
#define DMUI_IMGUI_FINGERPRINT_CUSTOM_DRAW_VERT 0x00000008u
#define DMUI_IMGUI_FINGERPRINT_BGRA_PACKED_COLOR 0x00000010u
#define DMUI_IMGUI_FINGERPRINT_OBSOLETE_DISABLED 0x00000020u
#define DMUI_IMGUI_FINGERPRINT_TEST_ENGINE 0x00000040u
#define DMUI_IMGUI_FINGERPRINT_LEGACY_CRC32 0x00000080u
#define DMUI_IMGUI_FINGERPRINT_FREETYPE 0x00000100u
#define DMUI_IMGUI_FINGERPRINT_MATH_OPERATORS 0x00000200u
#define DMUI_IMGUI_FINGERPRINT_DEBUG_TOOLS_DISABLED 0x00000400u
#define DMUI_IMGUI_FINGERPRINT_CUSTOM_DRAW_IDX 0x00000800u
#define DMUI_IMGUI_FINGERPRINT_CUSTOM_DRAW_CALLBACK 0x00001000u
#define DMUI_IMGUI_FINGERPRINT_VEC2_EXTRA 0x00002000u
#define DMUI_IMGUI_FINGERPRINT_VEC4_EXTRA 0x00004000u

typedef uint32_t DMUI_Result;

#define DMUI_RESULT_OK 0u
#define DMUI_RESULT_UNSUPPORTED_ABI 1u
#define DMUI_RESULT_INVALID_ARGUMENT 2u
#define DMUI_RESULT_STRUCT_TOO_SMALL 3u
#define DMUI_RESULT_INVALID_DESCRIPTOR 4u
#define DMUI_RESULT_FINGERPRINT_MISMATCH 5u
#define DMUI_RESULT_DUPLICATE_CLIENT_ID 6u
#define DMUI_RESULT_DUPLICATE_PAGE_ID 7u
#define DMUI_RESULT_REGISTRATION_CLOSED 8u
#define DMUI_RESULT_HOST_DISABLED 9u
#define DMUI_RESULT_HOST_NOT_INITIALIZED 10u
#define DMUI_RESULT_HOST_NOT_READY 11u
#define DMUI_RESULT_BACKEND_FAILED 12u
#define DMUI_RESULT_RESOURCE_EXHAUSTED 13u
#define DMUI_RESULT_CLIENT_NOT_FOUND 14u
#define DMUI_RESULT_PAGE_NOT_FOUND 15u
#define DMUI_RESULT_INVALID_PAGE_KIND 16u
#define DMUI_RESULT_NO_FRAME_DEMAND 17u
#define DMUI_RESULT_CALLBACK_FAILED 18u
#define DMUI_RESULT_CLIENT_CAPABILITY_REQUIRED 19u
#define DMUI_RESULT_SWAPCHAIN_REJECTED 20u
#define DMUI_RESULT_RENDERER_BUSY 21u
#define DMUI_RESULT_DUPLICATE_ACTION_ID 22u
#define DMUI_RESULT_ACTION_NOT_FOUND 23u
#define DMUI_RESULT_MALFORMED_ACTION_ID 24u
#define DMUI_RESULT_UNKNOWN_CHORD 25u
#define DMUI_RESULT_WRONG_THREAD 26u
#define DMUI_RESULT_UNBALANCED_BRACKET 27u

static inline const char* DMUI_ResultToString(DMUI_Result result) DMUI_NOEXCEPT
{
	switch (result)
	{
	case DMUI_RESULT_OK:
		return "OK";
	case DMUI_RESULT_UNSUPPORTED_ABI:
		return "UNSUPPORTED_ABI";
	case DMUI_RESULT_INVALID_ARGUMENT:
		return "INVALID_ARGUMENT";
	case DMUI_RESULT_STRUCT_TOO_SMALL:
		return "STRUCT_TOO_SMALL";
	case DMUI_RESULT_INVALID_DESCRIPTOR:
		return "INVALID_DESCRIPTOR";
	case DMUI_RESULT_FINGERPRINT_MISMATCH:
		return "FINGERPRINT_MISMATCH";
	case DMUI_RESULT_DUPLICATE_CLIENT_ID:
		return "DUPLICATE_CLIENT_ID";
	case DMUI_RESULT_DUPLICATE_PAGE_ID:
		return "DUPLICATE_PAGE_ID";
	case DMUI_RESULT_REGISTRATION_CLOSED:
		return "REGISTRATION_CLOSED";
	case DMUI_RESULT_HOST_DISABLED:
		return "HOST_DISABLED";
	case DMUI_RESULT_HOST_NOT_INITIALIZED:
		return "HOST_NOT_INITIALIZED";
	case DMUI_RESULT_HOST_NOT_READY:
		return "HOST_NOT_READY";
	case DMUI_RESULT_BACKEND_FAILED:
		return "BACKEND_FAILED";
	case DMUI_RESULT_RESOURCE_EXHAUSTED:
		return "RESOURCE_EXHAUSTED";
	case DMUI_RESULT_CLIENT_NOT_FOUND:
		return "CLIENT_NOT_FOUND";
	case DMUI_RESULT_PAGE_NOT_FOUND:
		return "PAGE_NOT_FOUND";
	case DMUI_RESULT_INVALID_PAGE_KIND:
		return "INVALID_PAGE_KIND";
	case DMUI_RESULT_NO_FRAME_DEMAND:
		return "NO_FRAME_DEMAND";
	case DMUI_RESULT_CALLBACK_FAILED:
		return "CALLBACK_FAILED";
	case DMUI_RESULT_CLIENT_CAPABILITY_REQUIRED:
		return "CLIENT_CAPABILITY_REQUIRED";
	case DMUI_RESULT_SWAPCHAIN_REJECTED:
		return "SWAPCHAIN_REJECTED";
	case DMUI_RESULT_RENDERER_BUSY:
		return "RENDERER_BUSY";
	case DMUI_RESULT_DUPLICATE_ACTION_ID:
		return "DUPLICATE_ACTION_ID";
	case DMUI_RESULT_ACTION_NOT_FOUND:
		return "ACTION_NOT_FOUND";
	case DMUI_RESULT_MALFORMED_ACTION_ID:
		return "MALFORMED_ACTION_ID";
	case DMUI_RESULT_UNKNOWN_CHORD:
		return "UNKNOWN_CHORD";
	case DMUI_RESULT_WRONG_THREAD:
		return "WRONG_THREAD";
	case DMUI_RESULT_UNBALANCED_BRACKET:
		return "UNBALANCED_BRACKET";
	default:
		return "UNKNOWN";
	}
}

typedef uint32_t DMUI_HostState;

#define DMUI_HOST_STATE_NOT_INITIALIZED 0u
#define DMUI_HOST_STATE_WAITING_FOR_PRESENT 1u
#define DMUI_HOST_STATE_INITIALIZING 2u
#define DMUI_HOST_STATE_READY 3u
#define DMUI_HOST_STATE_UNAVAILABLE 4u

typedef uint32_t DMUI_UnavailableReason;

#define DMUI_UNAVAILABLE_NONE 0u
#define DMUI_UNAVAILABLE_HOST_DISABLED 1u
#define DMUI_UNAVAILABLE_BACKEND_FAILED 2u

typedef uint32_t DMUI_PageKind;

#define DMUI_PAGE_KIND_SETTINGS 1u
#define DMUI_PAGE_KIND_OVERLAY 2u

typedef uint32_t DMUI_StatusSeverity;

#define DMUI_STATUS_SEVERITY_INFO 0u
#define DMUI_STATUS_SEVERITY_SUCCESS 1u
#define DMUI_STATUS_SEVERITY_WARNING 2u
#define DMUI_STATUS_SEVERITY_ERROR 3u

typedef uint32_t DMUI_FontRole;

#define DMUI_FONT_ROLE_BODY 0u
#define DMUI_FONT_ROLE_TITLE 1u
#define DMUI_FONT_ROLE_HEADING 2u
#define DMUI_FONT_ROLE_SUBHEADING 3u
#define DMUI_FONT_ROLE_SUBTEXT 4u
#define DMUI_FONT_ROLE_COUNT 5u

typedef uint32_t DMUI_SettingsAction;

#define DMUI_SETTINGS_ACTION_RESET 0u
#define DMUI_SETTINGS_ACTION_REVERT 1u
#define DMUI_SETTINGS_ACTION_APPLY 2u

typedef uint32_t DMUI_HotkeyBindingState;

#define DMUI_HOTKEY_BINDING_BOUND 0u
#define DMUI_HOTKEY_BINDING_UNBOUND_USER 1u
#define DMUI_HOTKEY_BINDING_UNBOUND_DEFAULT_CONFLICT 2u
#define DMUI_HOTKEY_BINDING_UNBOUND_NEVER_SET 3u
#define DMUI_HOTKEY_BINDING_UNBOUND_OVERRIDE_CONFLICT 4u
#define DMUI_HOTKEY_BINDING_UNBOUND_INVALID_OVERRIDE 5u

typedef uint32_t DMUI_ClientCapabilities;

#define DMUI_CLIENT_CAPABILITY_NONE 0u
#define DMUI_CLIENT_CAPABILITY_RENDERER_REPLACEMENT 0x00000001u

typedef uint32_t DMUI_ClientOrigin;

#define DMUI_CLIENT_ORIGIN_NATIVE 0u
#define DMUI_CLIENT_ORIGIN_BRIDGED 1u

typedef uint64_t DMUI_ClientHandle;
typedef uint64_t DMUI_PageHandle;
typedef uint64_t DMUI_ActionHandle;
typedef uint64_t DMUI_FrameObserverHandle;
typedef uint64_t DMUI_HotkeyActionHandle;
typedef uint64_t DMUI_PageActivityObserverHandle;

#define DMUI_INVALID_CLIENT_HANDLE ((DMUI_ClientHandle)0u)
#define DMUI_INVALID_PAGE_HANDLE ((DMUI_PageHandle)0u)
#define DMUI_INVALID_ACTION_HANDLE ((DMUI_ActionHandle)0u)
#define DMUI_INVALID_FRAME_OBSERVER_HANDLE ((DMUI_FrameObserverHandle)0u)
#define DMUI_INVALID_HOTKEY_ACTION_HANDLE ((DMUI_HotkeyActionHandle)0u)
#define DMUI_INVALID_PAGE_ACTIVITY_OBSERVER_HANDLE ((DMUI_PageActivityObserverHandle)0u)

typedef uint32_t DMUI_PageActivityKind;

#define DMUI_PAGE_ACTIVITY_ACTIVATED 1u
#define DMUI_PAGE_ACTIVITY_CHANGED 2u
#define DMUI_PAGE_ACTIVITY_DEACTIVATED 3u

typedef uint32_t DMUI_SettingsRowLayout;

#define DMUI_SETTINGS_ROW_LAYOUT_LABEL_VALUE 0u
#define DMUI_SETTINGS_ROW_LAYOUT_FULL_SPAN 1u

#if defined(_MSC_VER)
#pragma pack(push, 8)
#endif

typedef struct DMUI_ImGuiFingerprint
{
	uint32_t structSize;
	char upstreamCommit[41];
	uint32_t imguiVersionNum;
	uint32_t flags;
	uint32_t sizeOfImGuiIO;
	uint32_t sizeOfImGuiStyle;
	uint32_t sizeOfImVec2;
	uint32_t sizeOfImVec4;
	uint32_t sizeOfImDrawVert;
	uint32_t sizeOfImDrawIdx;
	uint32_t alignOfImGuiIO;
	uint32_t alignOfImGuiStyle;
	uint32_t alignOfImVec2;
	uint32_t alignOfImVec4;
	uint32_t alignOfImDrawVert;
	uint32_t alignOfImDrawIdx;
	uint32_t sizeOfImWchar;
	uint32_t alignOfImWchar;
	uint32_t sizeOfImTextureID;
	uint32_t alignOfImTextureID;
	uint32_t sizeOfImGuiID;
	uint32_t alignOfImGuiID;
	uint32_t sizeOfImFont;
	uint32_t alignOfImFont;
	uint32_t sizeOfImFontConfig;
	uint32_t alignOfImFontConfig;
	uint32_t sizeOfImFontGlyph;
	uint32_t alignOfImFontGlyph;
	uint32_t sizeOfImGuiContext;
	uint32_t alignOfImGuiContext;
	uint32_t sizeOfImGuiErrorRecoveryState;
	uint32_t alignOfImGuiErrorRecoveryState;
	uint32_t sizeOfImGuiNextWindowData;
	uint32_t alignOfImGuiNextWindowData;
	uint32_t sizeOfImGuiNextItemData;
	uint32_t alignOfImGuiNextItemData;
	uint32_t sizeOfImGuiPopupData;
	uint32_t alignOfImGuiPopupData;
	uint32_t offsetOfImDrawVertPos;
	uint32_t offsetOfImDrawVertUv;
	uint32_t offsetOfImDrawVertCol;
	uint64_t layoutSignature;
} DMUI_ImGuiFingerprint;

typedef void* (DMUI_CALL *DMUI_ImGuiAllocFn)(size_t size, void* userData) DMUI_NOEXCEPT;
typedef void (DMUI_CALL *DMUI_ImGuiFreeFn)(void* allocation, void* userData) DMUI_NOEXCEPT;

typedef struct DMUI_HostReadyInfo
{
	uint32_t structSize;
	uint32_t apiVersion;
	void* imguiContext;
	DMUI_ImGuiAllocFn imguiAlloc;
	DMUI_ImGuiFreeFn imguiFree;
	void* imguiAllocatorUserData;
} DMUI_HostReadyInfo;

typedef void (DMUI_CALL *DMUI_HostReadyCallback)(
	const DMUI_HostReadyInfo* info,
	void* userData);
typedef void (DMUI_CALL *DMUI_HostUnavailableCallback)(
	DMUI_UnavailableReason reason,
	void* userData);
typedef void (DMUI_CALL *DMUI_PageDrawCallback)(void* userData);
typedef void (DMUI_CALL *DMUI_ActionCallback)(void* userData);
typedef struct DMUI_PageActivityInfo DMUI_PageActivityInfo;
typedef void (DMUI_CALL *DMUI_PageActivityCallback)(
	const DMUI_PageActivityInfo* info,
	void* userData);
// Frame callbacks run on the render thread and cannot be unregistered.
typedef void (DMUI_CALL *DMUI_FrameCallback)(void* userData);
// Hotkey callbacks run on the render thread, beside frame observers.
// Handlers must return promptly: blocking I/O or long work costs frame time directly.
// Edges dispatch FIFO, are never collapsed, and persist across stalled frames.
// Auto-repeat is coalesced, so each physical press delivers exactly one press edge.
// On overflow the bounded queue drops and logs only whole press/release pairs.
// Unregistering cancels queued edges, so the pair guarantee ends when the action is removed.
// A callback may unregister its own action; keep userData valid until that callback returns.
// Otherwise clients own userData and must keep it valid until unregister returns.
typedef void (DMUI_CALL *DMUI_HotkeyCallback)(
	DMUI_HotkeyActionHandle action,
	uint32_t pressed,
	void* userData);

typedef struct DMUI_ClientDescriptor
{
	uint32_t structSize;
	uint32_t apiVersion;
	const char* id;
	const char* displayName;
	uint32_t version;
	// Null selects layout-independent forwarding instead of a shared ImGui context.
	const DMUI_ImGuiFingerprint* expectedImGui;
	DMUI_HostReadyCallback onHostReady;
	DMUI_HostUnavailableCallback onHostUnavailable;
	void* userData;
	DMUI_ClientCapabilities capabilities;
	const char* iconName;
	DMUI_ClientOrigin origin;
	const char* bridgeSourceLabel;
} DMUI_ClientDescriptor;

#define DMUI_CLIENT_DESCRIPTOR_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_ClientDescriptor, bridgeSourceLabel) + sizeof(const char*)))

typedef struct DMUI_PageDescriptor
{
	uint32_t structSize;
	const char* id;
	const char* displayName;
	const char* category;
	const char* summary;
	int32_t sortKey;
	DMUI_PageKind kind;
	DMUI_PageDrawCallback draw;
	void* userData;
} DMUI_PageDescriptor;

#define DMUI_PAGE_DESCRIPTOR_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_PageDescriptor, userData) + sizeof(void*)))

typedef struct DMUI_ActionDescriptor
{
	uint32_t structSize;
	const char* id;
	const char* displayLabel;
	const char* iconName;
	const char* tooltip;
	int32_t sortKey;
	DMUI_ActionCallback callback;
	void* userData;
} DMUI_ActionDescriptor;

#define DMUI_ACTION_DESCRIPTOR_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_ActionDescriptor, userData) + sizeof(void*)))

typedef struct DMUI_FrameObserverDescriptor
{
	uint32_t structSize;
	DMUI_FrameCallback callback;
	void* userData;
} DMUI_FrameObserverDescriptor;

#define DMUI_FRAME_OBSERVER_DESCRIPTOR_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_FrameObserverDescriptor, userData) + sizeof(void*)))

typedef struct DMUI_HotkeyActionDescriptor
{
	uint32_t structSize;
	const char* id;
	const char* displayName;
	const char* suggestedDefaultChord;
	DMUI_HotkeyCallback callback;
	void* userData;
} DMUI_HotkeyActionDescriptor;

#define DMUI_HOTKEY_ACTION_DESCRIPTOR_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_HotkeyActionDescriptor, userData) + sizeof(void*)))

typedef struct DMUI_LinkDescriptor
{
	uint32_t structSize;
	const char* label;
	const char* url;
	const char* note;
	uint32_t glyph;
	uint32_t enabled;
} DMUI_LinkDescriptor;

#define DMUI_LINK_DESCRIPTOR_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_LinkDescriptor, enabled) + sizeof(uint32_t)))

typedef struct DMUI_FaqEntry
{
	uint32_t structSize;
	const char* question;
	const char* answer;
} DMUI_FaqEntry;

#define DMUI_FAQ_ENTRY_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_FaqEntry, answer) + sizeof(const char*)))

typedef struct DMUI_DiagnosticDescriptor
{
	uint32_t structSize;
	DMUI_StatusSeverity severity;
	const char* scope;
	const char* summary;
	const char* detail;
} DMUI_DiagnosticDescriptor;

#define DMUI_DIAGNOSTIC_DESCRIPTOR_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_DiagnosticDescriptor, detail) + sizeof(const char*)))

typedef struct DMUI_PageActivityInfo
{
	uint32_t structSize;
	DMUI_PageActivityKind kind;
	DMUI_PageHandle previousPage;
	DMUI_PageHandle activePage;
} DMUI_PageActivityInfo;

#define DMUI_PAGE_ACTIVITY_INFO_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_PageActivityInfo, activePage) + sizeof(DMUI_PageHandle)))

typedef struct DMUI_PageActivityObserverDescriptor
{
	uint32_t structSize;
	DMUI_PageActivityCallback callback;
	void* userData;
} DMUI_PageActivityObserverDescriptor;

#define DMUI_PAGE_ACTIVITY_OBSERVER_DESCRIPTOR_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_PageActivityObserverDescriptor, userData) + sizeof(void*)))

typedef struct DMUI_HotkeyBindingInfo
{
	uint32_t structSize;
	DMUI_HotkeyBindingState state;
	char chord[32];
} DMUI_HotkeyBindingInfo;

typedef struct DMUI_HostStateInfo
{
	uint32_t structSize;
	DMUI_HostState state;
	DMUI_UnavailableReason unavailableReason;
	uint32_t registrationOpen;
	uint32_t clientCount;
	uint32_t pageCount;
	uint32_t demandedOverlayCount;
} DMUI_HostStateInfo;

typedef struct DMUI_Vec2
{
	float x;
	float y;
} DMUI_Vec2;

typedef struct DMUI_Vec4
{
	float x;
	float y;
	float z;
	float w;
} DMUI_Vec4;

typedef struct DMUI_StyleMetrics
{
	uint32_t structSize;
	DMUI_Vec2 itemSpacing;
	DMUI_Vec2 framePadding;
	DMUI_Vec2 itemInnerSpacing;
	DMUI_Vec2 cellPadding;
	DMUI_Vec2 windowPadding;
	float indentSpacing;
	float scrollbarSize;
} DMUI_StyleMetrics;

#define DMUI_STYLE_METRICS_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_StyleMetrics, scrollbarSize) + sizeof(float)))

typedef struct DMUI_SettingsRowOptions
{
	uint32_t structSize;
	uint32_t resetVisible;
	uint32_t resetEnabled;
} DMUI_SettingsRowOptions;

#define DMUI_SETTINGS_ROW_OPTIONS_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_SettingsRowOptions, resetEnabled) + sizeof(uint32_t)))

typedef struct DMUI_SettingsRowBeginOptions
{
	uint32_t structSize;
	DMUI_SettingsRowLayout layout;
} DMUI_SettingsRowBeginOptions;

#define DMUI_SETTINGS_ROW_BEGIN_OPTIONS_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_SettingsRowBeginOptions, layout) + sizeof(DMUI_SettingsRowLayout)))

typedef struct DMUI_ThemeColors
{
	uint32_t structSize;
	DMUI_Vec4 success;
	DMUI_Vec4 warning;
	DMUI_Vec4 error;
	DMUI_Vec4 info;
	DMUI_Vec4 muted;
	DMUI_Vec4 accent;
	DMUI_Vec4 accentMuted;
	DMUI_Vec4 statusDisable;
	DMUI_Vec4 statusError;
	DMUI_Vec4 statusWarning;
	DMUI_Vec4 statusRestartNeeded;
	DMUI_Vec4 statusCurrentHotkey;
	DMUI_Vec4 statusSuccess;
	DMUI_Vec4 statusInfo;
} DMUI_ThemeColors;

#define DMUI_THEME_COLORS_0_1_SIZE \
	((uint32_t)(offsetof(DMUI_ThemeColors, statusInfo) + sizeof(DMUI_Vec4)))

typedef DMUI_Result (DMUI_CALL *DMUI_RegisterClientFn)(
	const DMUI_ClientDescriptor* descriptor,
	DMUI_ClientHandle* client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_RegisterPageFn)(
	DMUI_ClientHandle client,
	const DMUI_PageDescriptor* descriptor,
	DMUI_PageHandle* page) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_QueryStateFn)(
	DMUI_HostStateInfo* state) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_RequestFrameFn)(
	DMUI_ClientHandle client,
	DMUI_PageHandle page) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_ReleaseFrameFn)(
	DMUI_ClientHandle client,
	DMUI_PageHandle page) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_IsMenuVisibleFn)(
	uint32_t* visible) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_SelectPageFn)(
	DMUI_ClientHandle client,
	DMUI_PageHandle page) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_AttachSwapChainFn)(
	DMUI_ClientHandle client,
	void* nativeSwapChain) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_RegisterActionFn)(
	DMUI_ClientHandle client,
	const DMUI_ActionDescriptor* descriptor,
	DMUI_ActionHandle* action) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_SetStatusFn)(
	DMUI_ClientHandle client,
	DMUI_StatusSeverity severity,
	const char* message) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_GetThemeColorsFn)(
	DMUI_ClientHandle client,
	DMUI_ThemeColors* colors) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_PushFontFn)(
	DMUI_ClientHandle client,
	DMUI_FontRole role) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_PopFontFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_DrawSectionHeaderFn)(
	DMUI_ClientHandle client,
	const char* text,
	uint32_t glyph) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_DrawBulletTextFn)(
	DMUI_ClientHandle client,
	const char* text) DMUI_NOEXCEPT;
// buffer must be NUL-terminated within capacity bytes, and capacity zero is invalid.
// On success, buffer stays NUL-terminated and output longer than capacity is truncated to fit.
// changed is set when the edited text differs from the input, including a truncated edit.
typedef DMUI_Result (DMUI_CALL *DMUI_DrawSearchInputFn)(
	DMUI_ClientHandle client,
	const char* id,
	const char* hint,
	char* buffer,
	size_t capacity,
	uint32_t* changed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_DrawCollapsingSectionHeaderFn)(
	DMUI_ClientHandle client,
	const char* key,
	const char* text,
	uint32_t glyph,
	uint32_t* expanded,
	size_t count) DMUI_NOEXCEPT;
// Link rows are render-thread-only and valid only inside page draw callbacks.
// Buttons share the available width; enabled clicks copy the URL without launching a browser.
// Tooltips use a non-empty note when present, otherwise the URL, including for disabled links.
// A zero count succeeds without drawing.
// Null required values, empty labels or enabled URLs, and short descriptors are invalid.
typedef DMUI_Result (DMUI_CALL *DMUI_DrawLinkRowFn)(
	DMUI_ClientHandle client,
	const char* id,
	const DMUI_LinkDescriptor* links,
	size_t count) DMUI_NOEXCEPT;
// FAQ rows are render-thread-only and valid only inside page draw callbacks.
// The host owns presentation and expansion state, keyed by the row ID and question.
// A zero count succeeds without drawing; null, empty, and short entries are invalid.
typedef DMUI_Result (DMUI_CALL *DMUI_DrawFaqFn)(
	DMUI_ClientHandle client,
	const char* id,
	const DMUI_FaqEntry* entries,
	size_t count) DMUI_NOEXCEPT;
// Diagnostics may be reported from any thread, including during client loading.
// The host copies descriptor strings before returning and aggregates matching reports.
// Scope and detail are optional; null descriptors, empty summaries, unknown severities,
// and short descriptors are invalid.
typedef DMUI_Result (DMUI_CALL *DMUI_ReportDiagnosticFn)(
	DMUI_ClientHandle client,
	const DMUI_DiagnosticDescriptor* diagnostic) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_DrawSettingsActionButtonFn)(
	DMUI_ClientHandle client,
	const char* id,
	DMUI_Vec2 origin,
	DMUI_Vec2 size,
	DMUI_SettingsAction action,
	const char* fallbackLabel,
	const char* tooltip,
	uint32_t enabled,
	uint32_t* pressed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_SettingsActionButtonWidthFn)(
	DMUI_ClientHandle client,
	DMUI_SettingsAction action,
	const char* fallbackLabel,
	float buttonExtent,
	float* width) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_SettingsActionButtonExtentFn)(
	DMUI_ClientHandle client,
	float* extent) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_RegisterFrameObserverFn)(
	DMUI_ClientHandle client,
	const DMUI_FrameObserverDescriptor* descriptor,
	DMUI_FrameObserverHandle* observer) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_QueryVideoMemoryFn)(
	DMUI_ClientHandle client,
	uint64_t* used,
	uint64_t* budget) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_RegisterHotkeyActionFn)(
	DMUI_ClientHandle client,
	const DMUI_HotkeyActionDescriptor* descriptor,
	DMUI_HotkeyActionHandle* action) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_QueryHotkeyBindingFn)(
	DMUI_ClientHandle client,
	DMUI_HotkeyActionHandle action,
	DMUI_HotkeyBindingInfo* binding) DMUI_NOEXCEPT;
// Registration accepts any thread; unregister must run on the render thread, or returns
// DMUI_RESULT_WRONG_THREAD. The asymmetry exists for userData lifetime, not container safety:
// dispatch also runs on the render thread, so no callback can be in flight when unregister returns.
// No later callback for the action runs after unregister returns successfully.
// The saved override remains orphaned and is reapplied if the action is registered again.
// A key held across unregister releases against the old handle, so a re-registered action
// never receives a release edge for a press it did not observe.
typedef DMUI_Result (DMUI_CALL *DMUI_UnregisterHotkeyActionFn)(
	DMUI_ClientHandle client,
	DMUI_HotkeyActionHandle action) DMUI_NOEXCEPT;
// Settings-table brackets are render-thread-only, non-nestable, and valid only in page callbacks.
// A successful begin with visible == 0 opens no bracket and requires no matching end.
// A successful begin with visible != 0 must be matched by its corresponding end.
typedef DMUI_Result (DMUI_CALL *DMUI_BeginSettingsTableFn)(
	DMUI_ClientHandle client,
	const char* id,
	uint32_t* visible) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_BeginSettingsRowFn)(
	DMUI_ClientHandle client,
	const char* id,
	const char* label,
	const char* description,
	uint32_t* visible) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_EndSettingsRowFn)(
	DMUI_ClientHandle client,
	const DMUI_SettingsRowOptions* options,
	uint32_t* resetPressed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_EndSettingsTableFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_BeginSettingsRowExFn)(
	DMUI_ClientHandle client,
	const char* id,
	const char* label,
	const char* description,
	const DMUI_SettingsRowBeginOptions* options,
	uint32_t* visible) DMUI_NOEXCEPT;
// Page activity observers are process-lifetime registrations invoked during shell drawing on the render thread.
typedef DMUI_Result (DMUI_CALL *DMUI_RegisterPageActivityObserverFn)(
	DMUI_ClientHandle client,
	const DMUI_PageActivityObserverDescriptor* descriptor,
	DMUI_PageActivityObserverHandle* observer) DMUI_NOEXCEPT;

typedef struct DMUI_HostAPI
{
	uint32_t structSize;
	uint32_t apiVersion;
	const DMUI_ImGuiFingerprint* imguiFingerprint;
	DMUI_RegisterClientFn registerClient;
	DMUI_RegisterPageFn registerPage;
	DMUI_QueryStateFn queryState;
	DMUI_RequestFrameFn requestFrame;
	DMUI_ReleaseFrameFn releaseFrame;
	DMUI_IsMenuVisibleFn isMenuVisible;
	DMUI_SelectPageFn selectPage;
	DMUI_AttachSwapChainFn attachSwapChain;
	DMUI_RegisterActionFn registerAction;
	DMUI_SetStatusFn setStatus;
	DMUI_GetThemeColorsFn getThemeColors;
	DMUI_PushFontFn pushFont;
	DMUI_PopFontFn popFont;
	DMUI_DrawSectionHeaderFn drawSectionHeader;
	DMUI_DrawSearchInputFn drawSearchInput;
	DMUI_DrawCollapsingSectionHeaderFn drawCollapsingSectionHeader;
	DMUI_DrawSettingsActionButtonFn drawSettingsActionButton;
	DMUI_SettingsActionButtonWidthFn settingsActionButtonWidth;
	DMUI_SettingsActionButtonExtentFn settingsActionButtonExtent;
	DMUI_RegisterFrameObserverFn registerFrameObserver;
	DMUI_QueryVideoMemoryFn queryVideoMemory;
	DMUI_DrawBulletTextFn drawBulletText;
	DMUI_RegisterHotkeyActionFn registerHotkeyAction;
	DMUI_QueryHotkeyBindingFn queryHotkeyBinding;
	DMUI_UnregisterHotkeyActionFn unregisterHotkeyAction;
	DMUI_BeginSettingsTableFn beginSettingsTable;
	DMUI_BeginSettingsRowFn beginSettingsRow;
	DMUI_EndSettingsRowFn endSettingsRow;
	DMUI_EndSettingsTableFn endSettingsTable;
	DMUI_BeginSettingsRowExFn beginSettingsRowEx;
	DMUI_RegisterPageActivityObserverFn registerPageActivityObserver;
	DMUI_DrawLinkRowFn drawLinkRow;
	DMUI_DrawFaqFn drawFaq;
	DMUI_ReportDiagnosticFn reportDiagnostic;
} DMUI_HostAPI;

#define DMUI_HOST_API_SELECT_PAGE_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, selectPage) + sizeof(DMUI_SelectPageFn)))
#define DMUI_HOST_API_ATTACH_SWAP_CHAIN_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, attachSwapChain) + sizeof(DMUI_AttachSwapChainFn)))
#define DMUI_HOST_API_REGISTER_ACTION_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, registerAction) + sizeof(DMUI_RegisterActionFn)))
#define DMUI_HOST_API_SET_STATUS_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, setStatus) + sizeof(DMUI_SetStatusFn)))
#define DMUI_HOST_API_GET_THEME_COLORS_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, getThemeColors) + sizeof(DMUI_GetThemeColorsFn)))
#define DMUI_HOST_API_PUSH_FONT_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, pushFont) + sizeof(DMUI_PushFontFn)))
#define DMUI_HOST_API_POP_FONT_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, popFont) + sizeof(DMUI_PopFontFn)))
#define DMUI_HOST_API_DRAW_SECTION_HEADER_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, drawSectionHeader) + sizeof(DMUI_DrawSectionHeaderFn)))
#define DMUI_HOST_API_DRAW_SEARCH_INPUT_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, drawSearchInput) + sizeof(DMUI_DrawSearchInputFn)))
#define DMUI_HOST_API_DRAW_COLLAPSING_SECTION_HEADER_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, drawCollapsingSectionHeader) + sizeof(DMUI_DrawCollapsingSectionHeaderFn)))
#define DMUI_HOST_API_DRAW_SETTINGS_ACTION_BUTTON_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, drawSettingsActionButton) + sizeof(DMUI_DrawSettingsActionButtonFn)))
#define DMUI_HOST_API_SETTINGS_ACTION_BUTTON_WIDTH_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, settingsActionButtonWidth) + sizeof(DMUI_SettingsActionButtonWidthFn)))
#define DMUI_HOST_API_SETTINGS_ACTION_BUTTON_EXTENT_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, settingsActionButtonExtent) + sizeof(DMUI_SettingsActionButtonExtentFn)))
#define DMUI_HOST_API_REGISTER_FRAME_OBSERVER_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, registerFrameObserver) + sizeof(DMUI_RegisterFrameObserverFn)))
#define DMUI_HOST_API_QUERY_VIDEO_MEMORY_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, queryVideoMemory) + sizeof(DMUI_QueryVideoMemoryFn)))
#define DMUI_HOST_API_DRAW_BULLET_TEXT_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, drawBulletText) + sizeof(DMUI_DrawBulletTextFn)))
#define DMUI_HOST_API_REGISTER_HOTKEY_ACTION_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, registerHotkeyAction) + sizeof(DMUI_RegisterHotkeyActionFn)))
#define DMUI_HOST_API_QUERY_HOTKEY_BINDING_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, queryHotkeyBinding) + sizeof(DMUI_QueryHotkeyBindingFn)))
#define DMUI_HOST_API_UNREGISTER_HOTKEY_ACTION_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, unregisterHotkeyAction) + sizeof(DMUI_UnregisterHotkeyActionFn)))
#define DMUI_HOST_API_BEGIN_SETTINGS_TABLE_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, beginSettingsTable) + sizeof(DMUI_BeginSettingsTableFn)))
#define DMUI_HOST_API_BEGIN_SETTINGS_ROW_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, beginSettingsRow) + sizeof(DMUI_BeginSettingsRowFn)))
#define DMUI_HOST_API_END_SETTINGS_ROW_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, endSettingsRow) + sizeof(DMUI_EndSettingsRowFn)))
#define DMUI_HOST_API_END_SETTINGS_TABLE_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, endSettingsTable) + sizeof(DMUI_EndSettingsTableFn)))
#define DMUI_HOST_API_BEGIN_SETTINGS_ROW_EX_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, beginSettingsRowEx) + sizeof(DMUI_BeginSettingsRowExFn)))
#define DMUI_HOST_API_REGISTER_PAGE_ACTIVITY_OBSERVER_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, registerPageActivityObserver) + sizeof(DMUI_RegisterPageActivityObserverFn)))
#define DMUI_HOST_API_DRAW_LINK_ROW_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, drawLinkRow) + sizeof(DMUI_DrawLinkRowFn)))
#define DMUI_HOST_API_DRAW_FAQ_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, drawFaq) + sizeof(DMUI_DrawFaqFn)))
#define DMUI_HOST_API_REPORT_DIAGNOSTIC_SIZE \
	((uint32_t)(offsetof(DMUI_HostAPI, reportDiagnostic) + sizeof(DMUI_ReportDiagnosticFn)))

#if defined(_MSC_VER)
#pragma pack(pop)
#endif

DMUI_EXPORT DMUI_Result DMUI_CALL DMUI_GetStyleMetrics(
	DMUI_StyleMetrics* metrics) DMUI_NOEXCEPT;
DMUI_EXPORT uint32_t DMUI_CALL DMUI_GetImGuiVersionNum(void) DMUI_NOEXCEPT;
DMUI_EXPORT const DMUI_HostAPI* DMUI_CALL DMUI_GetHostAPI(
	uint32_t requestedVersion) DMUI_NOEXCEPT;
