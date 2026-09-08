#define ImGui DMUICompileImGuiForward
#define dmui DMUICompileForwardingClient
#include <DearModdingUI/Client.h>
#undef dmui

#include <type_traits>

static_assert(sizeof(ImVec2_c) == sizeof(float) * 2);
static_assert(sizeof(ImVec4_c) == sizeof(float) * 4);
static_assert(std::is_same_v<ImVec2, ImVec2_c>);
static_assert(std::is_same_v<ImVec4, ImVec4_c>);
static_assert(ImGui::kForwardImGuiVersionNum == DMUI_IMGUI_VERSION_NUM);

void CompileImGuiForward()
{
	static DMUICompileForwardingClient::Client client{
		"example.forward.mod",
		"Forward Example",
		DMUICompileForwardingClient::Version{ 1, 0 },
		DMUICompileForwardingClient::kForwardingClient
	};
	bool selected{};
	char multiline[128]{};
	const float values[]{ 1.0f, 2.0f };
	DMUI_StyleMetrics metrics{};

	(void)client.HostPresent();
	(void)ImGui::GetFont();
	(void)ImGui::GetStyleMetrics(metrics);
	(void)ImGui::IsForwardVersionCompatible();

	(void) ImGui::BeginCombo("combo", "preview");
	ImGui::EndCombo();
	(void) ImGui::Button("button");
	(void) ImGui::InputTextMultiline(
		"comments",
		multiline,
		sizeof(multiline),
		{ 320.0f, ImGui::GetTextLineHeightWithSpacing() * 3.0f });
	(void) ImGui::IsItemDeactivatedAfterEdit();
	(void) ImGui::CollapsingHeader("header");
	(void) ImGui::CollapsingHeader("header", &selected);
	ImGui::PlotLines("plot", values, 2);
	ImGui::PushID("id");
	ImGui::PushID(1);
	ImGui::PopID();
	ImGui::PushFont(ImGui::GetFont(), metrics.fontSizeBase);
	ImGui::PopFont();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	ImGui::PopStyleColor();
	ImGui::PushTextWrapPos(0.0f);
	ImGui::TextUnformatted("wrapped");
	ImGui::PopTextWrapPos();
	(void) ImGui::Selectable("selectable", &selected);
	ImGui::Text("value: %d", 1);
	ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f }, "text");
	ImGui::SetTooltip("tooltip");
}
