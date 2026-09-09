#pragma once

// Generated from schema/ui-contract.json; do not edit.
// Included by DearModdingUI/UI.h after its context implementation.

namespace dmui::ui
{
	using Vec2 = DMUI_Vec2;
	using Vec4 = DMUI_Vec4;
	using Color32 = uint32_t;
	using ID = uint32_t;

	enum class Color : uint32_t
	{
		kNone = 0u,
		kText = DMUI_UI_COLOR_TEXT,
		kTextDisabled = DMUI_UI_COLOR_TEXT_DISABLED,
		kWindowBg = DMUI_UI_COLOR_WINDOW_BG,
		kChildBg = DMUI_UI_COLOR_CHILD_BG,
		kPopupBg = DMUI_UI_COLOR_POPUP_BG,
		kBorder = DMUI_UI_COLOR_BORDER,
		kBorderShadow = DMUI_UI_COLOR_BORDER_SHADOW,
		kFrameBg = DMUI_UI_COLOR_FRAME_BG,
		kFrameBgHovered = DMUI_UI_COLOR_FRAME_BG_HOVERED,
		kFrameBgActive = DMUI_UI_COLOR_FRAME_BG_ACTIVE,
		kTitleBg = DMUI_UI_COLOR_TITLE_BG,
		kTitleBgActive = DMUI_UI_COLOR_TITLE_BG_ACTIVE,
		kTitleBgCollapsed = DMUI_UI_COLOR_TITLE_BG_COLLAPSED,
		kMenuBarBg = DMUI_UI_COLOR_MENU_BAR_BG,
		kScrollbarBg = DMUI_UI_COLOR_SCROLLBAR_BG,
		kScrollbarGrab = DMUI_UI_COLOR_SCROLLBAR_GRAB,
		kScrollbarGrabHovered = DMUI_UI_COLOR_SCROLLBAR_GRAB_HOVERED,
		kScrollbarGrabActive = DMUI_UI_COLOR_SCROLLBAR_GRAB_ACTIVE,
		kCheckMark = DMUI_UI_COLOR_CHECK_MARK,
		kCheckboxSelectedBg = DMUI_UI_COLOR_CHECKBOX_SELECTED_BG,
		kSliderGrab = DMUI_UI_COLOR_SLIDER_GRAB,
		kSliderGrabActive = DMUI_UI_COLOR_SLIDER_GRAB_ACTIVE,
		kButton = DMUI_UI_COLOR_BUTTON,
		kButtonHovered = DMUI_UI_COLOR_BUTTON_HOVERED,
		kButtonActive = DMUI_UI_COLOR_BUTTON_ACTIVE,
		kHeader = DMUI_UI_COLOR_HEADER,
		kHeaderHovered = DMUI_UI_COLOR_HEADER_HOVERED,
		kHeaderActive = DMUI_UI_COLOR_HEADER_ACTIVE,
		kSeparator = DMUI_UI_COLOR_SEPARATOR,
		kSeparatorHovered = DMUI_UI_COLOR_SEPARATOR_HOVERED,
		kSeparatorActive = DMUI_UI_COLOR_SEPARATOR_ACTIVE,
		kResizeGrip = DMUI_UI_COLOR_RESIZE_GRIP,
		kResizeGripHovered = DMUI_UI_COLOR_RESIZE_GRIP_HOVERED,
		kResizeGripActive = DMUI_UI_COLOR_RESIZE_GRIP_ACTIVE,
		kInputTextCursor = DMUI_UI_COLOR_INPUT_TEXT_CURSOR,
		kTabHovered = DMUI_UI_COLOR_TAB_HOVERED,
		kTab = DMUI_UI_COLOR_TAB,
		kTabSelected = DMUI_UI_COLOR_TAB_SELECTED,
		kTabSelectedOverline = DMUI_UI_COLOR_TAB_SELECTED_OVERLINE,
		kTabDimmed = DMUI_UI_COLOR_TAB_DIMMED,
		kTabDimmedSelected = DMUI_UI_COLOR_TAB_DIMMED_SELECTED,
		kTabDimmedSelectedOverline = DMUI_UI_COLOR_TAB_DIMMED_SELECTED_OVERLINE,
		kDockingPreview = DMUI_UI_COLOR_DOCKING_PREVIEW,
		kDockingEmptyBg = DMUI_UI_COLOR_DOCKING_EMPTY_BG,
		kPlotLines = DMUI_UI_COLOR_PLOT_LINES,
		kPlotLinesHovered = DMUI_UI_COLOR_PLOT_LINES_HOVERED,
		kPlotHistogram = DMUI_UI_COLOR_PLOT_HISTOGRAM,
		kPlotHistogramHovered = DMUI_UI_COLOR_PLOT_HISTOGRAM_HOVERED,
		kTableHeaderBg = DMUI_UI_COLOR_TABLE_HEADER_BG,
		kTableBorderStrong = DMUI_UI_COLOR_TABLE_BORDER_STRONG,
		kTableBorderLight = DMUI_UI_COLOR_TABLE_BORDER_LIGHT,
		kTableRowBg = DMUI_UI_COLOR_TABLE_ROW_BG,
		kTableRowBgAlt = DMUI_UI_COLOR_TABLE_ROW_BG_ALT,
		kTextLink = DMUI_UI_COLOR_TEXT_LINK,
		kTextSelectedBg = DMUI_UI_COLOR_TEXT_SELECTED_BG,
		kTreeLines = DMUI_UI_COLOR_TREE_LINES,
		kDragDropTarget = DMUI_UI_COLOR_DRAG_DROP_TARGET,
		kDragDropTargetBg = DMUI_UI_COLOR_DRAG_DROP_TARGET_BG,
		kUnsavedMarker = DMUI_UI_COLOR_UNSAVED_MARKER,
		kNavCursor = DMUI_UI_COLOR_NAV_CURSOR,
		kNavWindowingHighlight = DMUI_UI_COLOR_NAV_WINDOWING_HIGHLIGHT,
		kNavWindowingDimBg = DMUI_UI_COLOR_NAV_WINDOWING_DIM_BG,
		kModalWindowDimBg = DMUI_UI_COLOR_MODAL_WINDOW_DIM_BG
	};

