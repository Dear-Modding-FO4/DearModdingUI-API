#pragma once

// Generated from schema/ui-contract.json; do not edit.

#include <DearModdingUI/API.h>

#define DMUI_UI_ABI_1 1u
#define DMUI_UI_ABI_CURRENT DMUI_UI_ABI_1
#define DMUI_UI_REVISION_1 1u
#define DMUI_UI_REVISION_CURRENT DMUI_UI_REVISION_1

typedef uint32_t DMUI_UIColor;
#define DMUI_UI_COLOR_NONE 0u
#define DMUI_UI_COLOR_TEXT 1000u
#define DMUI_UI_COLOR_TEXT_DISABLED 1001u
#define DMUI_UI_COLOR_WINDOW_BG 1002u
#define DMUI_UI_COLOR_CHILD_BG 1003u
#define DMUI_UI_COLOR_POPUP_BG 1004u
#define DMUI_UI_COLOR_BORDER 1005u
#define DMUI_UI_COLOR_BORDER_SHADOW 1006u
#define DMUI_UI_COLOR_FRAME_BG 1007u
#define DMUI_UI_COLOR_FRAME_BG_HOVERED 1008u
#define DMUI_UI_COLOR_FRAME_BG_ACTIVE 1009u
#define DMUI_UI_COLOR_TITLE_BG 1010u
#define DMUI_UI_COLOR_TITLE_BG_ACTIVE 1011u
#define DMUI_UI_COLOR_TITLE_BG_COLLAPSED 1012u
#define DMUI_UI_COLOR_MENU_BAR_BG 1013u
#define DMUI_UI_COLOR_SCROLLBAR_BG 1014u
#define DMUI_UI_COLOR_SCROLLBAR_GRAB 1015u
#define DMUI_UI_COLOR_SCROLLBAR_GRAB_HOVERED 1016u
#define DMUI_UI_COLOR_SCROLLBAR_GRAB_ACTIVE 1017u
#define DMUI_UI_COLOR_CHECK_MARK 1018u
#define DMUI_UI_COLOR_CHECKBOX_SELECTED_BG 1019u
#define DMUI_UI_COLOR_SLIDER_GRAB 1020u
#define DMUI_UI_COLOR_SLIDER_GRAB_ACTIVE 1021u
#define DMUI_UI_COLOR_BUTTON 1022u
#define DMUI_UI_COLOR_BUTTON_HOVERED 1023u
#define DMUI_UI_COLOR_BUTTON_ACTIVE 1024u
#define DMUI_UI_COLOR_HEADER 1025u
#define DMUI_UI_COLOR_HEADER_HOVERED 1026u
#define DMUI_UI_COLOR_HEADER_ACTIVE 1027u
#define DMUI_UI_COLOR_SEPARATOR 1028u
#define DMUI_UI_COLOR_SEPARATOR_HOVERED 1029u
#define DMUI_UI_COLOR_SEPARATOR_ACTIVE 1030u
#define DMUI_UI_COLOR_RESIZE_GRIP 1031u
#define DMUI_UI_COLOR_RESIZE_GRIP_HOVERED 1032u
#define DMUI_UI_COLOR_RESIZE_GRIP_ACTIVE 1033u
#define DMUI_UI_COLOR_INPUT_TEXT_CURSOR 1034u
#define DMUI_UI_COLOR_TAB_HOVERED 1035u
#define DMUI_UI_COLOR_TAB 1036u
#define DMUI_UI_COLOR_TAB_SELECTED 1037u
#define DMUI_UI_COLOR_TAB_SELECTED_OVERLINE 1038u
#define DMUI_UI_COLOR_TAB_DIMMED 1039u
#define DMUI_UI_COLOR_TAB_DIMMED_SELECTED 1040u
#define DMUI_UI_COLOR_TAB_DIMMED_SELECTED_OVERLINE 1041u
#define DMUI_UI_COLOR_DOCKING_PREVIEW 1042u
#define DMUI_UI_COLOR_DOCKING_EMPTY_BG 1043u
#define DMUI_UI_COLOR_PLOT_LINES 1044u
#define DMUI_UI_COLOR_PLOT_LINES_HOVERED 1045u
#define DMUI_UI_COLOR_PLOT_HISTOGRAM 1046u
#define DMUI_UI_COLOR_PLOT_HISTOGRAM_HOVERED 1047u
#define DMUI_UI_COLOR_TABLE_HEADER_BG 1048u
#define DMUI_UI_COLOR_TABLE_BORDER_STRONG 1049u
#define DMUI_UI_COLOR_TABLE_BORDER_LIGHT 1050u
#define DMUI_UI_COLOR_TABLE_ROW_BG 1051u
#define DMUI_UI_COLOR_TABLE_ROW_BG_ALT 1052u
#define DMUI_UI_COLOR_TEXT_LINK 1053u
#define DMUI_UI_COLOR_TEXT_SELECTED_BG 1054u
#define DMUI_UI_COLOR_TREE_LINES 1055u
#define DMUI_UI_COLOR_DRAG_DROP_TARGET 1056u
#define DMUI_UI_COLOR_DRAG_DROP_TARGET_BG 1057u
#define DMUI_UI_COLOR_UNSAVED_MARKER 1058u
#define DMUI_UI_COLOR_NAV_CURSOR 1059u
#define DMUI_UI_COLOR_NAV_WINDOWING_HIGHLIGHT 1060u
#define DMUI_UI_COLOR_NAV_WINDOWING_DIM_BG 1061u
#define DMUI_UI_COLOR_MODAL_WINDOW_DIM_BG 1062u

