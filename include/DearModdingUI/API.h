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

#define DMUI_ABI_MAJOR 3u
#define DMUI_ABI_MINOR 0u
// Minor occupies the high half so a .0 version equals its bare major.
#define DMUI_MAKE_ABI_VERSION(major, minor) ((((uint32_t)(major)) & 0xFFFFu) | (((uint32_t)(minor)) << 16u))
#define DMUI_ABI_VERSION_MAJOR(version) ((uint32_t)(version) & 0xFFFFu)
#define DMUI_ABI_VERSION_MINOR(version) ((uint32_t)(version) >> 16u)
#define DMUI_ABI_VERSION DMUI_MAKE_ABI_VERSION(DMUI_ABI_MAJOR, DMUI_ABI_MINOR)

typedef uint32_t DMUI_Result;

#define DMUI_RESULT_OK 0u
#define DMUI_RESULT_UNSUPPORTED_ABI 1u
#define DMUI_RESULT_INVALID_ARGUMENT 2u
#define DMUI_RESULT_INVALID_DESCRIPTOR 4u
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
#define DMUI_RESULT_UNSUPPORTED_RESOURCE 30u
#define DMUI_RESULT_STALE_HANDLE 31u
#define DMUI_RESULT_BUSY 32u
#define DMUI_RESULT_BUFFER_TOO_SMALL 33u
#define DMUI_RESULT_NOT_VISIBLE 34u
#define DMUI_RESULT_STALE_SUBMISSION 35u
#define DMUI_RESULT_DUPLICATE_CATEGORY_ID 37u
#define DMUI_RESULT_CATEGORY_NOT_FOUND 38u
#define DMUI_RESULT_EXTERNAL_OPEN_FAILED 39u
#define DMUI_RESULT_EXTERNAL_RESOLUTION_FAILED 40u
#define DMUI_RESULT_EXTERNAL_RESOLUTION_UNSUPPORTED 41u
#define DMUI_RESULT_FILE_NOT_FOUND 42u
#define DMUI_RESULT_ACCESS_DENIED 43u
#define DMUI_RESULT_IMAGE_TOO_LARGE 44u
#define DMUI_RESULT_IMAGE_DECODE_FAILED 45u
#define DMUI_RESULT_IMAGE_DEVICE_FAILED 46u
#define DMUI_RESULT_TEXT_NOT_FOUND 47u