	enum class DataType : uint32_t
	{
		kNone = 0u,
		kS8 = DMUI_UI_DATA_TYPE_S8,
		kU8 = DMUI_UI_DATA_TYPE_U8,
		kS16 = DMUI_UI_DATA_TYPE_S16,
		kU16 = DMUI_UI_DATA_TYPE_U16,
		kS32 = DMUI_UI_DATA_TYPE_S32,
		kU32 = DMUI_UI_DATA_TYPE_U32,
		kS64 = DMUI_UI_DATA_TYPE_S64,
		kU64 = DMUI_UI_DATA_TYPE_U64,
		kFloat = DMUI_UI_DATA_TYPE_FLOAT,
		kDouble = DMUI_UI_DATA_TYPE_DOUBLE
	};

	enum class ComboFlags : uint32_t
	{
		kNone = 0u,
		kPopupAlignLeft = DMUI_UI_COMBO_FLAGS_POPUP_ALIGN_LEFT,
		kHeightSmall = DMUI_UI_COMBO_FLAGS_HEIGHT_SMALL,
		kHeightRegular = DMUI_UI_COMBO_FLAGS_HEIGHT_REGULAR,
		kHeightLarge = DMUI_UI_COMBO_FLAGS_HEIGHT_LARGE,
		kHeightLargest = DMUI_UI_COMBO_FLAGS_HEIGHT_LARGEST,
		kNoArrowButton = DMUI_UI_COMBO_FLAGS_NO_ARROW_BUTTON,
		kNoPreview = DMUI_UI_COMBO_FLAGS_NO_PREVIEW,
		kWidthFitPreview = DMUI_UI_COMBO_FLAGS_WIDTH_FIT_PREVIEW
	};

	[[nodiscard]] constexpr ComboFlags operator|(
		ComboFlags a_left, ComboFlags a_right) noexcept
	{
		return static_cast<ComboFlags>(
			static_cast<uint32_t>(a_left) |
			static_cast<uint32_t>(a_right));
	}

	[[nodiscard]] constexpr ComboFlags operator&(
		ComboFlags a_left, ComboFlags a_right) noexcept
	{
		return static_cast<ComboFlags>(
			static_cast<uint32_t>(a_left) &
			static_cast<uint32_t>(a_right));
	}

	constexpr ComboFlags& operator|=(
		ComboFlags& a_left, ComboFlags a_right) noexcept
	{
		a_left = a_left | a_right;
		return a_left;
	}

	enum class HoveredFlags : uint32_t
	{
		kNone = 0u,
		kChildWindows = DMUI_UI_HOVERED_FLAGS_CHILD_WINDOWS,
		kRootWindow = DMUI_UI_HOVERED_FLAGS_ROOT_WINDOW,
		kAnyWindow = DMUI_UI_HOVERED_FLAGS_ANY_WINDOW,
		kNoPopupHierarchy = DMUI_UI_HOVERED_FLAGS_NO_POPUP_HIERARCHY,
		kDockHierarchy = DMUI_UI_HOVERED_FLAGS_DOCK_HIERARCHY,
		kAllowWhenBlockedByPopup = DMUI_UI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_POPUP,
		kAllowWhenBlockedByActiveItem = DMUI_UI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_ACTIVE_ITEM,
		kAllowWhenOverlappedByItem = DMUI_UI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_ITEM,
		kAllowWhenOverlappedByWindow = DMUI_UI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_WINDOW,
		kAllowWhenDisabled = DMUI_UI_HOVERED_FLAGS_ALLOW_WHEN_DISABLED,
		kNoNavOverride = DMUI_UI_HOVERED_FLAGS_NO_NAV_OVERRIDE,
		kForTooltip = DMUI_UI_HOVERED_FLAGS_FOR_TOOLTIP,
		kStationary = DMUI_UI_HOVERED_FLAGS_STATIONARY,
		kDelayNone = DMUI_UI_HOVERED_FLAGS_DELAY_NONE,
		kDelayShort = DMUI_UI_HOVERED_FLAGS_DELAY_SHORT,
		kDelayNormal = DMUI_UI_HOVERED_FLAGS_DELAY_NORMAL,
		kNoSharedDelay = DMUI_UI_HOVERED_FLAGS_NO_SHARED_DELAY
	};

	[[nodiscard]] constexpr HoveredFlags operator|(
		HoveredFlags a_left, HoveredFlags a_right) noexcept
	{
		return static_cast<HoveredFlags>(
			static_cast<uint32_t>(a_left) |
			static_cast<uint32_t>(a_right));
	}

	[[nodiscard]] constexpr HoveredFlags operator&(
		HoveredFlags a_left, HoveredFlags a_right) noexcept
	{
		return static_cast<HoveredFlags>(
			static_cast<uint32_t>(a_left) &
			static_cast<uint32_t>(a_right));
	}

	constexpr HoveredFlags& operator|=(
		HoveredFlags& a_left, HoveredFlags a_right) noexcept
	{
		a_left = a_left | a_right;
		return a_left;
	}