typedef uint32_t DMUI_UIDataType;
#define DMUI_UI_DATA_TYPE_NONE 0u
#define DMUI_UI_DATA_TYPE_S8 101u
#define DMUI_UI_DATA_TYPE_U8 102u
#define DMUI_UI_DATA_TYPE_S16 103u
#define DMUI_UI_DATA_TYPE_U16 104u
#define DMUI_UI_DATA_TYPE_S32 105u
#define DMUI_UI_DATA_TYPE_U32 106u
#define DMUI_UI_DATA_TYPE_S64 107u
#define DMUI_UI_DATA_TYPE_U64 108u
#define DMUI_UI_DATA_TYPE_FLOAT 109u
#define DMUI_UI_DATA_TYPE_DOUBLE 110u

typedef uint32_t DMUI_UIComboFlags;
#define DMUI_UI_COMBO_FLAGS_NONE 0u
#define DMUI_UI_COMBO_FLAGS_POPUP_ALIGN_LEFT 1u
#define DMUI_UI_COMBO_FLAGS_HEIGHT_SMALL 2u
#define DMUI_UI_COMBO_FLAGS_HEIGHT_REGULAR 4u
#define DMUI_UI_COMBO_FLAGS_HEIGHT_LARGE 8u
#define DMUI_UI_COMBO_FLAGS_HEIGHT_LARGEST 16u
#define DMUI_UI_COMBO_FLAGS_NO_ARROW_BUTTON 32u
#define DMUI_UI_COMBO_FLAGS_NO_PREVIEW 64u
#define DMUI_UI_COMBO_FLAGS_WIDTH_FIT_PREVIEW 128u

typedef uint32_t DMUI_UIHoveredFlags;
#define DMUI_UI_HOVERED_FLAGS_NONE 0u
#define DMUI_UI_HOVERED_FLAGS_CHILD_WINDOWS 1u
#define DMUI_UI_HOVERED_FLAGS_ROOT_WINDOW 2u
#define DMUI_UI_HOVERED_FLAGS_ANY_WINDOW 4u
#define DMUI_UI_HOVERED_FLAGS_NO_POPUP_HIERARCHY 8u
#define DMUI_UI_HOVERED_FLAGS_DOCK_HIERARCHY 16u
#define DMUI_UI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_POPUP 32u
#define DMUI_UI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_ACTIVE_ITEM 64u
#define DMUI_UI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_ITEM 128u
#define DMUI_UI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_WINDOW 256u
#define DMUI_UI_HOVERED_FLAGS_ALLOW_WHEN_DISABLED 512u
#define DMUI_UI_HOVERED_FLAGS_NO_NAV_OVERRIDE 1024u
#define DMUI_UI_HOVERED_FLAGS_FOR_TOOLTIP 2048u
#define DMUI_UI_HOVERED_FLAGS_STATIONARY 4096u
#define DMUI_UI_HOVERED_FLAGS_DELAY_NONE 8192u
#define DMUI_UI_HOVERED_FLAGS_DELAY_SHORT 16384u
#define DMUI_UI_HOVERED_FLAGS_DELAY_NORMAL 32768u
#define DMUI_UI_HOVERED_FLAGS_NO_SHARED_DELAY 65536u

typedef uint32_t DMUI_UIInputTextFlags;
#define DMUI_UI_INPUT_TEXT_FLAGS_NONE 0u
#define DMUI_UI_INPUT_TEXT_FLAGS_CHARS_DECIMAL 1u
#define DMUI_UI_INPUT_TEXT_FLAGS_CHARS_HEXADECIMAL 2u
#define DMUI_UI_INPUT_TEXT_FLAGS_CHARS_SCIENTIFIC 4u
#define DMUI_UI_INPUT_TEXT_FLAGS_CHARS_UPPERCASE 8u
#define DMUI_UI_INPUT_TEXT_FLAGS_CHARS_NO_BLANK 16u
#define DMUI_UI_INPUT_TEXT_FLAGS_ALLOW_TAB_INPUT 32u
#define DMUI_UI_INPUT_TEXT_FLAGS_ENTER_RETURNS_TRUE 64u
#define DMUI_UI_INPUT_TEXT_FLAGS_ESCAPE_CLEARS_ALL 128u
#define DMUI_UI_INPUT_TEXT_FLAGS_CTRL_ENTER_FOR_NEW_LINE 256u
#define DMUI_UI_INPUT_TEXT_FLAGS_READ_ONLY 512u
#define DMUI_UI_INPUT_TEXT_FLAGS_PASSWORD 1024u
#define DMUI_UI_INPUT_TEXT_FLAGS_ALWAYS_OVERWRITE 2048u
#define DMUI_UI_INPUT_TEXT_FLAGS_AUTO_SELECT_ALL 4096u
#define DMUI_UI_INPUT_TEXT_FLAGS_PARSE_EMPTY_REF_VAL 8192u
#define DMUI_UI_INPUT_TEXT_FLAGS_DISPLAY_EMPTY_REF_VAL 16384u
#define DMUI_UI_INPUT_TEXT_FLAGS_NO_HORIZONTAL_SCROLL 32768u
#define DMUI_UI_INPUT_TEXT_FLAGS_NO_UNDO_REDO 65536u
#define DMUI_UI_INPUT_TEXT_FLAGS_ELIDE_LEFT 131072u
#define DMUI_UI_INPUT_TEXT_FLAGS_WORD_WRAP 262144u
#define DMUI_UI_INPUT_TEXT_FLAGS_REJECTED_CALLBACK_MASK 4227858432u