static inline const char* DMUI_ResultToString(DMUI_Result result) DMUI_NOEXCEPT
{
	switch (result)
	{
	case DMUI_RESULT_FILE_NOT_FOUND:
		return "FILE_NOT_FOUND";
	case DMUI_RESULT_TEXT_NOT_FOUND:
		return "TEXT_NOT_FOUND";
	case DMUI_RESULT_ACCESS_DENIED:
		return "ACCESS_DENIED";
	case DMUI_RESULT_IMAGE_TOO_LARGE:
		return "IMAGE_TOO_LARGE";
	case DMUI_RESULT_IMAGE_DECODE_FAILED:
		return "IMAGE_DECODE_FAILED";
	case DMUI_RESULT_IMAGE_DEVICE_FAILED:
		return "IMAGE_DEVICE_FAILED";
	case DMUI_RESULT_OK:
		return "OK";
	case DMUI_RESULT_UNSUPPORTED_ABI:
		return "UNSUPPORTED_ABI";
	case DMUI_RESULT_INVALID_ARGUMENT:
		return "INVALID_ARGUMENT";
	case DMUI_RESULT_INVALID_DESCRIPTOR:
		return "INVALID_DESCRIPTOR";
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
	case DMUI_RESULT_UNSUPPORTED_RESOURCE:
		return "UNSUPPORTED_RESOURCE";
	case DMUI_RESULT_STALE_HANDLE:
		return "STALE_HANDLE";
	case DMUI_RESULT_BUSY:
		return "BUSY";
	case DMUI_RESULT_BUFFER_TOO_SMALL:
		return "BUFFER_TOO_SMALL";
	case DMUI_RESULT_NOT_VISIBLE:
		return "NOT_VISIBLE";
	case DMUI_RESULT_STALE_SUBMISSION:
		return "STALE_SUBMISSION";
	case DMUI_RESULT_DUPLICATE_CATEGORY_ID:
		return "DUPLICATE_CATEGORY_ID";
	case DMUI_RESULT_CATEGORY_NOT_FOUND:
		return "CATEGORY_NOT_FOUND";
	case DMUI_RESULT_EXTERNAL_OPEN_FAILED:
		return "EXTERNAL_OPEN_FAILED";
	case DMUI_RESULT_EXTERNAL_RESOLUTION_FAILED:
		return "EXTERNAL_RESOLUTION_FAILED";
	case DMUI_RESULT_EXTERNAL_RESOLUTION_UNSUPPORTED:
		return "EXTERNAL_RESOLUTION_UNSUPPORTED";
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

typedef uint32_t DMUI_Severity;

#define DMUI_SEVERITY_INFO 0u
#define DMUI_SEVERITY_SUCCESS 1u
#define DMUI_SEVERITY_WARNING 2u
#define DMUI_SEVERITY_ERROR 3u

typedef uint32_t DMUI_FontRole;

#define DMUI_FONT_ROLE_BODY 0u
#define DMUI_FONT_ROLE_TITLE 1u
#define DMUI_FONT_ROLE_HEADING 2u
#define DMUI_FONT_ROLE_SUBHEADING 3u
#define DMUI_FONT_ROLE_SUBTEXT 4u
#define DMUI_FONT_ROLE_MONOSPACE 5u
#define DMUI_FONT_ROLE_COUNT 6u

typedef uint32_t DMUI_TextTone;

#define DMUI_TEXT_TONE_INHERIT 0u
#define DMUI_TEXT_TONE_MUTED 1u
#define DMUI_TEXT_TONE_ACCENT 2u
#define DMUI_TEXT_TONE_ACCENT_MUTED 3u
#define DMUI_TEXT_TONE_INFO 4u
#define DMUI_TEXT_TONE_SUCCESS 5u
#define DMUI_TEXT_TONE_WARNING 6u
#define DMUI_TEXT_TONE_ERROR 7u
#define DMUI_TEXT_TONE_DISABLED 8u
#define DMUI_TEXT_TONE_RESTART_NEEDED 9u
#define DMUI_TEXT_TONE_CURRENT_HOTKEY 10u
#define DMUI_TEXT_TONE_COUNT 11u

typedef uint32_t DMUI_TextSpanFlags;

#define DMUI_TEXT_SPAN_BOLD 0x00000001u
#define DMUI_TEXT_SPAN_ITALIC 0x00000002u
#define DMUI_TEXT_SPAN_UNDERLINE 0x00000004u
#define DMUI_TEXT_SPAN_STRIKETHROUGH 0x00000008u
#define DMUI_TEXT_SPAN_INTERACTIVE 0x00000010u
#define DMUI_TEXT_SPAN_HAS_ROLE 0x00000020u
#define DMUI_TEXT_SPAN_CUSTOM_FOREGROUND 0x00000040u
#define DMUI_TEXT_SPAN_CUSTOM_BACKGROUND 0x00000080u

typedef uint32_t DMUI_TextOptionFlags;

#define DMUI_TEXT_OPTION_WRAP 0x00000001u
#define DMUI_TEXT_OPTION_BULLET 0x00000002u

typedef uint32_t DMUI_TextViewFlags;

#define DMUI_TEXT_VIEW_FOLLOW_TAIL 0x00000001u
#define DMUI_TEXT_VIEW_SELECTABLE 0x00000002u

typedef uint32_t DMUI_TextViewScrollCommand;

#define DMUI_TEXT_VIEW_SCROLL_NONE 0u
#define DMUI_TEXT_VIEW_SCROLL_PAGE_UP 1u
#define DMUI_TEXT_VIEW_SCROLL_PAGE_DOWN 2u
#define DMUI_TEXT_VIEW_SCROLL_TOP 3u
#define DMUI_TEXT_VIEW_SCROLL_BOTTOM 4u
#define DMUI_TEXT_VIEW_SCROLL_COUNT 5u

typedef uint32_t DMUI_TextViewEvents;

#define DMUI_TEXT_VIEW_EVENT_SPAN_HOVERED 0x00000001u
#define DMUI_TEXT_VIEW_EVENT_SPAN_CLICKED 0x00000002u

typedef uint32_t DMUI_ButtonStyle;

#define DMUI_BUTTON_STYLE_DEFAULT 0u
#define DMUI_BUTTON_STYLE_PRIMARY 1u
#define DMUI_BUTTON_STYLE_SUBTLE 2u

typedef uint32_t DMUI_TextFieldStyle;

#define DMUI_TEXT_FIELD_STYLE_PLAIN 0u
#define DMUI_TEXT_FIELD_STYLE_SEARCH 1u

typedef uint32_t DMUI_NumericType;

#define DMUI_NUMERIC_TYPE_S8 1u
#define DMUI_NUMERIC_TYPE_U8 2u
#define DMUI_NUMERIC_TYPE_S16 3u
#define DMUI_NUMERIC_TYPE_U16 4u
#define DMUI_NUMERIC_TYPE_S32 5u
#define DMUI_NUMERIC_TYPE_U32 6u
#define DMUI_NUMERIC_TYPE_S64 7u
#define DMUI_NUMERIC_TYPE_U64 8u
#define DMUI_NUMERIC_TYPE_FLOAT 9u
#define DMUI_NUMERIC_TYPE_DOUBLE 10u

typedef uint32_t DMUI_NumericPresentation;

#define DMUI_NUMERIC_PRESENTATION_SLIDER 0u
#define DMUI_NUMERIC_PRESENTATION_DRAG 1u
#define DMUI_NUMERIC_PRESENTATION_INPUT 2u

typedef uint32_t DMUI_NumericFlags;

#define DMUI_NUMERIC_HAS_MINIMUM 0x00000001u
#define DMUI_NUMERIC_HAS_MAXIMUM 0x00000002u
#define DMUI_NUMERIC_HAS_STEP 0x00000004u
#define DMUI_NUMERIC_CLAMP 0x00000008u

typedef uint32_t DMUI_ColorEditFlags;

#define DMUI_COLOR_EDIT_ALPHA 0x00000001u

typedef uint32_t DMUI_KeyCaptureSlot;

#define DMUI_KEY_CAPTURE_SLOT_KEYBOARD_MOUSE 0u
#define DMUI_KEY_CAPTURE_SLOT_GAMEPAD 1u

typedef uint32_t DMUI_KeyCaptureFlags;

#define DMUI_KEY_CAPTURE_ALLOW_CLEAR 0x00000001u
#define DMUI_KEY_CAPTURE_READ_ONLY 0x00000002u

// Room for the longest chord the host accepts, with its terminator.
#define DMUI_KEY_CHORD_CAPACITY 32u

typedef uint32_t DMUI_SectionFlags;

#define DMUI_SECTION_BOX 0x00000001u
#define DMUI_SECTION_HAS_COUNT 0x00000002u

// A glyph of zero means "infer from the title"; this value means no icon.
#define DMUI_GLYPH_NONE 0xFFFFFFFFu

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
typedef uint64_t DMUI_ImageHandle;
typedef uint64_t DMUI_DialogHandle;

#define DMUI_INVALID_CLIENT_HANDLE ((DMUI_ClientHandle)0u)
#define DMUI_INVALID_PAGE_HANDLE ((DMUI_PageHandle)0u)
#define DMUI_INVALID_ACTION_HANDLE ((DMUI_ActionHandle)0u)
#define DMUI_INVALID_FRAME_OBSERVER_HANDLE ((DMUI_FrameObserverHandle)0u)
#define DMUI_INVALID_HOTKEY_ACTION_HANDLE ((DMUI_HotkeyActionHandle)0u)
#define DMUI_INVALID_PAGE_ACTIVITY_OBSERVER_HANDLE ((DMUI_PageActivityObserverHandle)0u)
#define DMUI_INVALID_IMAGE_HANDLE ((DMUI_ImageHandle)0u)
#define DMUI_INVALID_DIALOG_HANDLE ((DMUI_DialogHandle)0u)

typedef uint32_t DMUI_PageActivityKind;

#define DMUI_PAGE_ACTIVITY_ACTIVATED 1u
#define DMUI_PAGE_ACTIVITY_CHANGED 2u
#define DMUI_PAGE_ACTIVITY_DEACTIVATED 3u

typedef uint32_t DMUI_FieldLayout;

#define DMUI_FIELD_LAYOUT_LABEL_VALUE 0u
#define DMUI_FIELD_LAYOUT_FULL_SPAN 1u

#define DMUI_FIELD_FEEDBACK_MAX_MESSAGE_BYTES 16384u

typedef uint32_t DMUI_HotkeyContextPolicy;

#define DMUI_HOTKEY_CONTEXT_ALWAYS 0u
#define DMUI_HOTKEY_CONTEXT_HOST_INPUT_INACTIVE 1u
#define DMUI_HOTKEY_CONTEXT_GAMEPLAY_UNOBSTRUCTED 2u

typedef uint32_t DMUI_ImageStatus;

#define DMUI_IMAGE_STATUS_READY 0u
#define DMUI_IMAGE_STATUS_INVALIDATED 1u
#define DMUI_IMAGE_STATUS_RELEASED 2u
#define DMUI_IMAGE_STATUS_LOADING 3u
#define DMUI_IMAGE_STATUS_FAILED 4u

typedef uint32_t DMUI_PixelFormat;

// Bytes are ordered R, G, B, A. Alpha is straight, not premultiplied.
#define DMUI_PIXEL_FORMAT_RGBA8_UNORM 1u

typedef uint32_t DMUI_OverlayAnchor;

#define DMUI_OVERLAY_ANCHOR_TOP_LEFT 0u
#define DMUI_OVERLAY_ANCHOR_TOP_RIGHT 1u
#define DMUI_OVERLAY_ANCHOR_BOTTOM_LEFT 2u
#define DMUI_OVERLAY_ANCHOR_BOTTOM_RIGHT 3u
#define DMUI_OVERLAY_ANCHOR_FREE 4u

typedef uint32_t DMUI_OverlayFocusEndReason;

#define DMUI_OVERLAY_FOCUS_END_NONE 0u
#define DMUI_OVERLAY_FOCUS_END_RELEASED 1u
#define DMUI_OVERLAY_FOCUS_END_CANCELED 2u
#define DMUI_OVERLAY_FOCUS_END_SHELL_OPENED 3u
#define DMUI_OVERLAY_FOCUS_END_INTERRUPTED 4u
#define DMUI_OVERLAY_FOCUS_END_HOST_UNAVAILABLE 5u
#define DMUI_OVERLAY_FOCUS_END_CALLBACK_FAILED 6u

typedef uint32_t DMUI_PointerButtons;

#define DMUI_POINTER_BUTTON_LEFT 0x00000001u
#define DMUI_POINTER_BUTTON_RIGHT 0x00000002u
#define DMUI_POINTER_BUTTON_MIDDLE 0x00000004u
#define DMUI_POINTER_BUTTON_X1 0x00000008u
#define DMUI_POINTER_BUTTON_X2 0x00000010u

typedef uint32_t DMUI_DialogKind;

#define DMUI_DIALOG_KIND_CONFIRM 0u
#define DMUI_DIALOG_KIND_TEXT_ENTRY 1u

typedef uint32_t DMUI_DialogEventKind;

#define DMUI_DIALOG_EVENT_PENDING 0u
#define DMUI_DIALOG_EVENT_SUBMITTED 1u
#define DMUI_DIALOG_EVENT_CANCELLED 2u
#define DMUI_DIALOG_EVENT_COMPLETED 3u

typedef uint32_t DMUI_ExternalTargetKind;

#define DMUI_EXTERNAL_TARGET_NONE 0u
#define DMUI_EXTERNAL_TARGET_URI 1u
#define DMUI_EXTERNAL_TARGET_FILE 2u
#define DMUI_EXTERNAL_TARGET_DIRECTORY 3u
#define DMUI_EXTERNAL_TARGET_VIRTUAL_FILE 4u
#define DMUI_EXTERNAL_TARGET_VIRTUAL_FILE_PARENT 5u

typedef uint32_t DMUI_LinkAction;

#define DMUI_LINK_ACTION_COPY_TARGET 0u
#define DMUI_LINK_ACTION_OPEN_EXTERNAL 1u

#if defined(_MSC_VER)
#pragma pack(push, 8)
#endif

typedef struct DMUI_HostReadyInfo
{
	uint32_t abiMajor;
} DMUI_HostReadyInfo;

typedef void (DMUI_CALL *DMUI_HostReadyCallback)(
	const DMUI_HostReadyInfo* info,
	void* userData);
typedef void (DMUI_CALL *DMUI_HostUnavailableCallback)(
	DMUI_UnavailableReason reason,
	void* userData);
typedef DMUI_Result (DMUI_CALL *DMUI_PageDrawCallback)(void* userData);
typedef DMUI_Result (DMUI_CALL *DMUI_ActionCallback)(void* userData);
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
	const char* id;
	const char* displayName;
	uint32_t version;
	DMUI_HostReadyCallback onHostReady;
	DMUI_HostUnavailableCallback onHostUnavailable;
	void* userData;
	DMUI_ClientCapabilities capabilities;
	const char* iconName;
	DMUI_ClientOrigin origin;
	const char* bridgeSourceLabel;
} DMUI_ClientDescriptor;

typedef struct DMUI_PageDescriptor
{
	const char* id;
	const char* displayName;
	// Null or empty leaves the page uncategorized. Non-empty IDs must have
	// already been registered for this client.
	const char* categoryId;
	const char* summary;
	int32_t sortKey;
	DMUI_PageKind kind;
	DMUI_PageDrawCallback draw;
	void* userData;
	// Unknown icon names use semantic fallback.
	const char* iconName;
} DMUI_PageDescriptor;

typedef struct DMUI_CategoryDescriptor
{
	const char* id;
	const char* displayName;
	int32_t sortKey;
	uint32_t reserved;
	// Unknown icon names use semantic fallback.
	const char* iconName;
} DMUI_CategoryDescriptor;

typedef struct DMUI_ActionDescriptor
{
	const char* id;
	const char* displayLabel;
	const char* iconName;
	const char* tooltip;
	int32_t sortKey;
	DMUI_ActionCallback callback;
	void* userData;
} DMUI_ActionDescriptor;

typedef struct DMUI_FrameObserverDescriptor
{
	DMUI_FrameCallback callback;
	void* userData;
} DMUI_FrameObserverDescriptor;

typedef struct DMUI_HotkeyActionDescriptor
{
	const char* id;
	const char* displayName;
	const char* suggestedDefaultChord;
	DMUI_HotkeyCallback callback;
	void* userData;
	DMUI_HotkeyContextPolicy contextPolicy;
	uint32_t reserved;
} DMUI_HotkeyActionDescriptor;

typedef struct DMUI_ExternalOpenDescriptor
{
	DMUI_ExternalTargetKind targetKind;
	const char* target;
	const char* application;
	const char* const* arguments;
	uint32_t argumentCount;
	uint32_t reserved;
	const char* workingDirectory;
} DMUI_ExternalOpenDescriptor;

typedef struct DMUI_LinkDescriptor
{
	const char* label;
	const char* note;
	uint32_t glyph;
	uint32_t enabled;
	DMUI_LinkAction action;
	uint32_t reserved;
	const DMUI_ExternalOpenDescriptor* external;
} DMUI_LinkDescriptor;

// expanded is caller-owned state the host reads and toggles; it is required.
typedef struct DMUI_FaqEntry
{
	const char* question;
	const char* answer;
	uint32_t* expanded;
} DMUI_FaqEntry;

typedef struct DMUI_DiagnosticDescriptor
{
	DMUI_Severity severity;
	const char* scope;
	const char* summary;
	const char* detail;
} DMUI_DiagnosticDescriptor;

typedef struct DMUI_PageActivityInfo
{
	DMUI_PageActivityKind kind;
	DMUI_PageHandle previousPage;
	DMUI_PageHandle activePage;
} DMUI_PageActivityInfo;

typedef struct DMUI_PageActivityObserverDescriptor
{
	DMUI_PageActivityCallback callback;
	void* userData;
} DMUI_PageActivityObserverDescriptor;

typedef struct DMUI_HotkeyBindingInfo
{
	DMUI_HotkeyBindingState state;
	char chord[DMUI_KEY_CHORD_CAPACITY];
} DMUI_HotkeyBindingInfo;

typedef struct DMUI_HostStateInfo
{
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

// Where a floating window opens: its pivot (0..1 of its size) sits at position,
// in screen space. A pivot of {0, 0} anchors the top-left.
typedef struct DMUI_Placement
{
	DMUI_Vec2 position;
	DMUI_Vec2 pivot;
} DMUI_Placement;

// The callback is valid only for the active draw call and is nonreentrant.
// It must not issue drawing calls. On success it returns writable storage of at
// least minimumCapacity bytes while preserving the prior bytes. On failure the
// previous allocation and the input data/capacity values remain valid and
// unchanged. The host retains no pointer after the draw call.
typedef DMUI_Result (DMUI_CALL *DMUI_ResizeTextBufferFn)(
	void* userData,
	size_t minimumCapacity,
	char** data,
	size_t* capacity) DMUI_NOEXCEPT;

// capacity includes the terminating NUL and must be in [1, INT_MAX]. data must
// be NUL-terminated within capacity. A null resize callback makes the buffer
// fixed-capacity, which bounds new input like maxBytes; otherwise the host may
// request same-frame growth.
typedef struct DMUI_TextBuffer
{
	char* data;
	size_t capacity;
	DMUI_ResizeTextBufferFn resize;
	void* userData;
} DMUI_TextBuffer;

// inputText output; UTF-8 byte offsets and caret are zero while inactive.
typedef struct DMUI_TextEditState
{
	uint32_t events;
	uint32_t active;
	size_t cursor;
	size_t selectionStart;
	size_t selectionEnd;
	DMUI_Vec2 caretPosition;
	float lineHeight;
} DMUI_TextEditState;

typedef struct DMUI_Vec4
{
	float x;
	float y;
	float z;
	float w;
} DMUI_Vec4;

// One styled byte range of the UTF-8 text a call carries; text, spans, and
// every other pointer are borrowed for the call. Spans are sorted by offset,
// never overlap, and begin and end on UTF-8 boundaries inside the text. Text
// outside every span draws with defaults. foreground and background are theme
// tones: INHERIT keeps the default text color and draws no background, and any
// other background tone fills translucently. CUSTOM_FOREGROUND and
// CUSTOM_BACKGROUND select foregroundColor and backgroundColor instead of the
// tone, and the color's alpha is kept. BOLD and ITALIC select the face;
// UNDERLINE and STRIKETHROUGH decorate the range. INTERACTIVE makes a text view
// range hoverable and clickable and reports userId with its events. HAS_ROLE
// selects role, a DMUI_FontRole. Unknown flag bits, tones, and roles are
// rejected. Each consumer accepts a subset: the text operation rejects
// INTERACTIVE; a text view and a text field reject HAS_ROLE, where BOLD and
// ITALIC pick the monospace role's faces and fall back to regular where a face
// would change advances; a text field also rejects INTERACTIVE.
typedef struct DMUI_TextSpan
{
	size_t offset;
	size_t length;
	DMUI_TextTone foreground;
	DMUI_TextTone background;
	DMUI_Vec4 foregroundColor;
	DMUI_Vec4 backgroundColor;
	DMUI_FontRole role;
	DMUI_TextSpanFlags flags;
	uint64_t userId;
} DMUI_TextSpan;

#define DMUI_TEXT_VIEW_NO_OFFSET SIZE_MAX

// All pointers are borrowed only for drawTextView. text is immutable UTF-8
// without embedded NUL bytes. lineOffsets contains every line start, beginning
// with zero; a final textLength offset represents a trailing blank line. Match
// offsets are sorted, may overlap, and all use the shared matchByteLength.
// matchByteLength may remain nonzero when matchCount is zero. Spans use byte
// offsets into text (see DMUI_TextSpan); they may cross lines and may be
// INTERACTIVE but not HAS_ROLE. firstLineNumber is the absolute index of line 0,
// so a caller that trims front lines raises it by the number removed and the
// reader keeps their place: a content change otherwise keeps the scroll
// position, and a caller resets it with the TOP scroll command. FOLLOW_TAIL
// keeps the view pinned to the end only while it is scrolled to the end.
// SELECTABLE lets the pointer select text and Ctrl+C or Ctrl+Insert copy it
// while the view has focus. Revisions must change whenever their corresponding
// borrowed content changes.
typedef struct DMUI_TextViewDescriptor
{
	const char* id;
	const char* text;
	size_t textLength;
	const size_t* lineOffsets;
	size_t lineCount;
	const size_t* matchByteOffsets;
	size_t matchCount;
	size_t matchByteLength;
	const DMUI_TextSpan* spans;
	size_t spanCount;
	uint64_t contentRevision;
	uint64_t matchRevision;
	uint64_t spanRevision;
	uint64_t firstLineNumber;
	DMUI_TextViewFlags flags;
	DMUI_Vec2 viewport;
} DMUI_TextViewDescriptor;

// activeMatch and revealByteOffset use DMUI_TEXT_VIEW_NO_OFFSET for no value.
// revealByteOffset and scrollCommand are one-shot requests and are reset by a
// successful draw. Callers must set the state revisions to the descriptor
// revisions before issuing a reveal directly; the C++ helpers do this
// automatically. Every successful draw rewrites the outputs: atBottom is nonzero
// while the view shows its end, events holds this frame's span events, and
// spanIndex and userId name the hovered or clicked span (spanIndex is
// DMUI_TEXT_VIEW_NO_OFFSET and userId zero without an event).
typedef struct DMUI_TextViewState
{
	uint64_t contentRevision;
	uint64_t matchRevision;
	size_t activeMatch;
	size_t revealByteOffset;
	size_t spanIndex;
	uint64_t userId;
	DMUI_TextViewScrollCommand scrollCommand;
	uint32_t atBottom;
	DMUI_TextViewEvents events;
} DMUI_TextViewState;

typedef struct DMUI_StyleMetrics
{
	DMUI_Vec2 itemSpacing;
	DMUI_Vec2 framePadding;
	DMUI_Vec2 itemInnerSpacing;
	DMUI_Vec2 cellPadding;
	DMUI_Vec2 windowPadding;
	float indentSpacing;
	float scrollbarSize;
	float fontSizeBase;
	float alpha;
	float frameRounding;
	float frameBorderSize;
	float sectionGap;
	DMUI_Vec2 panelPadding;
} DMUI_StyleMetrics;

typedef struct DMUI_FieldBeginOptions
{
	DMUI_FieldLayout layout;
} DMUI_FieldBeginOptions;

typedef struct DMUI_FieldEndOptions
{
	uint32_t resetVisible;
	uint32_t resetEnabled;
} DMUI_FieldEndOptions;

typedef struct DMUI_FieldFeedback
{
	DMUI_Severity severity;
	const char* message;
} DMUI_FieldFeedback;

// UTF-8 strings are borrowed for the call. The item ID is id, or label when id
// is null or empty; one of them is required. glyph is a Phosphor codepoint the
// theme icon set resolves, zero for none; an unresolvable glyph is omitted and
// a label-less button then shows nothing. A zero size axis fits its content, a
// negative one fills the remaining region. tooltip follows the item tooltip
// policy, including while the button is disabled.
typedef struct DMUI_ButtonDescriptor
{
	const char* id;
	const char* label;
	uint32_t glyph;
	uint32_t enabled;
	DMUI_ButtonStyle style;
	uint32_t reserved;
	const char* tooltip;
	DMUI_Vec2 size;
} DMUI_ButtonDescriptor;

// Called by inputText after this frame's edits and before the text is drawn,
// on the render thread and nonreentrantly, with the current text. It must not
// issue drawing calls. On success it returns spans over that text (see
// DMUI_TextSpan) that stay valid until inputText returns; INTERACTIVE and
// HAS_ROLE spans are rejected. Any other result, or invalid spans, makes
// inputText return that failure after drawing the text unstyled.
typedef DMUI_Result (DMUI_CALL *DMUI_TextStyleFn)(
	void* userData,
	const char* text,
	size_t length,
	const DMUI_TextSpan** spans,
	size_t* count) DMUI_NOEXCEPT;

// The one text input. hint is borrowed UTF-8, null or empty for none. SEARCH
// draws the host search frame with a magnifier; PLAIN a standard frame. flags
// are DMUI_UIInputTextFlags and editFlags DMUI_UITextEditFlags. maxBytes limits
// the text length, zero is unlimited; a limited field preallocates its buffer
// through the resize callback and never truncates text that is already longer.
// lines of zero with a zero size.y is a single line; otherwise the field is
// multiline, lines tall unless size.y is positive. A multiline field has no
// hint and rejects history and completion keys. size.x of zero uses the item
// width, a negative one fills the remaining region. cursor applies with RELOAD.
// A non-null styleSpans colors and decorates the text through styleUserData; it
// needs a single-line field without the Password flag, and draws the text in the
// monospace role so advances match the caret.
typedef struct DMUI_TextFieldOptions
{
	const char* hint;
	size_t cursor;
	DMUI_TextFieldStyle style;
	uint32_t flags;
	uint32_t editFlags;
	uint32_t maxBytes;
	uint32_t lines;
	uint32_t reserved;
	DMUI_Vec2 size;
	DMUI_TextStyleFn styleSpans;
	void* styleUserData;
} DMUI_TextFieldOptions;

// Strings are borrowed UTF-8. id scopes the section and is required. title is
// drawn verbatim, and a section without title and icon is just a rule. glyph is
// a Phosphor codepoint, zero to infer one from a non-empty title, or
// DMUI_GLYPH_NONE. BOX holds the content in a themed panel. A non-null expanded
// makes the section collapsible and is read and toggled by the host; the
// content is visible while it is nonzero. HAS_COUNT appends "(count)" to the
// title.
typedef struct DMUI_SectionDescriptor
{
	const char* id;
	const char* title;
	uint32_t* expanded;
	uint32_t glyph;
	DMUI_SectionFlags flags;
	uint32_t count;
	uint32_t reserved;
} DMUI_SectionDescriptor;

// description is the option's tooltip under the item tooltip policy. A disabled
// option shows but cannot be chosen.
typedef struct DMUI_ChoiceOption
{
	const char* label;
	const char* description;
	uint32_t enabled;
	uint32_t reserved;
} DMUI_ChoiceOption;

// The one dropdown; no label is drawn beside it. id is required. selected is an
// index into options, and any other value is unmatched: the preview shows
// unmatchedLabel (null or empty for none) and the caller's value is untouched.
// Without options the control is disabled. A zero width uses the item width.
typedef struct DMUI_ChoiceDescriptor
{
	const char* id;
	const DMUI_ChoiceOption* options;
	const char* unmatchedLabel;
	uint32_t optionCount;
	int32_t selected;
	float width;
	uint32_t reserved;
} DMUI_ChoiceDescriptor;

// selected is the chosen index on the frame changed is set, otherwise the input.
// completed marks a committed choice.
typedef struct DMUI_ChoiceResult
{
	int32_t selected;
	uint32_t changed;
	uint32_t completed;
} DMUI_ChoiceResult;

// The one checkbox. id is required. An optional label is drawn verbatim
// beside the box and toggles it too; in a field the bracket owns the label.
// A nonzero value is checked.
typedef struct DMUI_ToggleDescriptor
{
	const char* id;
	const char* label;
	uint32_t value;
	uint32_t reserved;
} DMUI_ToggleDescriptor;

// value is the new state on the frame changed is set, otherwise the input.
typedef struct DMUI_ToggleResult
{
	uint32_t value;
	uint32_t changed;
} DMUI_ToggleResult;

// The one spinner: an indeterminate busy indicator. id is required. size is the
// diameter in pixels; zero uses the frame height. It animates from the frame
// clock, so the caller redraws it each frame while work is pending.
typedef struct DMUI_SpinnerDescriptor
{
	const char* id;
	float size;
	uint32_t reserved;
} DMUI_SpinnerDescriptor;

// A numeric member is the union member that matches the type: i for signed
// integers, u for unsigned integers, f for float and double.
typedef union DMUI_NumericValue
{
	int64_t i;
	uint64_t u;
	double f;
} DMUI_NumericValue;

// The one numeric input. id is required and no label is drawn; put the label in
// a field bracket. value, minimum, maximum, step, and stepOrigin must be
// representable in type and finite. SLIDER needs both bounds; DRAG and INPUT take
// either, and a reversed pair is swapped. CLAMP keeps every result inside the
// present bounds; without it a typed or stepped value may leave them. HAS_STEP
// snaps every edit to stepOrigin + n * step, with a positive step, and reports
// no change when that restores the input. format is
// printf-style with exactly one conversion (f F e E g G for FLOAT and DOUBLE,
// d i u x X for integers; flags, width, precision, and length modifiers are
// accepted), may have literal text around it, and uses %% for a percent sign;
// null uses the type's default. unit is appended to the formatted text and is
// literal. At most 63 bytes of format and 31 of unit. A zero width uses the item
// width. dragSpeed applies to DRAG; zero uses 1 for integers and 0.01 for
// FLOAT and DOUBLE. INPUT draws no step buttons.
typedef struct DMUI_NumericDescriptor
{
	const char* id;
	const char* format;
	const char* unit;
	DMUI_NumericType type;
	DMUI_NumericPresentation presentation;
	DMUI_NumericFlags flags;
	uint32_t reserved;
	float width;
	float dragSpeed;
	DMUI_NumericValue value;
	DMUI_NumericValue minimum;
	DMUI_NumericValue maximum;
	DMUI_NumericValue step;
	DMUI_NumericValue stepOrigin;
} DMUI_NumericDescriptor;

// value is the edited value on the frame changed is set, otherwise the input.
// completed marks the frame an edit ends (the control loses focus after a change).
typedef struct DMUI_NumericResult
{
	DMUI_NumericValue value;
	uint32_t changed;
	uint32_t completed;
} DMUI_NumericResult;

// A swatch of a color edit. Strings are borrowed UTF-8; name identifies the
// swatch and description is its tooltip, null for none. rgba is packed
// 0xRRGGBBAA.
typedef struct DMUI_ColorPreset
{
	const char* name;
	const char* description;
	uint32_t rgba;
	uint32_t reserved;
} DMUI_ColorPreset;

// The one color edit: the inline RGB(A) fields with their picker, then the
// presets as a row of swatches under an optional label. id is required and no
// label is drawn beside the edit. rgba is packed 0xRRGGBBAA. Without ALPHA only
// RGB is edited and the alpha byte passes through unchanged. A zero width uses
// the available width. Choosing a swatch replaces the color, keeping the alpha
// byte without ALPHA. At most 64 presets.
typedef struct DMUI_ColorEditDescriptor
{
	const char* id;
	const DMUI_ColorPreset* presets;
	const char* presetsLabel;
	uint32_t presetCount;
	uint32_t rgba;
	DMUI_ColorEditFlags flags;
	uint32_t reserved;
	float width;
} DMUI_ColorEditDescriptor;

// rgba is the new color on the frame changed is set, otherwise the input.
// completed marks the frame an edit ends: a swatch choice, or the end of a
// drag or typed entry.
typedef struct DMUI_ColorEditResult
{
	uint32_t rgba;
	uint32_t changed;
	uint32_t completed;
} DMUI_ColorEditResult;

// The one key binding editor. The field shows the binding; pressing it captures
// the next chord of the slot, released together, and Escape or the controller B
// button cancels. id is required and scopes the control; no label is drawn. All
// strings are borrowed UTF-8. binding is a chord such as "Ctrl+F5" with the
// grammar of the hotkey action defaults; null, empty, or "none" is unbound, and
// an unrecognized one is shown as invalid. displayText, when not empty,
// replaces the formatted binding in the field, for a client whose bindings
// are not host chords; the field is styled as bound only for a chord binding.
// details leads the tooltip. warning explains a conflict or an invalid binding:
// it tints the field and, when unbound, the field reads "Check binding". ALLOW_CLEAR
// adds an unbind button while bound. READ_ONLY shows the binding disabled and
// captures nothing. A zero width uses the available width.
typedef struct DMUI_KeyCaptureDescriptor
{
	const char* id;
	const char* binding;
	const char* displayText;
	const char* details;
	const char* warning;
	DMUI_KeyCaptureSlot slot;
	DMUI_KeyCaptureFlags flags;
	float width;
	uint32_t reserved;
} DMUI_KeyCaptureDescriptor;

// changed is set on the frame a chord was captured or the binding was cleared;
// chord then holds the new binding, "none" when cleared, and is empty
// otherwise. The client stores it; hotkey registration is separate.
typedef struct DMUI_KeyCaptureResult
{
	uint32_t changed;
	char chord[DMUI_KEY_CHORD_CAPACITY];
} DMUI_KeyCaptureResult;

// WRAP breaks lines at the content region, or at wrapWidth when it is positive.
// BULLET draws a bullet and indents the item.
typedef struct DMUI_TextOptions
{
	DMUI_TextOptionFlags flags;
	float wrapWidth;
} DMUI_TextOptions;

// Resolved colors of the active theme for custom drawing.
typedef struct DMUI_ThemeTokens
{
	DMUI_Vec4 text;
	DMUI_Vec4 muted;
	DMUI_Vec4 accent;
	DMUI_Vec4 accentMuted;
	DMUI_Vec4 info;
	DMUI_Vec4 success;
	DMUI_Vec4 warning;
	DMUI_Vec4 error;
	DMUI_Vec4 disabled;
	DMUI_Vec4 restartNeeded;
	DMUI_Vec4 currentHotkey;
	DMUI_Vec4 rowAlt;
} DMUI_ThemeTokens;

// The source pointer must be a live ID3D11ShaderResourceView for the duration
// of this call. The host retains its own COM reference on success.
typedef struct DMUI_D3D11ImageDescriptor
{
	void* shaderResourceView;
	uint32_t contentWidth;
	uint32_t contentHeight;
} DMUI_D3D11ImageDescriptor;

// The host synchronously consumes every referenced pixel byte before returning.
// It performs no alpha premultiplication or color-space conversion. rowPitch must
// cover width * 4 bytes. accessibleByteCount must cover
// (height - 1) * rowPitch + width * 4; final-row padding need not be accessible.
typedef struct DMUI_ImageDescriptor
{
	uint32_t width;
	uint32_t height;
	DMUI_PixelFormat pixelFormat;
	uint32_t reserved;
	uint64_t rowPitch;
	uint64_t accessibleByteCount;
	const void* pixels;
} DMUI_ImageDescriptor;

typedef struct DMUI_ImageDrawOptions
{
	DMUI_Vec2 size;
	DMUI_Vec2 uv0;
	DMUI_Vec2 uv1;
	DMUI_Vec4 tint;
	uint32_t preserveAspect;
	uint32_t reserved;
} DMUI_ImageDrawOptions;

typedef struct DMUI_ImageInfo
{
	DMUI_ImageStatus status;
	DMUI_Result failure;
	uint32_t contentWidth;
	uint32_t contentHeight;
	uint64_t deviceGeneration;
} DMUI_ImageInfo;

typedef struct DMUI_ManagedOverlayOptions
{
	DMUI_OverlayAnchor anchor;
	DMUI_Vec2 offset;
	// Screen pixels, matching placement.size; changed values apply once, zero uses minimum size or auto.
	DMUI_Vec2 size;
	DMUI_Vec2 minimumSize;
	DMUI_Vec2 maximumSize;
	float opacity;
	float contentScale;
	uint32_t backgroundVisible;
	uint32_t borderVisible;
	uint32_t allowArrangement;
} DMUI_ManagedOverlayOptions;

typedef struct DMUI_ManagedOverlayPlacement
{
	DMUI_OverlayAnchor anchor;
	DMUI_Vec2 offset;
	DMUI_Vec2 position;
	DMUI_Vec2 size;
	uint64_t changeGeneration;
	uint32_t arrangementCompleted;
	uint32_t visible;
} DMUI_ManagedOverlayPlacement;

typedef struct DMUI_OverlayFocusInfo
{
	uint32_t focused;
	DMUI_OverlayFocusEndReason endReason;
	uint64_t generation;
} DMUI_OverlayFocusInfo;

// position and viewport are set on any drawing page. The remaining fields are
// zero unless the page owns the pointer; see the spec's Pointer state.
typedef struct DMUI_PointerState
{
	uint32_t active;
	uint32_t captured;
	// Screen space.
	DMUI_Vec2 position;
	DMUI_Vec2 viewport;
	DMUI_Vec2 wheel;
	uint32_t down;
	uint32_t pressed;
	uint32_t released;
} DMUI_PointerState;

typedef struct DMUI_NotificationDescriptor
{
	DMUI_Severity severity;
	// Copied UTF-8: required message (1024 bytes), optional title (256 bytes).
	const char* message;
	// Zero selects the host default; lifetime starts at first presentation.
	uint32_t durationMilliseconds;
	const char* title;
} DMUI_NotificationDescriptor;

typedef struct DMUI_PlotReferenceLine
{
	float value;
	DMUI_Vec4 color;
} DMUI_PlotReferenceLine;

typedef struct DMUI_AnnotatedPlotDescriptor
{
	const float* samples;
	uint32_t sampleCount;
	uint32_t sampleOffset;
	float scaleMinimum;
	float scaleMaximum;
	DMUI_Vec2 size;
	const char* overlayText;
	const DMUI_PlotReferenceLine* referenceLines;
	uint32_t referenceLineCount;
} DMUI_AnnotatedPlotDescriptor;

typedef struct DMUI_DialogDescriptor
{
	DMUI_DialogKind kind;
	const char* title;
	const char* body;
	const char* acceptLabel;
	const char* cancelLabel;
	const char* hint;
	const char* initialText;
	uint32_t maximumTextBytes;
} DMUI_DialogDescriptor;

typedef struct DMUI_DialogEvent
{
	DMUI_DialogEventKind kind;
	uint64_t submissionId;
	uint32_t requiredTextCapacity;
} DMUI_DialogEvent;

typedef struct DMUI_IconResolutionRequest
{
	// Null or empty fields are equivalent. explicitName is limited to 128 bytes;
	// metadata fields are limited to 256 bytes.
	const char* explicitName;
	const char* primaryMetadata;
	const char* secondaryMetadata;
} DMUI_IconResolutionRequest;

typedef DMUI_Result (DMUI_CALL *DMUI_RegisterClientFn)(
	const DMUI_ClientDescriptor* descriptor,
	DMUI_ClientHandle* client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_RegisterCategoryFn)(
	DMUI_ClientHandle client,
	const DMUI_CategoryDescriptor* descriptor) DMUI_NOEXCEPT;
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
// Like unregisterHotkeyAction: render-thread only, or DMUI_RESULT_WRONG_THREAD.
// Dispatch also runs on the render thread, so no callback starts after this returns successfully.
// An action may unregister itself from its callback; keep userData valid until that callback returns.
// Otherwise clients own userData and must keep it valid until unregister returns.
// A foreign, stale, or already removed handle returns DMUI_RESULT_ACTION_NOT_FOUND.
typedef DMUI_Result (DMUI_CALL *DMUI_UnregisterActionFn)(
	DMUI_ClientHandle client,
	DMUI_ActionHandle action) DMUI_NOEXCEPT;
// Enabled state is thread-safe. A disabled action stays on its toolbar but cannot be pressed,
// and the command palette omits it. Actions start enabled.
typedef DMUI_Result (DMUI_CALL *DMUI_SetActionEnabledFn)(
	DMUI_ClientHandle client,
	DMUI_ActionHandle action,
	uint32_t enabled) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_SetStatusFn)(
	DMUI_ClientHandle client,
	DMUI_Severity severity,
	const char* message) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_PushFontFn)(
	DMUI_ClientHandle client,
	DMUI_FontRole role) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_PopFontFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
