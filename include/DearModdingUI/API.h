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

#define DMUI_ABI_VERSION 2u

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
#define DMUI_RESULT_SERVICE_UNAVAILABLE 28u
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
	case DMUI_RESULT_SERVICE_UNAVAILABLE:
		return "SERVICE_UNAVAILABLE";
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
#define DMUI_FONT_ROLE_MONOSPACE 5u
#define DMUI_FONT_ROLE_COUNT 6u

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

typedef uint32_t DMUI_SettingsRowLayout;

#define DMUI_SETTINGS_ROW_LAYOUT_LABEL_VALUE 0u
#define DMUI_SETTINGS_ROW_LAYOUT_FULL_SPAN 1u

typedef uint32_t DMUI_FieldLayout;

#define DMUI_FIELD_LAYOUT_LABEL_VALUE 0u
#define DMUI_FIELD_LAYOUT_FULL_SPAN 1u

typedef uint32_t DMUI_FieldFeedbackSeverity;

#define DMUI_FIELD_FEEDBACK_SEVERITY_INFO 0u
#define DMUI_FIELD_FEEDBACK_SEVERITY_WARNING 1u
#define DMUI_FIELD_FEEDBACK_SEVERITY_ERROR 2u
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
	uint32_t abiVersion;
} DMUI_HostReadyInfo;

typedef void (DMUI_CALL *DMUI_HostReadyCallback)(
	const DMUI_HostReadyInfo* info,
	void* userData);
typedef void (DMUI_CALL *DMUI_HostUnavailableCallback)(
	DMUI_UnavailableReason reason,
	void* userData);
typedef DMUI_Result (DMUI_CALL *DMUI_PageDrawCallback)(void* userData);
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

typedef struct DMUI_FaqEntry
{
	const char* question;
	const char* answer;
} DMUI_FaqEntry;

typedef struct DMUI_DiagnosticDescriptor
{
	DMUI_StatusSeverity severity;
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
	char chord[32];
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
// fixed-capacity; otherwise the host may request same-frame growth.
typedef struct DMUI_TextBuffer
{
	char* data;
	size_t capacity;
	DMUI_ResizeTextBufferFn resize;
	void* userData;
} DMUI_TextBuffer;

#define DMUI_TEXT_VIEW_NO_OFFSET SIZE_MAX

// All pointers are borrowed only for drawTextView. text is immutable UTF-8
// without embedded NUL bytes. lineOffsets contains every line start, beginning
// with zero; a final textLength offset represents a trailing blank line. Match
// offsets are sorted, may overlap, and all use the shared matchByteLength.
// matchByteLength may remain nonzero when matchCount is zero.
// Revisions must change whenever their corresponding borrowed content changes.
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
	uint64_t contentRevision;
	uint64_t matchRevision;
	DMUI_Vec2 viewport;
} DMUI_TextViewDescriptor;

// activeMatch and revealByteOffset use DMUI_TEXT_VIEW_NO_OFFSET for no value.
// revealByteOffset is a one-shot request and is reset by a successful draw.
// Callers must set the state revisions to the descriptor revisions before
// issuing a reveal directly; the C++ helpers do this automatically.
typedef struct DMUI_TextViewState
{
	uint64_t contentRevision;
	uint64_t matchRevision;
	size_t activeMatch;
	size_t revealByteOffset;
} DMUI_TextViewState;

typedef struct DMUI_Vec4
{
	float x;
	float y;
	float z;
	float w;
} DMUI_Vec4;

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
} DMUI_StyleMetrics;

typedef struct DMUI_SettingsRowOptions
{
	uint32_t resetVisible;
	uint32_t resetEnabled;
} DMUI_SettingsRowOptions;

typedef struct DMUI_SettingsRowBeginOptions
{
	DMUI_SettingsRowLayout layout;
} DMUI_SettingsRowBeginOptions;

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
	DMUI_FieldFeedbackSeverity severity;
	const char* message;
} DMUI_FieldFeedback;

typedef struct DMUI_ThemeColors
{
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
	DMUI_Vec2 minimumSize;
	DMUI_Vec2 maximumSize;
	float opacity;
	float contentScale;
	uint32_t backgroundVisible;
	uint32_t borderVisible;
	uint32_t allowArrangement;
	uint32_t reserved;
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

typedef struct DMUI_NotificationDescriptor
{
	DMUI_StatusSeverity severity;
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
// buffer must be NUL-terminated within capacity bytes, capacity must be in
// [1, INT_MAX], and the logical input value cannot contain embedded NUL bytes.
// Editing never truncates the existing value. New input is limited to the
// remaining capacity and may be inserted partially, clipped at a complete UTF-8
// boundary. On success buffer remains NUL-terminated and changed reports whether
// the accepted value differs from the input value.
typedef DMUI_Result (DMUI_CALL *DMUI_DrawSearchInputFn)(
	DMUI_ClientHandle client,
	const char* id,
	const char* hint,
	char* buffer,
	size_t capacity,
	uint32_t* changed) DMUI_NOEXCEPT;
// The text buffer is borrowed only for this render-thread draw call. With a
// resize callback, insertion may grow the buffer and complete in the same
// frame. Without one, the fixed-capacity insertion semantics above apply.
typedef DMUI_Result (DMUI_CALL *DMUI_DrawSearchInputBufferFn)(
	DMUI_ClientHandle client,
	const char* id,
	const char* hint,
	DMUI_TextBuffer* buffer,
	uint32_t* changed) DMUI_NOEXCEPT;
// Render-thread-only, valid only inside the owning page draw callback. Invalid
// line or match offsets and short structures are errors. Stale presentation
// revisions reset the state to no active match or reveal request. The host
// retains no borrowed pointer after the call.
typedef DMUI_Result (DMUI_CALL *DMUI_DrawTextViewFn)(
	DMUI_ClientHandle client,
	const DMUI_TextViewDescriptor* descriptor,
	DMUI_TextViewState* state) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_DrawCollapsingSectionHeaderFn)(
	DMUI_ClientHandle client,
	const char* key,
	const char* text,
	uint32_t glyph,
	uint32_t* expanded,
	size_t count) DMUI_NOEXCEPT;
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

typedef struct DMUI_HostAPI
{
	uint32_t abiVersion;
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
	DMUI_SetHotkeyActionEnabledFn setHotkeyActionEnabled;
	DMUI_ImportD3D11ImageFn importD3D11Image;
	DMUI_ReleaseImageFn releaseImage;
	DMUI_QueryImageFn queryImage;
	DMUI_ConfigureOverlayFn configureOverlay;
	DMUI_QueryOverlayFn queryOverlay;
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
	DMUI_DrawSearchInputBufferFn drawSearchInputBuffer;
} DMUI_HostAPI;

#if defined(_MSC_VER)
#pragma pack(pop)
#endif

DMUI_EXPORT const DMUI_HostAPI* DMUI_CALL DMUI_GetAPI(
	uint32_t requestedHostAbi) DMUI_NOEXCEPT;
