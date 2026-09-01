#include <DearModdingUI/VisualDecisions.h>

#if defined(IMGUI_VERSION)
#error VisualDecisions must not depend on Dear ImGui
#endif

static_assert(DearModdingUI::Theme::kFontRoleDefaults.size() == 5);
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

void CompileVisualDecisions()
{
	(void)DearModdingUI::Theme::ResolveRoleFontSize(
		DearModdingUI::Theme::FontRole::kBody,
		1080);
}