// Render-thread-only, valid only inside the owning page draw callback. Invalid
// line or match offsets and short structures are errors. Stale presentation
// revisions reset the state to no active match or reveal request. The host
// retains no borrowed pointer after the call.
typedef DMUI_Result (DMUI_CALL *DMUI_DrawTextViewFn)(
	DMUI_ClientHandle client,
	const DMUI_TextViewDescriptor* descriptor,
	DMUI_TextViewState* state) DMUI_NOEXCEPT;
// Link rows are render-thread-only and valid only inside page draw callbacks.
// Buttons share the available width. COPY_TARGET copies external.target;
// OPEN_EXTERNAL dispatches the same descriptor through openExternal.
// Tooltips use a non-empty note when present, otherwise the target.
// A zero count succeeds without drawing.
// Null required values, empty labels or enabled URLs, and short descriptors are invalid.
typedef DMUI_Result (DMUI_CALL *DMUI_DrawLinkRowFn)(
	DMUI_ClientHandle client,
	const char* id,
	const DMUI_LinkDescriptor* links,
	size_t count) DMUI_NOEXCEPT;
// FAQ rows are render-thread-only and valid only inside page draw callbacks.
// The host owns presentation; each entry's caller-owned expanded flag holds
// its state.
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
typedef DMUI_Result (DMUI_CALL *DMUI_EndSettingsTableFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
// Page activity observers are process-lifetime registrations invoked during shell drawing on the render thread.
typedef DMUI_Result (DMUI_CALL *DMUI_RegisterPageActivityObserverFn)(
	DMUI_ClientHandle client,
	const DMUI_PageActivityObserverDescriptor* descriptor,
	DMUI_PageActivityObserverHandle* observer) DMUI_NOEXCEPT;
// Enabled state is thread-safe. Disabling a held action preserves its owned key-up.
typedef DMUI_Result (DMUI_CALL *DMUI_SetHotkeyActionEnabledFn)(
	DMUI_ClientHandle client,
	DMUI_HotkeyActionHandle action,
	uint32_t enabled) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_ImportD3D11ImageFn)(
	DMUI_ClientHandle client,
	const DMUI_D3D11ImageDescriptor* descriptor,
	DMUI_ImageHandle* image) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_ReleaseImageFn)(
	DMUI_ClientHandle client,
	DMUI_ImageHandle image) DMUI_NOEXCEPT;
