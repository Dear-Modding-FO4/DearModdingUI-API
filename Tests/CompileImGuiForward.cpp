#include <DearModdingUI/ImGuiForward.h>

#include <type_traits>

static_assert(sizeof(ImVec2_c) == sizeof(float) * 2);
static_assert(sizeof(ImVec4_c) == sizeof(float) * 4);
static_assert(std::is_same_v<ImVec2, ImVec2_c>);
static_assert(std::is_same_v<ImVec4, ImVec4_c>);

void CompileImGuiForward()
{
	bool selected{};
	const float values[]{ 1.0f, 2.0f };

	(void) ImGui::BeginCombo("combo", "preview");
	ImGui::EndCombo();
	(void) ImGui::Button("button");
	(void) ImGui::CollapsingHeader("header");
	(void) ImGui::CollapsingHeader("header", &selected);
	ImGui::PlotLines("plot", values, 2);
	ImGui::PushID("id");
	ImGui::PushID(1);
	ImGui::PopID();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	ImGui::PopStyleColor();
	(void) ImGui::Selectable("selectable", &selected);
	ImGui::Text("value: %d", 1);
	ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f }, "text");
	ImGui::SetTooltip("tooltip");
}