	enum class InputTextFlags : uint32_t
	{
		kNone = 0u,
		kCharsDecimal = DMUI_UI_INPUT_TEXT_FLAGS_CHARS_DECIMAL,
		kCharsHexadecimal = DMUI_UI_INPUT_TEXT_FLAGS_CHARS_HEXADECIMAL,
		kCharsScientific = DMUI_UI_INPUT_TEXT_FLAGS_CHARS_SCIENTIFIC,
		kCharsUppercase = DMUI_UI_INPUT_TEXT_FLAGS_CHARS_UPPERCASE,
		kCharsNoBlank = DMUI_UI_INPUT_TEXT_FLAGS_CHARS_NO_BLANK,
		kAllowTabInput = DMUI_UI_INPUT_TEXT_FLAGS_ALLOW_TAB_INPUT,
		kEnterReturnsTrue = DMUI_UI_INPUT_TEXT_FLAGS_ENTER_RETURNS_TRUE,
		kEscapeClearsAll = DMUI_UI_INPUT_TEXT_FLAGS_ESCAPE_CLEARS_ALL,
		kCtrlEnterForNewLine = DMUI_UI_INPUT_TEXT_FLAGS_CTRL_ENTER_FOR_NEW_LINE,
		kReadOnly = DMUI_UI_INPUT_TEXT_FLAGS_READ_ONLY,
		kPassword = DMUI_UI_INPUT_TEXT_FLAGS_PASSWORD,
		kAlwaysOverwrite = DMUI_UI_INPUT_TEXT_FLAGS_ALWAYS_OVERWRITE,
		kAutoSelectAll = DMUI_UI_INPUT_TEXT_FLAGS_AUTO_SELECT_ALL,
		kParseEmptyRefVal = DMUI_UI_INPUT_TEXT_FLAGS_PARSE_EMPTY_REF_VAL,
		kDisplayEmptyRefVal = DMUI_UI_INPUT_TEXT_FLAGS_DISPLAY_EMPTY_REF_VAL,
		kNoHorizontalScroll = DMUI_UI_INPUT_TEXT_FLAGS_NO_HORIZONTAL_SCROLL,
		kNoUndoRedo = DMUI_UI_INPUT_TEXT_FLAGS_NO_UNDO_REDO,
		kElideLeft = DMUI_UI_INPUT_TEXT_FLAGS_ELIDE_LEFT,
		kWordWrap = DMUI_UI_INPUT_TEXT_FLAGS_WORD_WRAP
	};

	[[nodiscard]] constexpr InputTextFlags operator|(
		InputTextFlags a_left, InputTextFlags a_right) noexcept
	{
		return static_cast<InputTextFlags>(
			static_cast<uint32_t>(a_left) |
			static_cast<uint32_t>(a_right));
	}

	[[nodiscard]] constexpr InputTextFlags operator&(
		InputTextFlags a_left, InputTextFlags a_right) noexcept
	{
		return static_cast<InputTextFlags>(
			static_cast<uint32_t>(a_left) &
			static_cast<uint32_t>(a_right));
	}

	constexpr InputTextFlags& operator|=(
		InputTextFlags& a_left, InputTextFlags a_right) noexcept
	{
		a_left = a_left | a_right;
		return a_left;
	}

	inline constexpr uint32_t kInputTextFlagsRejectedCallbackMask{ DMUI_UI_INPUT_TEXT_FLAGS_REJECTED_CALLBACK_MASK };

	enum class SelectableFlags : uint32_t
	{
		kNone = 0u,
		kNoAutoClosePopups = DMUI_UI_SELECTABLE_FLAGS_NO_AUTO_CLOSE_POPUPS,
		kSpanAllColumns = DMUI_UI_SELECTABLE_FLAGS_SPAN_ALL_COLUMNS,
		kAllowDoubleClick = DMUI_UI_SELECTABLE_FLAGS_ALLOW_DOUBLE_CLICK,
		kDisabled = DMUI_UI_SELECTABLE_FLAGS_DISABLED,
		kAllowOverlap = DMUI_UI_SELECTABLE_FLAGS_ALLOW_OVERLAP,
		kHighlight = DMUI_UI_SELECTABLE_FLAGS_HIGHLIGHT,
		kSelectOnNav = DMUI_UI_SELECTABLE_FLAGS_SELECT_ON_NAV
	};

	[[nodiscard]] constexpr SelectableFlags operator|(
		SelectableFlags a_left, SelectableFlags a_right) noexcept
	{
		return static_cast<SelectableFlags>(
			static_cast<uint32_t>(a_left) |
			static_cast<uint32_t>(a_right));
	}

	[[nodiscard]] constexpr SelectableFlags operator&(
		SelectableFlags a_left, SelectableFlags a_right) noexcept
	{
		return static_cast<SelectableFlags>(
			static_cast<uint32_t>(a_left) &
			static_cast<uint32_t>(a_right));
	}

	constexpr SelectableFlags& operator|=(
		SelectableFlags& a_left, SelectableFlags a_right) noexcept
	{
		a_left = a_left | a_right;
		return a_left;
	}

	enum class SliderFlags : uint32_t
	{
		kNone = 0u,
		kLogarithmic = DMUI_UI_SLIDER_FLAGS_LOGARITHMIC,
		kNoRoundToFormat = DMUI_UI_SLIDER_FLAGS_NO_ROUND_TO_FORMAT,
		kNoInput = DMUI_UI_SLIDER_FLAGS_NO_INPUT,
		kWrapAround = DMUI_UI_SLIDER_FLAGS_WRAP_AROUND,
		kClampOnInput = DMUI_UI_SLIDER_FLAGS_CLAMP_ON_INPUT,
		kClampZeroRange = DMUI_UI_SLIDER_FLAGS_CLAMP_ZERO_RANGE,
		kNoSpeedTweaks = DMUI_UI_SLIDER_FLAGS_NO_SPEED_TWEAKS,
		kColorMarkers = DMUI_UI_SLIDER_FLAGS_COLOR_MARKERS,
		kAlwaysClamp = DMUI_UI_SLIDER_FLAGS_ALWAYS_CLAMP
	};

	[[nodiscard]] constexpr SliderFlags operator|(
		SliderFlags a_left, SliderFlags a_right) noexcept
	{
		return static_cast<SliderFlags>(
			static_cast<uint32_t>(a_left) |
			static_cast<uint32_t>(a_right));
	}

	[[nodiscard]] constexpr SliderFlags operator&(
		SliderFlags a_left, SliderFlags a_right) noexcept
	{
		return static_cast<SliderFlags>(
			static_cast<uint32_t>(a_left) &
			static_cast<uint32_t>(a_right));
	}

	constexpr SliderFlags& operator|=(
		SliderFlags& a_left, SliderFlags a_right) noexcept
	{
		a_left = a_left | a_right;
		return a_left;
	}