typedef uint32_t DMUI_UISelectableFlags;
#define DMUI_UI_SELECTABLE_FLAGS_NONE 0u
#define DMUI_UI_SELECTABLE_FLAGS_NO_AUTO_CLOSE_POPUPS 1u
#define DMUI_UI_SELECTABLE_FLAGS_SPAN_ALL_COLUMNS 2u
#define DMUI_UI_SELECTABLE_FLAGS_ALLOW_DOUBLE_CLICK 4u
#define DMUI_UI_SELECTABLE_FLAGS_DISABLED 8u
#define DMUI_UI_SELECTABLE_FLAGS_ALLOW_OVERLAP 16u
#define DMUI_UI_SELECTABLE_FLAGS_HIGHLIGHT 32u
#define DMUI_UI_SELECTABLE_FLAGS_SELECT_ON_NAV 64u

typedef uint32_t DMUI_UISliderFlags;
#define DMUI_UI_SLIDER_FLAGS_NONE 0u
#define DMUI_UI_SLIDER_FLAGS_LOGARITHMIC 1u
#define DMUI_UI_SLIDER_FLAGS_NO_ROUND_TO_FORMAT 2u
#define DMUI_UI_SLIDER_FLAGS_NO_INPUT 4u
#define DMUI_UI_SLIDER_FLAGS_WRAP_AROUND 8u
#define DMUI_UI_SLIDER_FLAGS_CLAMP_ON_INPUT 16u
#define DMUI_UI_SLIDER_FLAGS_CLAMP_ZERO_RANGE 32u
#define DMUI_UI_SLIDER_FLAGS_NO_SPEED_TWEAKS 64u
#define DMUI_UI_SLIDER_FLAGS_COLOR_MARKERS 128u
#define DMUI_UI_SLIDER_FLAGS_ALWAYS_CLAMP 48u

typedef uint32_t DMUI_UITableFlags;
#define DMUI_UI_TABLE_FLAGS_NONE 0u
#define DMUI_UI_TABLE_FLAGS_RESIZABLE 1u
#define DMUI_UI_TABLE_FLAGS_REORDERABLE 2u
#define DMUI_UI_TABLE_FLAGS_HIDEABLE 4u
#define DMUI_UI_TABLE_FLAGS_SORTABLE 8u
#define DMUI_UI_TABLE_FLAGS_NO_SAVED_SETTINGS 16u
#define DMUI_UI_TABLE_FLAGS_CONTEXT_MENU_IN_BODY 32u
#define DMUI_UI_TABLE_FLAGS_ROW_BG 64u
#define DMUI_UI_TABLE_FLAGS_BORDERS_INNER_H 128u
#define DMUI_UI_TABLE_FLAGS_BORDERS_OUTER_H 256u
#define DMUI_UI_TABLE_FLAGS_BORDERS_INNER_V 512u
#define DMUI_UI_TABLE_FLAGS_BORDERS_OUTER_V 1024u
#define DMUI_UI_TABLE_FLAGS_NO_BORDERS_IN_BODY 2048u
#define DMUI_UI_TABLE_FLAGS_NO_BORDERS_IN_BODY_UNTIL_RESIZE 4096u
#define DMUI_UI_TABLE_FLAGS_SIZING_FIXED_FIT 8192u
#define DMUI_UI_TABLE_FLAGS_SIZING_FIXED_SAME 16384u
#define DMUI_UI_TABLE_FLAGS_SIZING_STRETCH_PROP 32768u
#define DMUI_UI_TABLE_FLAGS_SIZING_STRETCH_SAME 65536u
#define DMUI_UI_TABLE_FLAGS_NO_HOST_EXTEND_X 131072u
#define DMUI_UI_TABLE_FLAGS_NO_HOST_EXTEND_Y 262144u
#define DMUI_UI_TABLE_FLAGS_NO_KEEP_COLUMNS_VISIBLE 524288u
#define DMUI_UI_TABLE_FLAGS_PRECISE_WIDTHS 1048576u
#define DMUI_UI_TABLE_FLAGS_NO_CLIP 2097152u
#define DMUI_UI_TABLE_FLAGS_PAD_OUTER_X 4194304u
#define DMUI_UI_TABLE_FLAGS_NO_PAD_OUTER_X 8388608u
#define DMUI_UI_TABLE_FLAGS_NO_PAD_INNER_X 16777216u
#define DMUI_UI_TABLE_FLAGS_SCROLL_X 33554432u
#define DMUI_UI_TABLE_FLAGS_SCROLL_Y 67108864u
#define DMUI_UI_TABLE_FLAGS_SORT_MULTI 134217728u
#define DMUI_UI_TABLE_FLAGS_SORT_TRISTATE 268435456u
#define DMUI_UI_TABLE_FLAGS_HIGHLIGHT_HOVERED_COLUMN 536870912u
#define DMUI_UI_TABLE_FLAGS_BORDERS_H 384u
#define DMUI_UI_TABLE_FLAGS_BORDERS_V 1536u
#define DMUI_UI_TABLE_FLAGS_BORDERS_INNER 640u
#define DMUI_UI_TABLE_FLAGS_BORDERS_OUTER 1280u
#define DMUI_UI_TABLE_FLAGS_BORDERS 1920u