// Released/invalidated status remains queryable only until the slot is reused.
// Reuse advances the opaque handle generation; older handles then return STALE_HANDLE.
typedef DMUI_Result (DMUI_CALL *DMUI_QueryImageFn)(
	DMUI_ClientHandle client,
	DMUI_ImageHandle image,
	DMUI_ImageInfo* info) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_ConfigureOverlayFn)(
	DMUI_ClientHandle client,
	DMUI_PageHandle page,
	const DMUI_ManagedOverlayOptions* options) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_QueryOverlayFn)(
	DMUI_ClientHandle client,
	DMUI_PageHandle page,
	DMUI_ManagedOverlayPlacement* placement) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_ResetOverlayFn)(
	DMUI_ClientHandle client,
	DMUI_PageHandle page) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_RequestOverlayFocusFn)(
	DMUI_ClientHandle client,
	DMUI_PageHandle page) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_ReleaseOverlayFocusFn)(
	DMUI_ClientHandle client,
	DMUI_PageHandle page) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_QueryOverlayFocusFn)(
	DMUI_ClientHandle client,
	DMUI_PageHandle page,
	DMUI_OverlayFocusInfo* info) DMUI_NOEXCEPT;
// Reads Interface\Translations\<owner>_<language>.txt, then <owner>_en.txt.
// requiredCapacity counts the terminator.
typedef DMUI_Result (DMUI_CALL *DMUI_ResolveTextFn)(
	const char* owner,
	const char* key,
	char* buffer,
	uint32_t capacity,
	uint32_t* requiredCapacity) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_PostNotificationFn)(
	DMUI_ClientHandle client,
	const DMUI_NotificationDescriptor* descriptor) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_RequestDialogFn)(
	DMUI_ClientHandle client,
	const DMUI_DialogDescriptor* descriptor,
	DMUI_DialogHandle* dialog) DMUI_NOEXCEPT;