	enum class TableFlags : uint32_t
	{
		kNone = 0u,
		kResizable = DMUI_UI_TABLE_FLAGS_RESIZABLE,
		kReorderable = DMUI_UI_TABLE_FLAGS_REORDERABLE,
		kHideable = DMUI_UI_TABLE_FLAGS_HIDEABLE,
		kSortable = DMUI_UI_TABLE_FLAGS_SORTABLE,
		kNoSavedSettings = DMUI_UI_TABLE_FLAGS_NO_SAVED_SETTINGS,
		kContextMenuInBody = DMUI_UI_TABLE_FLAGS_CONTEXT_MENU_IN_BODY,
		kRowBg = DMUI_UI_TABLE_FLAGS_ROW_BG,
		kBordersInnerH = DMUI_UI_TABLE_FLAGS_BORDERS_INNER_H,
		kBordersOuterH = DMUI_UI_TABLE_FLAGS_BORDERS_OUTER_H,
		kBordersInnerV = DMUI_UI_TABLE_FLAGS_BORDERS_INNER_V,
		kBordersOuterV = DMUI_UI_TABLE_FLAGS_BORDERS_OUTER_V,
		kNoBordersInBody = DMUI_UI_TABLE_FLAGS_NO_BORDERS_IN_BODY,
		kNoBordersInBodyUntilResize = DMUI_UI_TABLE_FLAGS_NO_BORDERS_IN_BODY_UNTIL_RESIZE,
		kSizingFixedFit = DMUI_UI_TABLE_FLAGS_SIZING_FIXED_FIT,
		kSizingFixedSame = DMUI_UI_TABLE_FLAGS_SIZING_FIXED_SAME,
		kSizingStretchProp = DMUI_UI_TABLE_FLAGS_SIZING_STRETCH_PROP,
		kSizingStretchSame = DMUI_UI_TABLE_FLAGS_SIZING_STRETCH_SAME,
		kNoHostExtendX = DMUI_UI_TABLE_FLAGS_NO_HOST_EXTEND_X,
		kNoHostExtendY = DMUI_UI_TABLE_FLAGS_NO_HOST_EXTEND_Y,
		kNoKeepColumnsVisible = DMUI_UI_TABLE_FLAGS_NO_KEEP_COLUMNS_VISIBLE,
		kPreciseWidths = DMUI_UI_TABLE_FLAGS_PRECISE_WIDTHS,
		kNoClip = DMUI_UI_TABLE_FLAGS_NO_CLIP,
		kPadOuterX = DMUI_UI_TABLE_FLAGS_PAD_OUTER_X,
		kNoPadOuterX = DMUI_UI_TABLE_FLAGS_NO_PAD_OUTER_X,
		kNoPadInnerX = DMUI_UI_TABLE_FLAGS_NO_PAD_INNER_X,
		kScrollX = DMUI_UI_TABLE_FLAGS_SCROLL_X,
		kScrollY = DMUI_UI_TABLE_FLAGS_SCROLL_Y,
		kSortMulti = DMUI_UI_TABLE_FLAGS_SORT_MULTI,
		kSortTristate = DMUI_UI_TABLE_FLAGS_SORT_TRISTATE,
		kHighlightHoveredColumn = DMUI_UI_TABLE_FLAGS_HIGHLIGHT_HOVERED_COLUMN,
		kBordersH = DMUI_UI_TABLE_FLAGS_BORDERS_H,
		kBordersV = DMUI_UI_TABLE_FLAGS_BORDERS_V,
		kBordersInner = DMUI_UI_TABLE_FLAGS_BORDERS_INNER,
		kBordersOuter = DMUI_UI_TABLE_FLAGS_BORDERS_OUTER,
		kBorders = DMUI_UI_TABLE_FLAGS_BORDERS
	};

	[[nodiscard]] constexpr TableFlags operator|(
		TableFlags a_left, TableFlags a_right) noexcept
	{
		return static_cast<TableFlags>(
			static_cast<uint32_t>(a_left) |
			static_cast<uint32_t>(a_right));
	}

	[[nodiscard]] constexpr TableFlags operator&(
		TableFlags a_left, TableFlags a_right) noexcept
	{
		return static_cast<TableFlags>(
			static_cast<uint32_t>(a_left) &
			static_cast<uint32_t>(a_right));
	}

	constexpr TableFlags& operator|=(
		TableFlags& a_left, TableFlags a_right) noexcept
	{
		a_left = a_left | a_right;
		return a_left;
	}

	enum class TableColumnFlags : uint32_t
	{
		kNone = 0u,
		kDisabled = DMUI_UI_TABLE_COLUMN_FLAGS_DISABLED,
		kDefaultHide = DMUI_UI_TABLE_COLUMN_FLAGS_DEFAULT_HIDE,
		kDefaultSort = DMUI_UI_TABLE_COLUMN_FLAGS_DEFAULT_SORT,
		kWidthStretch = DMUI_UI_TABLE_COLUMN_FLAGS_WIDTH_STRETCH,
		kWidthFixed = DMUI_UI_TABLE_COLUMN_FLAGS_WIDTH_FIXED,
		kNoResize = DMUI_UI_TABLE_COLUMN_FLAGS_NO_RESIZE,
		kNoReorder = DMUI_UI_TABLE_COLUMN_FLAGS_NO_REORDER,
		kNoHide = DMUI_UI_TABLE_COLUMN_FLAGS_NO_HIDE,
		kNoClip = DMUI_UI_TABLE_COLUMN_FLAGS_NO_CLIP,
		kNoSort = DMUI_UI_TABLE_COLUMN_FLAGS_NO_SORT,
		kNoSortAscending = DMUI_UI_TABLE_COLUMN_FLAGS_NO_SORT_ASCENDING,
		kNoSortDescending = DMUI_UI_TABLE_COLUMN_FLAGS_NO_SORT_DESCENDING,
		kNoHeaderLabel = DMUI_UI_TABLE_COLUMN_FLAGS_NO_HEADER_LABEL,
		kNoHeaderWidth = DMUI_UI_TABLE_COLUMN_FLAGS_NO_HEADER_WIDTH,
		kPreferSortAscending = DMUI_UI_TABLE_COLUMN_FLAGS_PREFER_SORT_ASCENDING,
		kPreferSortDescending = DMUI_UI_TABLE_COLUMN_FLAGS_PREFER_SORT_DESCENDING,
		kIndentEnable = DMUI_UI_TABLE_COLUMN_FLAGS_INDENT_ENABLE,
		kIndentDisable = DMUI_UI_TABLE_COLUMN_FLAGS_INDENT_DISABLE,
		kAngledHeader = DMUI_UI_TABLE_COLUMN_FLAGS_ANGLED_HEADER
	};