typedef uint32_t DMUI_UITableColumnFlags;
#define DMUI_UI_TABLE_COLUMN_FLAGS_NONE 0u
#define DMUI_UI_TABLE_COLUMN_FLAGS_DISABLED 1u
#define DMUI_UI_TABLE_COLUMN_FLAGS_DEFAULT_HIDE 2u
#define DMUI_UI_TABLE_COLUMN_FLAGS_DEFAULT_SORT 4u
#define DMUI_UI_TABLE_COLUMN_FLAGS_WIDTH_STRETCH 8u
#define DMUI_UI_TABLE_COLUMN_FLAGS_WIDTH_FIXED 16u
#define DMUI_UI_TABLE_COLUMN_FLAGS_NO_RESIZE 32u
#define DMUI_UI_TABLE_COLUMN_FLAGS_NO_REORDER 64u
#define DMUI_UI_TABLE_COLUMN_FLAGS_NO_HIDE 128u
#define DMUI_UI_TABLE_COLUMN_FLAGS_NO_CLIP 256u
#define DMUI_UI_TABLE_COLUMN_FLAGS_NO_SORT 512u
#define DMUI_UI_TABLE_COLUMN_FLAGS_NO_SORT_ASCENDING 1024u
#define DMUI_UI_TABLE_COLUMN_FLAGS_NO_SORT_DESCENDING 2048u
#define DMUI_UI_TABLE_COLUMN_FLAGS_NO_HEADER_LABEL 4096u
#define DMUI_UI_TABLE_COLUMN_FLAGS_NO_HEADER_WIDTH 8192u
#define DMUI_UI_TABLE_COLUMN_FLAGS_PREFER_SORT_ASCENDING 16384u
#define DMUI_UI_TABLE_COLUMN_FLAGS_PREFER_SORT_DESCENDING 32768u
#define DMUI_UI_TABLE_COLUMN_FLAGS_INDENT_ENABLE 65536u
#define DMUI_UI_TABLE_COLUMN_FLAGS_INDENT_DISABLE 131072u
#define DMUI_UI_TABLE_COLUMN_FLAGS_ANGLED_HEADER 262144u

typedef uint32_t DMUI_UITableRowFlags;
#define DMUI_UI_TABLE_ROW_FLAGS_NONE 0u
#define DMUI_UI_TABLE_ROW_FLAGS_HEADERS 1u

typedef uint32_t DMUI_UITreeNodeFlags;
#define DMUI_UI_TREE_NODE_FLAGS_NONE 0u
#define DMUI_UI_TREE_NODE_FLAGS_SELECTED 1u
#define DMUI_UI_TREE_NODE_FLAGS_FRAMED 2u
#define DMUI_UI_TREE_NODE_FLAGS_ALLOW_OVERLAP 4u
#define DMUI_UI_TREE_NODE_FLAGS_NO_TREE_PUSH_ON_OPEN 8u
#define DMUI_UI_TREE_NODE_FLAGS_NO_AUTO_OPEN_ON_LOG 16u
#define DMUI_UI_TREE_NODE_FLAGS_DEFAULT_OPEN 32u
#define DMUI_UI_TREE_NODE_FLAGS_OPEN_ON_DOUBLE_CLICK 64u
#define DMUI_UI_TREE_NODE_FLAGS_OPEN_ON_ARROW 128u
#define DMUI_UI_TREE_NODE_FLAGS_LEAF 256u
#define DMUI_UI_TREE_NODE_FLAGS_BULLET 512u
#define DMUI_UI_TREE_NODE_FLAGS_FRAME_PADDING 1024u
#define DMUI_UI_TREE_NODE_FLAGS_SPAN_AVAIL_WIDTH 2048u
#define DMUI_UI_TREE_NODE_FLAGS_SPAN_FULL_WIDTH 4096u
#define DMUI_UI_TREE_NODE_FLAGS_SPAN_LABEL_WIDTH 8192u
#define DMUI_UI_TREE_NODE_FLAGS_SPAN_ALL_COLUMNS 16384u
#define DMUI_UI_TREE_NODE_FLAGS_LABEL_SPAN_ALL_COLUMNS 32768u
#define DMUI_UI_TREE_NODE_FLAGS_NAV_LEFT_JUMPS_TO_PARENT 65536u
#define DMUI_UI_TREE_NODE_FLAGS_DRAW_LINES_NONE 131072u
#define DMUI_UI_TREE_NODE_FLAGS_DRAW_LINES_FULL 262144u
#define DMUI_UI_TREE_NODE_FLAGS_DRAW_LINES_TO_NODES 524288u
#define DMUI_UI_TREE_NODE_FLAGS_COLLAPSING_HEADER 26u

