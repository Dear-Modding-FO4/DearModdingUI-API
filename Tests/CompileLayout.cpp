#include <DearModdingUI/Presentation/Layout.h>

#if defined(IMGUI_VERSION)
#error Layout must not depend on Dear ImGui
#endif

static_assert(
	DearModdingUI::DecideInlineIconLayout(
		true,
		20.0f,
		10.0f,
		16.0f,
		4.0f).contentWidth == 40.0f);
static_assert(
	DearModdingUI::ResolveCenteredGlyphOrigin(
		10.0f,
		10.0f,
		0.0f,
		0.0f,
		4.0f,
		4.0f,
		1.0f) ==
		DearModdingUI::GlyphDrawOrigin{ 8.0f, 8.0f });
static_assert(DearModdingUI::BulletRunContentInset(4.0f, 10.0f) == 7.0f);
static_assert(
	DearModdingUI::ResolvePageActionRowLayout(0.0f, 100.0f, 30.0f, 3, 5.0f)
		.actionsMinX == 60.0f);
static_assert(
	DearModdingUI::RowContentOffsetY(
		40.0f,
		{ 20.0f },
		DearModdingUI::RowContentMetric::kBox) == 10.0f);
static_assert(
	DearModdingUI::ResolveRowContentRect(
		DearModdingUI::RowContentRectKind::kSelectable,
		{ 0.0f, 0.0f, 100.0f, 40.0f },
		5.0f) ==
		DearModdingUI::RowContentRect{ 0.0f, 0.0f, 100.0f, 40.0f });
static_assert(
	DearModdingUI::ResolveRowContentRect(
		DearModdingUI::RowContentRectKind::kTable,
		{ 0.0f, 0.0f, 100.0f, 40.0f },
		5.0f) ==
		DearModdingUI::RowContentRect{ 0.0f, 5.0f, 100.0f, 35.0f });
static_assert(
	DearModdingUI::ResolveRowContentLayout(
		0.0f,
		100.0f,
		5.0f,
		20.0f,
		4.0f,
		6.0f,
		true,
		true,
		10.0f).textMinX == 55.0f);
static_assert(
	DearModdingUI::ResolveTrailingControlLayout(
		0.0f,
		100.0f,
		20.0f,
		5.0f).adjacentMaxX == 75.0f);
static_assert(
	DearModdingUI::ActionButtonWidth(false, 20.0f, 16.0f, 4.0f) == 28.0f);
static_assert(
	DearModdingUI::ResolveSettingsActionButtonWidthSum(
		std::array{ 10.0f, -2.0f, 5.0f },
		true,
		1) == 15.0f);