	[[nodiscard]] constexpr TableColumnFlags operator|(
		TableColumnFlags a_left, TableColumnFlags a_right) noexcept
	{
		return static_cast<TableColumnFlags>(
			static_cast<uint32_t>(a_left) |
			static_cast<uint32_t>(a_right));
	}

	[[nodiscard]] constexpr TableColumnFlags operator&(
		TableColumnFlags a_left, TableColumnFlags a_right) noexcept
	{
		return static_cast<TableColumnFlags>(
			static_cast<uint32_t>(a_left) &
			static_cast<uint32_t>(a_right));
	}

	constexpr TableColumnFlags& operator|=(
		TableColumnFlags& a_left, TableColumnFlags a_right) noexcept
	{
		a_left = a_left | a_right;
		return a_left;
	}

	enum class TableRowFlags : uint32_t
	{
		kNone = 0u,
		kHeaders = DMUI_UI_TABLE_ROW_FLAGS_HEADERS
	};

	[[nodiscard]] constexpr TableRowFlags operator|(
		TableRowFlags a_left, TableRowFlags a_right) noexcept
	{
		return static_cast<TableRowFlags>(
			static_cast<uint32_t>(a_left) |
			static_cast<uint32_t>(a_right));
	}

	[[nodiscard]] constexpr TableRowFlags operator&(
		TableRowFlags a_left, TableRowFlags a_right) noexcept
	{
		return static_cast<TableRowFlags>(
			static_cast<uint32_t>(a_left) &
			static_cast<uint32_t>(a_right));
	}

	constexpr TableRowFlags& operator|=(
		TableRowFlags& a_left, TableRowFlags a_right) noexcept
	{
		a_left = a_left | a_right;
		return a_left;
	}

	enum class TreeNodeFlags : uint32_t
	{
		kNone = 0u,
		kSelected = DMUI_UI_TREE_NODE_FLAGS_SELECTED,
		kFramed = DMUI_UI_TREE_NODE_FLAGS_FRAMED,
		kAllowOverlap = DMUI_UI_TREE_NODE_FLAGS_ALLOW_OVERLAP,
		kNoTreePushOnOpen = DMUI_UI_TREE_NODE_FLAGS_NO_TREE_PUSH_ON_OPEN,
		kNoAutoOpenOnLog = DMUI_UI_TREE_NODE_FLAGS_NO_AUTO_OPEN_ON_LOG,
		kDefaultOpen = DMUI_UI_TREE_NODE_FLAGS_DEFAULT_OPEN,
		kOpenOnDoubleClick = DMUI_UI_TREE_NODE_FLAGS_OPEN_ON_DOUBLE_CLICK,
		kOpenOnArrow = DMUI_UI_TREE_NODE_FLAGS_OPEN_ON_ARROW,
		kLeaf = DMUI_UI_TREE_NODE_FLAGS_LEAF,
		kBullet = DMUI_UI_TREE_NODE_FLAGS_BULLET,
		kFramePadding = DMUI_UI_TREE_NODE_FLAGS_FRAME_PADDING,
		kSpanAvailWidth = DMUI_UI_TREE_NODE_FLAGS_SPAN_AVAIL_WIDTH,
		kSpanFullWidth = DMUI_UI_TREE_NODE_FLAGS_SPAN_FULL_WIDTH,
		kSpanLabelWidth = DMUI_UI_TREE_NODE_FLAGS_SPAN_LABEL_WIDTH,
		kSpanAllColumns = DMUI_UI_TREE_NODE_FLAGS_SPAN_ALL_COLUMNS,
		kLabelSpanAllColumns = DMUI_UI_TREE_NODE_FLAGS_LABEL_SPAN_ALL_COLUMNS,
		kNavLeftJumpsToParent = DMUI_UI_TREE_NODE_FLAGS_NAV_LEFT_JUMPS_TO_PARENT,
		kDrawLinesNone = DMUI_UI_TREE_NODE_FLAGS_DRAW_LINES_NONE,
		kDrawLinesFull = DMUI_UI_TREE_NODE_FLAGS_DRAW_LINES_FULL,
		kDrawLinesToNodes = DMUI_UI_TREE_NODE_FLAGS_DRAW_LINES_TO_NODES,
		kCollapsingHeader = DMUI_UI_TREE_NODE_FLAGS_COLLAPSING_HEADER
	};

	[[nodiscard]] constexpr TreeNodeFlags operator|(
		TreeNodeFlags a_left, TreeNodeFlags a_right) noexcept
	{
		return static_cast<TreeNodeFlags>(
			static_cast<uint32_t>(a_left) |
			static_cast<uint32_t>(a_right));
	}

	[[nodiscard]] constexpr TreeNodeFlags operator&(
		TreeNodeFlags a_left, TreeNodeFlags a_right) noexcept
	{
		return static_cast<TreeNodeFlags>(
			static_cast<uint32_t>(a_left) &
			static_cast<uint32_t>(a_right));
	}

	constexpr TreeNodeFlags& operator|=(
		TreeNodeFlags& a_left, TreeNodeFlags a_right) noexcept
	{
		a_left = a_left | a_right;
		return a_left;
	}