typedef DMUI_Result (DMUI_CALL *DMUI_UIGetStyleMetricsFn)(
	DMUI_ClientHandle client,
	DMUI_StyleMetrics* metrics) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIBeginComboFn)(
	DMUI_ClientHandle client,
	const char* label,
	const char* previewValue,
	DMUI_UIComboFlags flags,
	uint32_t* visible) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIEndComboFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIBeginDisabledFn)(
	DMUI_ClientHandle client,
	uint32_t disabled) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIEndDisabledFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIBeginTableFn)(
	DMUI_ClientHandle client,
	const char* id,
	int32_t columns,
	DMUI_UITableFlags flags,
	DMUI_Vec2 outerSize,
	float innerWidth,
	uint32_t* visible) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIEndTableFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIBeginTooltipFn)(
	DMUI_ClientHandle client,
	uint32_t* visible) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIEndTooltipFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIButtonFn)(
	DMUI_ClientHandle client,
	const char* label,
	DMUI_Vec2 size,
	uint32_t* pressed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UICalcTextSizeFn)(
	DMUI_ClientHandle client,
	const char* text,
	size_t textLength,
	uint32_t hideTextAfterDoubleHash,
	float wrapWidth,
	DMUI_Vec2* size) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UICheckboxFn)(
	DMUI_ClientHandle client,
	const char* label,
	uint32_t* value,
	uint32_t* changed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UICollapsingHeaderFn)(
	DMUI_ClientHandle client,
	const char* label,
	DMUI_UITreeNodeFlags flags,
	uint32_t* open) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UICollapsingHeaderVisibleFn)(
	DMUI_ClientHandle client,
	const char* label,
	uint32_t* visible,
	DMUI_UITreeNodeFlags flags,
	uint32_t* open) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIDragScalarFn)(
	DMUI_ClientHandle client,
	const char* label,
	DMUI_UIDataType dataType,
	void* data,
	uint32_t dataSize,
	float speed,
	const void* minimum,
	uint32_t minimumSize,
	const void* maximum,
	uint32_t maximumSize,
	const char* format,
	DMUI_UISliderFlags flags,
	uint32_t* changed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIDummyFn)(
	DMUI_ClientHandle client,
	DMUI_Vec2 size) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIGetContentRegionAvailFn)(
	DMUI_ClientHandle client,
	DMUI_Vec2* size) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIGetCursorScreenPosFn)(
	DMUI_ClientHandle client,
	DMUI_Vec2* position) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIGetFontSizeFn)(
	DMUI_ClientHandle client,
	float* size) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIGetFrameHeightFn)(
	DMUI_ClientHandle client,
	float* height) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIGetStyleColorFn)(
	DMUI_ClientHandle client,
	DMUI_UIColor color,
	DMUI_Vec4* value) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIGetTextLineHeightWithSpacingFn)(
	DMUI_ClientHandle client,
	float* height) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIIndentFn)(
	DMUI_ClientHandle client,
	float width) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIInputScalarFn)(
	DMUI_ClientHandle client,
	const char* label,
	DMUI_UIDataType dataType,
	void* data,
	uint32_t dataSize,
	const void* step,
	uint32_t stepSize,
	const void* fastStep,
	uint32_t fastStepSize,
	const char* format,
	DMUI_UIInputTextFlags flags,
	uint32_t* changed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIInputTextFn)(
	DMUI_ClientHandle client,
	const char* label,
	char* buffer,
	uint32_t capacity,
	DMUI_UIInputTextFlags flags,
	uint32_t* changed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIInputTextMultilineFn)(
	DMUI_ClientHandle client,
	const char* label,
	char* buffer,
	uint32_t capacity,
	DMUI_Vec2 size,
	DMUI_UIInputTextFlags flags,
	uint32_t* changed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIInputTextWithHintFn)(
	DMUI_ClientHandle client,
	const char* label,
	const char* hint,
	char* buffer,
	uint32_t capacity,
	DMUI_UIInputTextFlags flags,
	uint32_t* changed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIIsItemDeactivatedAfterEditFn)(
	DMUI_ClientHandle client,
	uint32_t* deactivated) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIIsItemHoveredFn)(
	DMUI_ClientHandle client,
	DMUI_UIHoveredFlags flags,
	uint32_t* hovered) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIPopIDFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIPopStyleColorFn)(
	DMUI_ClientHandle client,
	int32_t count) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIPopTextWrapPosFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIProgressBarFn)(
	DMUI_ClientHandle client,
	float fraction,
	DMUI_Vec2 size,
	const char* overlay,
	size_t overlayLength) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIPushIDStringFn)(
	DMUI_ClientHandle client,
	const char* id) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIPushIDRangeFn)(
	DMUI_ClientHandle client,
	const char* id,
	size_t length) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIPushIDValueFn)(
	DMUI_ClientHandle client,
	uint64_t id) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIPushStyleColorU32Fn)(
	DMUI_ClientHandle client,
	DMUI_UIColor color,
	uint32_t rgba) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIPushStyleColorFn)(
	DMUI_ClientHandle client,
	DMUI_UIColor color,
	DMUI_Vec4 value) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIPushTextWrapPosFn)(
	DMUI_ClientHandle client,
	float localX) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISameLineFn)(
	DMUI_ClientHandle client,
	float offsetFromStartX,
	float spacing) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISelectableFn)(
	DMUI_ClientHandle client,
	const char* label,
	uint32_t selected,
	DMUI_UISelectableFlags flags,
	DMUI_Vec2 size,
	uint32_t* pressed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISelectableToggleFn)(
	DMUI_ClientHandle client,
	const char* label,
	uint32_t* selected,
	DMUI_UISelectableFlags flags,
	DMUI_Vec2 size,
	uint32_t* pressed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISeparatorFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISetClipboardTextFn)(
	DMUI_ClientHandle client,
	const char* text,
	size_t textLength) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISetCursorScreenPosFn)(
	DMUI_ClientHandle client,
	DMUI_Vec2 position) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISetItemDefaultFocusFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISetNextItemWidthFn)(
	DMUI_ClientHandle client,
	float width) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISetTooltipTextFn)(
	DMUI_ClientHandle client,
	const char* text,
	size_t textLength) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISliderScalarFn)(
	DMUI_ClientHandle client,
	const char* label,
	DMUI_UIDataType dataType,
	void* data,
	uint32_t dataSize,
	const void* minimum,
	uint32_t minimumSize,
	const void* maximum,
	uint32_t maximumSize,
	const char* format,
	DMUI_UISliderFlags flags,
	uint32_t* changed) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UISpacingFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UITableHeadersRowFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UITableNextColumnFn)(
	DMUI_ClientHandle client,
	uint32_t* visible) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UITableNextRowFn)(
	DMUI_ClientHandle client,
	DMUI_UITableRowFlags flags,
	float minimumHeight) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UITableSetColumnIndexFn)(
	DMUI_ClientHandle client,
	int32_t column,
	uint32_t* visible) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UITableSetupColumnFn)(
	DMUI_ClientHandle client,
	const char* label,
	DMUI_UITableColumnFlags flags,
	float initialWidthOrWeight,
	uint32_t userId) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UITableSetupScrollFreezeFn)(
	DMUI_ClientHandle client,
	int32_t columns,
	int32_t rows) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UITextFn)(
	DMUI_ClientHandle client,
	const char* text,
	size_t textLength) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UITextColoredFn)(
	DMUI_ClientHandle client,
	DMUI_Vec4 color,
	const char* text,
	size_t textLength) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UITextDisabledFn)(
	DMUI_ClientHandle client,
	const char* text,
	size_t textLength) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UITextWrappedFn)(
	DMUI_ClientHandle client,
	const char* text,
	size_t textLength) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIUnindentFn)(
	DMUI_ClientHandle client,
	float width) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UINewLineFn)(
	DMUI_ClientHandle client) DMUI_NOEXCEPT;