// BUFFER_TOO_SMALL leaves the event pending and reports requiredTextCapacity.
typedef DMUI_Result (DMUI_CALL *DMUI_PollDialogEventFn)(
	DMUI_ClientHandle client,
	DMUI_DialogHandle dialog,
	DMUI_DialogEvent* event,
	char* textBuffer,
	uint32_t textCapacity) DMUI_NOEXCEPT;
// accepted != 0 closes the dialog. Rejection optionally copies error and keeps text open.
typedef DMUI_Result (DMUI_CALL *DMUI_ResolveDialogSubmissionFn)(
	DMUI_ClientHandle client,
	DMUI_DialogHandle dialog,
	uint64_t submissionId,
	uint32_t accepted,
	const char* error) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_CancelDialogFn)(
	DMUI_ClientHandle client,
	DMUI_DialogHandle dialog) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_CreateImageFn)(
	DMUI_ClientHandle client,
	const DMUI_ImageDescriptor* descriptor,
	DMUI_ImageHandle* image) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UpdateImageFn)(
	DMUI_ClientHandle client,
	DMUI_ImageHandle image,
	const DMUI_ImageDescriptor* descriptor) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_LoadImageFileFn)(
	DMUI_ClientHandle client,
	const char* utf8Path,
	DMUI_ImageHandle* image) DMUI_NOEXCEPT;