	namespace detail
	{
		[[nodiscard]] inline bool HasOperationsThroughSize(
			const DMUI_UIAPI* a_api,
			uint32_t a_minimumSize) noexcept
		{
			return a_api &&
				a_minimumSize >= DMUI_UI_API_REQUIRED_SIZE &&
				a_minimumSize <= DMUI_UI_API_CURRENT_SIZE &&
				a_api->structSize >= a_minimumSize &&
				a_api->getStyleMetrics &&
				a_api->beginCombo &&
				a_api->endCombo &&
				a_api->beginDisabled &&
				a_api->endDisabled &&
				a_api->beginTable &&
				a_api->endTable &&
				a_api->beginTooltip &&
				a_api->endTooltip &&
				a_api->button &&
				a_api->calcTextSize &&
				a_api->checkbox &&
				a_api->collapsingHeader &&
				a_api->collapsingHeaderVisible &&
				a_api->dragScalar &&
				a_api->dummy &&
				a_api->getContentRegionAvail &&
				a_api->getCursorScreenPos &&
				a_api->getFontSize &&
				a_api->getFrameHeight &&
				a_api->getStyleColor &&
				a_api->getTextLineHeightWithSpacing &&
				a_api->indent &&
				a_api->inputScalar &&
				a_api->inputText &&
				a_api->inputTextMultiline &&
				a_api->inputTextWithHint &&
				a_api->isItemDeactivatedAfterEdit &&
				a_api->isItemHovered &&
				a_api->popID &&
				a_api->popStyleColor &&
				a_api->popTextWrapPos &&
				a_api->progressBar &&
				a_api->pushIDString &&
				a_api->pushIDRange &&
				a_api->pushIDValue &&
				a_api->pushStyleColorU32 &&
				a_api->pushStyleColor &&
				a_api->pushTextWrapPos &&
				a_api->sameLine &&
				a_api->selectable &&
				a_api->selectableToggle &&
				a_api->separator &&
				a_api->setClipboardText &&
				a_api->setCursorScreenPos &&
				a_api->setItemDefaultFocus &&
				a_api->setNextItemWidth &&
				a_api->setTooltipText &&
				a_api->sliderScalar &&
				a_api->spacing &&
				a_api->tableHeadersRow &&
				a_api->tableNextColumn &&
				a_api->tableNextRow &&
				a_api->tableSetColumnIndex &&
				a_api->tableSetupColumn &&
				a_api->tableSetupScrollFreeze &&
				a_api->text &&
				a_api->textColored &&
				a_api->textDisabled &&
				a_api->textWrapped &&
				a_api->unindent &&
				a_api->newLine &&
				(a_minimumSize < DMUI_UI_API_PLOT_LINES_SIZE || a_api->plotLines);
		}
	}