typedef DMUI_Result (DMUI_CALL *DMUI_UIPlotLinesFn)(
	DMUI_ClientHandle client,
	const char* label,
	const float* values,
	int32_t valueCount,
	int32_t valueOffset,
	const char* overlay,
	size_t overlayLength,
	float scaleMinimum,
	float scaleMaximum,
	DMUI_Vec2 size,
	uint32_t strideBytes) DMUI_NOEXCEPT;

typedef struct DMUI_UIAPI
{
	uint32_t structSize;
	uint32_t abiVersion;
	uint32_t revision;
	uint32_t reserved;
	DMUI_UIGetStyleMetricsFn getStyleMetrics;
	DMUI_UIBeginComboFn beginCombo;
	DMUI_UIEndComboFn endCombo;
	DMUI_UIBeginDisabledFn beginDisabled;
	DMUI_UIEndDisabledFn endDisabled;
	DMUI_UIBeginTableFn beginTable;
	DMUI_UIEndTableFn endTable;
	DMUI_UIBeginTooltipFn beginTooltip;
	DMUI_UIEndTooltipFn endTooltip;
	DMUI_UIButtonFn button;
	DMUI_UICalcTextSizeFn calcTextSize;
	DMUI_UICheckboxFn checkbox;
	DMUI_UICollapsingHeaderFn collapsingHeader;
	DMUI_UICollapsingHeaderVisibleFn collapsingHeaderVisible;
	DMUI_UIDragScalarFn dragScalar;
	DMUI_UIDummyFn dummy;
	DMUI_UIGetContentRegionAvailFn getContentRegionAvail;
	DMUI_UIGetCursorScreenPosFn getCursorScreenPos;
	DMUI_UIGetFontSizeFn getFontSize;
	DMUI_UIGetFrameHeightFn getFrameHeight;
	DMUI_UIGetStyleColorFn getStyleColor;
	DMUI_UIGetTextLineHeightWithSpacingFn getTextLineHeightWithSpacing;
	DMUI_UIIndentFn indent;
	DMUI_UIInputScalarFn inputScalar;
	DMUI_UIInputTextFn inputText;
	DMUI_UIInputTextMultilineFn inputTextMultiline;
	DMUI_UIInputTextWithHintFn inputTextWithHint;
	DMUI_UIIsItemDeactivatedAfterEditFn isItemDeactivatedAfterEdit;
	DMUI_UIIsItemHoveredFn isItemHovered;
	DMUI_UIPopIDFn popID;
	DMUI_UIPopStyleColorFn popStyleColor;
	DMUI_UIPopTextWrapPosFn popTextWrapPos;
	DMUI_UIProgressBarFn progressBar;
	DMUI_UIPushIDStringFn pushIDString;
	DMUI_UIPushIDRangeFn pushIDRange;
	DMUI_UIPushIDValueFn pushIDValue;
	DMUI_UIPushStyleColorU32Fn pushStyleColorU32;
	DMUI_UIPushStyleColorFn pushStyleColor;
	DMUI_UIPushTextWrapPosFn pushTextWrapPos;
	DMUI_UISameLineFn sameLine;
	DMUI_UISelectableFn selectable;
	DMUI_UISelectableToggleFn selectableToggle;
	DMUI_UISeparatorFn separator;
	DMUI_UISetClipboardTextFn setClipboardText;
	DMUI_UISetCursorScreenPosFn setCursorScreenPos;
	DMUI_UISetItemDefaultFocusFn setItemDefaultFocus;
	DMUI_UISetNextItemWidthFn setNextItemWidth;
	DMUI_UISetTooltipTextFn setTooltipText;
	DMUI_UISliderScalarFn sliderScalar;
	DMUI_UISpacingFn spacing;
	DMUI_UITableHeadersRowFn tableHeadersRow;
	DMUI_UITableNextColumnFn tableNextColumn;
	DMUI_UITableNextRowFn tableNextRow;
	DMUI_UITableSetColumnIndexFn tableSetColumnIndex;
	DMUI_UITableSetupColumnFn tableSetupColumn;
	DMUI_UITableSetupScrollFreezeFn tableSetupScrollFreeze;
	DMUI_UITextFn text;
	DMUI_UITextColoredFn textColored;
	DMUI_UITextDisabledFn textDisabled;
	DMUI_UITextWrappedFn textWrapped;
	DMUI_UIUnindentFn unindent;
	DMUI_UINewLineFn newLine;
	DMUI_UIPlotLinesFn plotLines;
} DMUI_UIAPI;

