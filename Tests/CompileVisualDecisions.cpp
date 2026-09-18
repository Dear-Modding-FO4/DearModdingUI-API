#include <DearModdingUI/VisualDecisions.h>

#if defined(IMGUI_VERSION)
#error VisualDecisions must not depend on Dear ImGui
#endif

static_assert(DearModdingUI::Theme::kFontRoleDefaults.size() == 6);
static_assert(
	DearModdingUI::ShouldDrawHeaderClose(false, true));

void CompileVisualDecisions()
{
	(void)DearModdingUI::ResolvePageActionRowLayout(
		0.0f,
		100.0f,
		30.0f,
		3,
		5.0f);
	(void)DearModdingUI::Theme::ResolveRoleFontSize(
		DearModdingUI::Theme::FontRole::kBody,
		1080);
}