	namespace checked
	{
		[[nodiscard]] inline DMUI_Result GetStyleMetrics(
			DMUI_StyleMetrics* metrics) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_GET_STYLE_METRICS_SIZE,
				&DMUI_UIAPI::getStyleMetrics, metrics);
		}

		[[nodiscard]] inline DMUI_Result BeginCombo(
			const char* label,
		const char* previewValue,
		DMUI_UIComboFlags flags,
		uint32_t* visible) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_BEGIN_COMBO_SIZE,
				&DMUI_UIAPI::beginCombo, label, previewValue, flags, visible);
		}

		[[nodiscard]] inline DMUI_Result EndCombo(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_END_COMBO_SIZE,
				&DMUI_UIAPI::endCombo);
		}

		[[nodiscard]] inline DMUI_Result BeginDisabled(
			uint32_t disabled) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_BEGIN_DISABLED_SIZE,
				&DMUI_UIAPI::beginDisabled, disabled);
		}

		[[nodiscard]] inline DMUI_Result EndDisabled(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_END_DISABLED_SIZE,
				&DMUI_UIAPI::endDisabled);
		}

		[[nodiscard]] inline DMUI_Result BeginTable(
			const char* id,
		int32_t columns,
		DMUI_UITableFlags flags,
		DMUI_Vec2 outerSize,
		float innerWidth,
		uint32_t* visible) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_BEGIN_TABLE_SIZE,
				&DMUI_UIAPI::beginTable, id, columns, flags, outerSize, innerWidth, visible);
		}

		[[nodiscard]] inline DMUI_Result EndTable(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_END_TABLE_SIZE,
				&DMUI_UIAPI::endTable);
		}

		[[nodiscard]] inline DMUI_Result BeginTooltip(
			uint32_t* visible) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_BEGIN_TOOLTIP_SIZE,
				&DMUI_UIAPI::beginTooltip, visible);
		}

		[[nodiscard]] inline DMUI_Result EndTooltip(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_END_TOOLTIP_SIZE,
				&DMUI_UIAPI::endTooltip);
		}

		[[nodiscard]] inline DMUI_Result Button(
			const char* label,
		DMUI_Vec2 size,
		uint32_t* pressed) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_BUTTON_SIZE,
				&DMUI_UIAPI::button, label, size, pressed);
		}

		[[nodiscard]] inline DMUI_Result CalcTextSize(
			const char* text,
		size_t textLength,
		uint32_t hideTextAfterDoubleHash,
		float wrapWidth,
		DMUI_Vec2* size) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_CALC_TEXT_SIZE_SIZE,
				&DMUI_UIAPI::calcTextSize, text, textLength, hideTextAfterDoubleHash, wrapWidth, size);
		}

		[[nodiscard]] inline DMUI_Result Checkbox(
			const char* label,
		uint32_t* value,
		uint32_t* changed) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_CHECKBOX_SIZE,
				&DMUI_UIAPI::checkbox, label, value, changed);
		}

		[[nodiscard]] inline DMUI_Result CollapsingHeader(
			const char* label,
		DMUI_UITreeNodeFlags flags,
		uint32_t* open) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_COLLAPSING_HEADER_SIZE,
				&DMUI_UIAPI::collapsingHeader, label, flags, open);
		}

		[[nodiscard]] inline DMUI_Result CollapsingHeaderVisible(
			const char* label,
		uint32_t* visible,
		DMUI_UITreeNodeFlags flags,
		uint32_t* open) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_COLLAPSING_HEADER_VISIBLE_SIZE,
				&DMUI_UIAPI::collapsingHeaderVisible, label, visible, flags, open);
		}

		[[nodiscard]] inline DMUI_Result DragScalar(
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
		uint32_t* changed) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_DRAG_SCALAR_SIZE,
				&DMUI_UIAPI::dragScalar, label, dataType, data, dataSize, speed, minimum, minimumSize, maximum, maximumSize, format, flags, changed);
		}

		[[nodiscard]] inline DMUI_Result Dummy(
			DMUI_Vec2 size) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_DUMMY_SIZE,
				&DMUI_UIAPI::dummy, size);
		}

		[[nodiscard]] inline DMUI_Result GetContentRegionAvail(
			DMUI_Vec2* size) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_GET_CONTENT_REGION_AVAIL_SIZE,
				&DMUI_UIAPI::getContentRegionAvail, size);
		}

		[[nodiscard]] inline DMUI_Result GetCursorScreenPos(
			DMUI_Vec2* position) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_GET_CURSOR_SCREEN_POS_SIZE,
				&DMUI_UIAPI::getCursorScreenPos, position);
		}

		[[nodiscard]] inline DMUI_Result GetFontSize(
			float* size) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_GET_FONT_SIZE_SIZE,
				&DMUI_UIAPI::getFontSize, size);
		}

		[[nodiscard]] inline DMUI_Result GetFrameHeight(
			float* height) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_GET_FRAME_HEIGHT_SIZE,
				&DMUI_UIAPI::getFrameHeight, height);
		}

		[[nodiscard]] inline DMUI_Result GetStyleColor(
			DMUI_UIColor color,
		DMUI_Vec4* value) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_GET_STYLE_COLOR_SIZE,
				&DMUI_UIAPI::getStyleColor, color, value);
		}

		[[nodiscard]] inline DMUI_Result GetTextLineHeightWithSpacing(
			float* height) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_GET_TEXT_LINE_HEIGHT_WITH_SPACING_SIZE,
				&DMUI_UIAPI::getTextLineHeightWithSpacing, height);
		}

		[[nodiscard]] inline DMUI_Result Indent(
			float width) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_INDENT_SIZE,
				&DMUI_UIAPI::indent, width);
		}

		[[nodiscard]] inline DMUI_Result InputScalar(
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
		uint32_t* changed) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_INPUT_SCALAR_SIZE,
				&DMUI_UIAPI::inputScalar, label, dataType, data, dataSize, step, stepSize, fastStep, fastStepSize, format, flags, changed);
		}

		[[nodiscard]] inline DMUI_Result InputText(
			const char* label,
		char* buffer,
		uint32_t capacity,
		DMUI_UIInputTextFlags flags,
		uint32_t* changed) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_INPUT_TEXT_SIZE,
				&DMUI_UIAPI::inputText, label, buffer, capacity, flags, changed);
		}

		[[nodiscard]] inline DMUI_Result InputTextMultiline(
			const char* label,
		char* buffer,
		uint32_t capacity,
		DMUI_Vec2 size,
		DMUI_UIInputTextFlags flags,
		uint32_t* changed) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_INPUT_TEXT_MULTILINE_SIZE,
				&DMUI_UIAPI::inputTextMultiline, label, buffer, capacity, size, flags, changed);
		}

		[[nodiscard]] inline DMUI_Result InputTextWithHint(
			const char* label,
		const char* hint,
		char* buffer,
		uint32_t capacity,
		DMUI_UIInputTextFlags flags,
		uint32_t* changed) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_INPUT_TEXT_WITH_HINT_SIZE,
				&DMUI_UIAPI::inputTextWithHint, label, hint, buffer, capacity, flags, changed);
		}

		[[nodiscard]] inline DMUI_Result IsItemDeactivatedAfterEdit(
			uint32_t* deactivated) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_IS_ITEM_DEACTIVATED_AFTER_EDIT_SIZE,
				&DMUI_UIAPI::isItemDeactivatedAfterEdit, deactivated);
		}

		[[nodiscard]] inline DMUI_Result IsItemHovered(
			DMUI_UIHoveredFlags flags,
		uint32_t* hovered) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_IS_ITEM_HOVERED_SIZE,
				&DMUI_UIAPI::isItemHovered, flags, hovered);
		}

		[[nodiscard]] inline DMUI_Result PopID(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_POP_ID_SIZE,
				&DMUI_UIAPI::popID);
		}

		[[nodiscard]] inline DMUI_Result PopStyleColor(
			int32_t count) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_POP_STYLE_COLOR_SIZE,
				&DMUI_UIAPI::popStyleColor, count);
		}

		[[nodiscard]] inline DMUI_Result PopTextWrapPos(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_POP_TEXT_WRAP_POS_SIZE,
				&DMUI_UIAPI::popTextWrapPos);
		}

		[[nodiscard]] inline DMUI_Result ProgressBar(
			float fraction,
		DMUI_Vec2 size,
		const char* overlay,
		size_t overlayLength) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_PROGRESS_BAR_SIZE,
				&DMUI_UIAPI::progressBar, fraction, size, overlay, overlayLength);
		}

		[[nodiscard]] inline DMUI_Result PushIDString(
			const char* id) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_PUSH_ID_STRING_SIZE,
				&DMUI_UIAPI::pushIDString, id);
		}

		[[nodiscard]] inline DMUI_Result PushIDRange(
			const char* id,
		size_t length) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_PUSH_ID_RANGE_SIZE,
				&DMUI_UIAPI::pushIDRange, id, length);
		}

		[[nodiscard]] inline DMUI_Result PushIDValue(
			uint64_t id) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_PUSH_ID_VALUE_SIZE,
				&DMUI_UIAPI::pushIDValue, id);
		}

		[[nodiscard]] inline DMUI_Result PushStyleColorU32(
			DMUI_UIColor color,
		uint32_t rgba) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_PUSH_STYLE_COLOR_U32_SIZE,
				&DMUI_UIAPI::pushStyleColorU32, color, rgba);
		}

		[[nodiscard]] inline DMUI_Result PushStyleColor(
			DMUI_UIColor color,
		DMUI_Vec4 value) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_PUSH_STYLE_COLOR_SIZE,
				&DMUI_UIAPI::pushStyleColor, color, value);
		}

		[[nodiscard]] inline DMUI_Result PushTextWrapPos(
			float localX) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_PUSH_TEXT_WRAP_POS_SIZE,
				&DMUI_UIAPI::pushTextWrapPos, localX);
		}

		[[nodiscard]] inline DMUI_Result SameLine(
			float offsetFromStartX,
		float spacing) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SAME_LINE_SIZE,
				&DMUI_UIAPI::sameLine, offsetFromStartX, spacing);
		}

		[[nodiscard]] inline DMUI_Result Selectable(
			const char* label,
		uint32_t selected,
		DMUI_UISelectableFlags flags,
		DMUI_Vec2 size,
		uint32_t* pressed) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SELECTABLE_SIZE,
				&DMUI_UIAPI::selectable, label, selected, flags, size, pressed);
		}

		[[nodiscard]] inline DMUI_Result SelectableToggle(
			const char* label,
		uint32_t* selected,
		DMUI_UISelectableFlags flags,
		DMUI_Vec2 size,
		uint32_t* pressed) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SELECTABLE_TOGGLE_SIZE,
				&DMUI_UIAPI::selectableToggle, label, selected, flags, size, pressed);
		}

		[[nodiscard]] inline DMUI_Result Separator(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SEPARATOR_SIZE,
				&DMUI_UIAPI::separator);
		}

		[[nodiscard]] inline DMUI_Result SetClipboardText(
			const char* text,
		size_t textLength) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SET_CLIPBOARD_TEXT_SIZE,
				&DMUI_UIAPI::setClipboardText, text, textLength);
		}

		[[nodiscard]] inline DMUI_Result SetCursorScreenPos(
			DMUI_Vec2 position) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SET_CURSOR_SCREEN_POS_SIZE,
				&DMUI_UIAPI::setCursorScreenPos, position);
		}

		[[nodiscard]] inline DMUI_Result SetItemDefaultFocus(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SET_ITEM_DEFAULT_FOCUS_SIZE,
				&DMUI_UIAPI::setItemDefaultFocus);
		}

		[[nodiscard]] inline DMUI_Result SetNextItemWidth(
			float width) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SET_NEXT_ITEM_WIDTH_SIZE,
				&DMUI_UIAPI::setNextItemWidth, width);
		}

		[[nodiscard]] inline DMUI_Result SetTooltipText(
			const char* text,
		size_t textLength) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SET_TOOLTIP_TEXT_SIZE,
				&DMUI_UIAPI::setTooltipText, text, textLength);
		}

		[[nodiscard]] inline DMUI_Result SliderScalar(
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
		uint32_t* changed) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SLIDER_SCALAR_SIZE,
				&DMUI_UIAPI::sliderScalar, label, dataType, data, dataSize, minimum, minimumSize, maximum, maximumSize, format, flags, changed);
		}

		[[nodiscard]] inline DMUI_Result Spacing(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_SPACING_SIZE,
				&DMUI_UIAPI::spacing);
		}

		[[nodiscard]] inline DMUI_Result TableHeadersRow(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_TABLE_HEADERS_ROW_SIZE,
				&DMUI_UIAPI::tableHeadersRow);
		}

		[[nodiscard]] inline DMUI_Result TableNextColumn(
			uint32_t* visible) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_TABLE_NEXT_COLUMN_SIZE,
				&DMUI_UIAPI::tableNextColumn, visible);
		}

		[[nodiscard]] inline DMUI_Result TableNextRow(
			DMUI_UITableRowFlags flags,
		float minimumHeight) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_TABLE_NEXT_ROW_SIZE,
				&DMUI_UIAPI::tableNextRow, flags, minimumHeight);
		}

		[[nodiscard]] inline DMUI_Result TableSetColumnIndex(
			int32_t column,
		uint32_t* visible) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_TABLE_SET_COLUMN_INDEX_SIZE,
				&DMUI_UIAPI::tableSetColumnIndex, column, visible);
		}

		[[nodiscard]] inline DMUI_Result TableSetupColumn(
			const char* label,
		DMUI_UITableColumnFlags flags,
		float initialWidthOrWeight,
		uint32_t userId) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_TABLE_SETUP_COLUMN_SIZE,
				&DMUI_UIAPI::tableSetupColumn, label, flags, initialWidthOrWeight, userId);
		}

		[[nodiscard]] inline DMUI_Result TableSetupScrollFreeze(
			int32_t columns,
		int32_t rows) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_TABLE_SETUP_SCROLL_FREEZE_SIZE,
				&DMUI_UIAPI::tableSetupScrollFreeze, columns, rows);
		}

		[[nodiscard]] inline DMUI_Result Text(
			const char* text,
		size_t textLength) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_TEXT_SIZE,
				&DMUI_UIAPI::text, text, textLength);
		}

		[[nodiscard]] inline DMUI_Result TextColored(
			DMUI_Vec4 color,
		const char* text,
		size_t textLength) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_TEXT_COLORED_SIZE,
				&DMUI_UIAPI::textColored, color, text, textLength);
		}

		[[nodiscard]] inline DMUI_Result TextDisabled(
			const char* text,
		size_t textLength) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_TEXT_DISABLED_SIZE,
				&DMUI_UIAPI::textDisabled, text, textLength);
		}

		[[nodiscard]] inline DMUI_Result TextWrapped(
			const char* text,
		size_t textLength) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_TEXT_WRAPPED_SIZE,
				&DMUI_UIAPI::textWrapped, text, textLength);
		}

		[[nodiscard]] inline DMUI_Result Unindent(
			float width) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_UNINDENT_SIZE,
				&DMUI_UIAPI::unindent, width);
		}

		[[nodiscard]] inline DMUI_Result NewLine(
			void) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_NEW_LINE_SIZE,
				&DMUI_UIAPI::newLine);
		}

		[[nodiscard]] inline DMUI_Result PlotLines(
			const char* label,
		const float* values,
		int32_t valueCount,
		int32_t valueOffset,
		const char* overlay,
		size_t overlayLength,
		float scaleMinimum,
		float scaleMaximum,
		DMUI_Vec2 size,
		uint32_t strideBytes) noexcept
		{
			return detail::Invoke(
				DMUI_UI_API_PLOT_LINES_SIZE,
				&DMUI_UIAPI::plotLines, label, values, valueCount, valueOffset, overlay, overlayLength, scaleMinimum, scaleMaximum, size, strideBytes);
		}
	}
}