#define DMUI_UI_API_GET_STYLE_METRICS_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, getStyleMetrics) + sizeof(DMUI_UIGetStyleMetricsFn)))
#define DMUI_UI_API_BEGIN_COMBO_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, beginCombo) + sizeof(DMUI_UIBeginComboFn)))
#define DMUI_UI_API_END_COMBO_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, endCombo) + sizeof(DMUI_UIEndComboFn)))
#define DMUI_UI_API_BEGIN_DISABLED_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, beginDisabled) + sizeof(DMUI_UIBeginDisabledFn)))
#define DMUI_UI_API_END_DISABLED_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, endDisabled) + sizeof(DMUI_UIEndDisabledFn)))
#define DMUI_UI_API_BEGIN_TABLE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, beginTable) + sizeof(DMUI_UIBeginTableFn)))
#define DMUI_UI_API_END_TABLE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, endTable) + sizeof(DMUI_UIEndTableFn)))
#define DMUI_UI_API_BEGIN_TOOLTIP_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, beginTooltip) + sizeof(DMUI_UIBeginTooltipFn)))
#define DMUI_UI_API_END_TOOLTIP_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, endTooltip) + sizeof(DMUI_UIEndTooltipFn)))
#define DMUI_UI_API_BUTTON_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, button) + sizeof(DMUI_UIButtonFn)))
#define DMUI_UI_API_CALC_TEXT_SIZE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, calcTextSize) + sizeof(DMUI_UICalcTextSizeFn)))
#define DMUI_UI_API_CHECKBOX_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, checkbox) + sizeof(DMUI_UICheckboxFn)))
#define DMUI_UI_API_COLLAPSING_HEADER_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, collapsingHeader) + sizeof(DMUI_UICollapsingHeaderFn)))
#define DMUI_UI_API_COLLAPSING_HEADER_VISIBLE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, collapsingHeaderVisible) + sizeof(DMUI_UICollapsingHeaderVisibleFn)))
#define DMUI_UI_API_DRAG_SCALAR_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, dragScalar) + sizeof(DMUI_UIDragScalarFn)))
#define DMUI_UI_API_DUMMY_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, dummy) + sizeof(DMUI_UIDummyFn)))
#define DMUI_UI_API_GET_CONTENT_REGION_AVAIL_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, getContentRegionAvail) + sizeof(DMUI_UIGetContentRegionAvailFn)))
#define DMUI_UI_API_GET_CURSOR_SCREEN_POS_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, getCursorScreenPos) + sizeof(DMUI_UIGetCursorScreenPosFn)))
#define DMUI_UI_API_GET_FONT_SIZE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, getFontSize) + sizeof(DMUI_UIGetFontSizeFn)))
#define DMUI_UI_API_GET_FRAME_HEIGHT_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, getFrameHeight) + sizeof(DMUI_UIGetFrameHeightFn)))
#define DMUI_UI_API_GET_STYLE_COLOR_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, getStyleColor) + sizeof(DMUI_UIGetStyleColorFn)))
#define DMUI_UI_API_GET_TEXT_LINE_HEIGHT_WITH_SPACING_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, getTextLineHeightWithSpacing) + sizeof(DMUI_UIGetTextLineHeightWithSpacingFn)))
#define DMUI_UI_API_INDENT_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, indent) + sizeof(DMUI_UIIndentFn)))
#define DMUI_UI_API_INPUT_SCALAR_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, inputScalar) + sizeof(DMUI_UIInputScalarFn)))
#define DMUI_UI_API_INPUT_TEXT_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, inputText) + sizeof(DMUI_UIInputTextFn)))
#define DMUI_UI_API_INPUT_TEXT_MULTILINE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, inputTextMultiline) + sizeof(DMUI_UIInputTextMultilineFn)))
#define DMUI_UI_API_INPUT_TEXT_WITH_HINT_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, inputTextWithHint) + sizeof(DMUI_UIInputTextWithHintFn)))
#define DMUI_UI_API_IS_ITEM_DEACTIVATED_AFTER_EDIT_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, isItemDeactivatedAfterEdit) + sizeof(DMUI_UIIsItemDeactivatedAfterEditFn)))
#define DMUI_UI_API_IS_ITEM_HOVERED_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, isItemHovered) + sizeof(DMUI_UIIsItemHoveredFn)))
#define DMUI_UI_API_POP_ID_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, popID) + sizeof(DMUI_UIPopIDFn)))
#define DMUI_UI_API_POP_STYLE_COLOR_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, popStyleColor) + sizeof(DMUI_UIPopStyleColorFn)))
#define DMUI_UI_API_POP_TEXT_WRAP_POS_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, popTextWrapPos) + sizeof(DMUI_UIPopTextWrapPosFn)))
#define DMUI_UI_API_PROGRESS_BAR_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, progressBar) + sizeof(DMUI_UIProgressBarFn)))
#define DMUI_UI_API_PUSH_ID_STRING_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, pushIDString) + sizeof(DMUI_UIPushIDStringFn)))
#define DMUI_UI_API_PUSH_ID_RANGE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, pushIDRange) + sizeof(DMUI_UIPushIDRangeFn)))
#define DMUI_UI_API_PUSH_ID_VALUE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, pushIDValue) + sizeof(DMUI_UIPushIDValueFn)))
#define DMUI_UI_API_PUSH_STYLE_COLOR_U32_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, pushStyleColorU32) + sizeof(DMUI_UIPushStyleColorU32Fn)))
#define DMUI_UI_API_PUSH_STYLE_COLOR_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, pushStyleColor) + sizeof(DMUI_UIPushStyleColorFn)))
#define DMUI_UI_API_PUSH_TEXT_WRAP_POS_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, pushTextWrapPos) + sizeof(DMUI_UIPushTextWrapPosFn)))
#define DMUI_UI_API_SAME_LINE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, sameLine) + sizeof(DMUI_UISameLineFn)))
#define DMUI_UI_API_SELECTABLE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, selectable) + sizeof(DMUI_UISelectableFn)))
#define DMUI_UI_API_SELECTABLE_TOGGLE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, selectableToggle) + sizeof(DMUI_UISelectableToggleFn)))
#define DMUI_UI_API_SEPARATOR_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, separator) + sizeof(DMUI_UISeparatorFn)))
#define DMUI_UI_API_SET_CLIPBOARD_TEXT_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, setClipboardText) + sizeof(DMUI_UISetClipboardTextFn)))
#define DMUI_UI_API_SET_CURSOR_SCREEN_POS_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, setCursorScreenPos) + sizeof(DMUI_UISetCursorScreenPosFn)))
#define DMUI_UI_API_SET_ITEM_DEFAULT_FOCUS_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, setItemDefaultFocus) + sizeof(DMUI_UISetItemDefaultFocusFn)))
#define DMUI_UI_API_SET_NEXT_ITEM_WIDTH_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, setNextItemWidth) + sizeof(DMUI_UISetNextItemWidthFn)))
#define DMUI_UI_API_SET_TOOLTIP_TEXT_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, setTooltipText) + sizeof(DMUI_UISetTooltipTextFn)))
#define DMUI_UI_API_SLIDER_SCALAR_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, sliderScalar) + sizeof(DMUI_UISliderScalarFn)))
#define DMUI_UI_API_SPACING_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, spacing) + sizeof(DMUI_UISpacingFn)))
#define DMUI_UI_API_TABLE_HEADERS_ROW_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, tableHeadersRow) + sizeof(DMUI_UITableHeadersRowFn)))
#define DMUI_UI_API_TABLE_NEXT_COLUMN_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, tableNextColumn) + sizeof(DMUI_UITableNextColumnFn)))
#define DMUI_UI_API_TABLE_NEXT_ROW_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, tableNextRow) + sizeof(DMUI_UITableNextRowFn)))
#define DMUI_UI_API_TABLE_SET_COLUMN_INDEX_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, tableSetColumnIndex) + sizeof(DMUI_UITableSetColumnIndexFn)))
#define DMUI_UI_API_TABLE_SETUP_COLUMN_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, tableSetupColumn) + sizeof(DMUI_UITableSetupColumnFn)))
#define DMUI_UI_API_TABLE_SETUP_SCROLL_FREEZE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, tableSetupScrollFreeze) + sizeof(DMUI_UITableSetupScrollFreezeFn)))
#define DMUI_UI_API_TEXT_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, text) + sizeof(DMUI_UITextFn)))
#define DMUI_UI_API_TEXT_COLORED_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, textColored) + sizeof(DMUI_UITextColoredFn)))
#define DMUI_UI_API_TEXT_DISABLED_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, textDisabled) + sizeof(DMUI_UITextDisabledFn)))
#define DMUI_UI_API_TEXT_WRAPPED_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, textWrapped) + sizeof(DMUI_UITextWrappedFn)))
#define DMUI_UI_API_UNINDENT_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, unindent) + sizeof(DMUI_UIUnindentFn)))
#define DMUI_UI_API_NEW_LINE_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, newLine) + sizeof(DMUI_UINewLineFn)))
#define DMUI_UI_API_PLOT_LINES_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPI, plotLines) + sizeof(DMUI_UIPlotLinesFn)))
#define DMUI_UI_API_REQUIRED_SIZE DMUI_UI_API_NEW_LINE_SIZE
#define DMUI_UI_API_CURRENT_SIZE DMUI_UI_API_PLOT_LINES_SIZE

typedef struct DMUI_UIAPIInfo
{
	uint32_t structSize;
	uint32_t abiVersion;
	uint32_t revision;
	uint32_t tableSize;
	const DMUI_UIAPI* api;
} DMUI_UIAPIInfo;

#define DMUI_UI_API_INFO_PREFIX_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPIInfo, tableSize) + sizeof(uint32_t)))
#define DMUI_UI_API_INFO_1_SIZE \
	((uint32_t)(offsetof(DMUI_UIAPIInfo, api) + sizeof(const DMUI_UIAPI*)))