// Same load as loadImageFile from encoded bytes (up to 64 MiB). The host copies
// the bytes before returning, so the buffer need not outlive the call.
typedef DMUI_Result (DMUI_CALL *DMUI_LoadImageMemoryFn)(
	DMUI_ClientHandle client,
	const void* bytes,
	size_t size,
	DMUI_ImageHandle* image) DMUI_NOEXCEPT;
// With no application, the OS-associated handler opens target. Arguments and
// workingDirectory are then invalid. An explicit application must be an absolute
// executable path. Its argv is application, supplied arguments, then target when
// targetKind is not NONE. No shell command interpreter or placeholder expansion
// is performed. Success means process creation or OS dispatch was accepted.
typedef DMUI_Result (DMUI_CALL *DMUI_OpenExternalFn)(
	DMUI_ClientHandle client,
	const DMUI_ExternalOpenDescriptor* descriptor,
	uint32_t* nativeError) DMUI_NOEXCEPT;
typedef struct DMUI_UIAPI DMUI_UIAPI;
// Pure, thread-safe query over immutable host icon data. A successful zero glyph
// means no match; errors also leave glyph zero.
typedef DMUI_Result (DMUI_CALL *DMUI_ResolveIconGlyphFn)(
	const DMUI_IconResolutionRequest* request,
	uint32_t* glyph) DMUI_NOEXCEPT;
