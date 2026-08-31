#include <DearModdingUI/VisualDecisions.h>

#if defined(IMGUI_VERSION)
#error VisualDecisions must not depend on Dear ImGui
#endif

static_assert(DearModdingUI::Theme::kFontRoleDefaults.size() == 5);
static_assert(
	DearModdingUI::ResolvePageActionRowLayout(0.0f, 100.0f, 30.0f, 3, 5.0f)
		.actionsMinX == 60.0f);

void CompileVisualDecisions()
{
	(void)DearModdingUI::Theme::ResolveRoleFontSize(
		DearModdingUI::Theme::FontRole::kBody,
		1080);
}