// Field brackets are render-thread-only and valid in drawing page callbacks.
// Inside a settings table, the field becomes its next row. Outside a settings
// table, it owns equivalent standalone geometry. A visible begin must be
// balanced by endField. Labels are required for LABEL_VALUE and optional for
// FULL_SPAN.
typedef DMUI_Result (DMUI_CALL *DMUI_BeginFieldFn)(
	DMUI_ClientHandle client,
	const char* id,
	const char* label,
	const char* description,
	const DMUI_FieldBeginOptions* options,
	uint32_t* visible) DMUI_NOEXCEPT;
// Feedback is copied by the host and retained only until the current field ends.
// A null or empty message clears feedback previously supplied for that field.
typedef DMUI_Result (DMUI_CALL *DMUI_SetFieldFeedbackFn)(
	DMUI_ClientHandle client,
	const DMUI_FieldFeedback* feedback) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_EndFieldFn)(
	DMUI_ClientHandle client,
	const DMUI_FieldEndOptions* options,
	uint32_t* resetPressed) DMUI_NOEXCEPT;

// Slots are appended in minor versions; the table never shrinks within a major.
typedef struct DMUI_HostAPI
{
	uint32_t abiMajor;
	const DMUI_UIAPI* ui;
	DMUI_RegisterClientFn registerClient;
	DMUI_RegisterPageFn registerPage;
	DMUI_QueryStateFn queryState;
	DMUI_RequestFrameFn requestFrame;
	DMUI_ReleaseFrameFn releaseFrame;
	DMUI_IsMenuVisibleFn isMenuVisible;
	DMUI_SelectPageFn selectPage;
	DMUI_AttachSwapChainFn attachSwapChain;
	DMUI_RegisterActionFn registerAction;
	DMUI_UnregisterActionFn unregisterAction;
	DMUI_SetActionEnabledFn setActionEnabled;
	DMUI_SetStatusFn setStatus;
	DMUI_PushFontFn pushFont;
	DMUI_PopFontFn popFont;
	DMUI_RegisterFrameObserverFn registerFrameObserver;
	DMUI_QueryVideoMemoryFn queryVideoMemory;
	DMUI_RegisterHotkeyActionFn registerHotkeyAction;
	DMUI_QueryHotkeyBindingFn queryHotkeyBinding;
	DMUI_UnregisterHotkeyActionFn unregisterHotkeyAction;
	DMUI_BeginSettingsTableFn beginSettingsTable;
	DMUI_EndSettingsTableFn endSettingsTable;
	DMUI_RegisterPageActivityObserverFn registerPageActivityObserver;
	DMUI_DrawLinkRowFn drawLinkRow;
	DMUI_DrawFaqFn drawFaq;
	DMUI_ReportDiagnosticFn reportDiagnostic;
	DMUI_SetHotkeyActionEnabledFn setHotkeyActionEnabled;
	DMUI_ImportD3D11ImageFn importD3D11Image;
	DMUI_ReleaseImageFn releaseImage;
	DMUI_QueryImageFn queryImage;
	DMUI_ConfigureOverlayFn configureOverlay;
	DMUI_QueryOverlayFn queryOverlay;
	DMUI_ResetOverlayFn resetOverlay;
	DMUI_PostNotificationFn postNotification;
	DMUI_RequestDialogFn requestDialog;
	DMUI_PollDialogEventFn pollDialogEvent;
	DMUI_ResolveDialogSubmissionFn resolveDialogSubmission;
	DMUI_CancelDialogFn cancelDialog;
	DMUI_CreateImageFn createImage;
	DMUI_UpdateImageFn updateImage;
	DMUI_RegisterCategoryFn registerCategory;
	DMUI_OpenExternalFn openExternal;
	DMUI_ResolveIconGlyphFn resolveIconGlyph;
	DMUI_BeginFieldFn beginField;
	DMUI_SetFieldFeedbackFn setFieldFeedback;
	DMUI_EndFieldFn endField;
	DMUI_DrawTextViewFn drawTextView;
	DMUI_LoadImageFileFn loadImageFile;
	DMUI_LoadImageMemoryFn loadImageMemory;
	DMUI_RequestOverlayFocusFn requestOverlayFocus;
	DMUI_ReleaseOverlayFocusFn releaseOverlayFocus;
	DMUI_QueryOverlayFocusFn queryOverlayFocus;
	DMUI_ResolveTextFn resolveText;
} DMUI_HostAPI;

#if defined(_MSC_VER)
#pragma pack(pop)
#endif

// Request the header minor, then lower same-major minors until a table is returned.
// Read or call only slots introduced at or before the negotiated minor.
DMUI_EXPORT const DMUI_HostAPI* DMUI_CALL DMUI_GetAPI(
	uint32_t requestedAbiVersion) DMUI_NOEXCEPT;
