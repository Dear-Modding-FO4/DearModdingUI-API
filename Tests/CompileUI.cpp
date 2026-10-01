#include <DearModdingUI/Client.h>
#include <DearModdingUI/UI.h>

#include <array>
#include <cstdint>
#include <string>
#include <type_traits>

static_assert(std::is_standard_layout_v<dmui::ui::Vec2>);
static_assert(std::is_standard_layout_v<dmui::ui::Vec4>);
static_assert(sizeof(dmui::ui::Vec2) == sizeof(float) * 2);
static_assert(sizeof(dmui::ui::Vec4) == sizeof(float) * 4);
static_assert(
	static_cast<uint32_t>(dmui::ui::Color::kText) ==
		DMUI_UI_COLOR_TEXT);
static_assert(DMUI_UI_COLOR_TEXT != 0u);
constexpr dmui::ClientOptions kPositionalClientOptions{
	DMUI_CLIENT_CAPABILITY_NONE,
};

[[nodiscard]] dmui::ChoiceSettingControl CompileUnmatchedChoiceLabel()
{
	return {
		.options = { { "", "None" }, { "preset.xml", "Preset" } },
		.unmatchedLabel = "None"
	};
}

void CompileStableUI()
{
	static dmui::Client client{
		"example.stable-ui",
		"Stable UI Example",
		dmui::Version{ 1, 0 },
		{},
		{},
		{
			.capabilities = DMUI_CLIENT_CAPABILITY_RENDERER_REPLACEMENT
		}
	};

	bool selected{};
	char multiline[128]{};
	const float values[]{ 1.0f, 2.0f };
	std::string search;
	const std::array<size_t, 1> lineOffsets{ 0 };
	const dmui::TextViewRequest textRequest{
		.text = "Preview",
		.lineOffsets = lineOffsets,
		.contentRevision = 1,
		.matchRevision = 1
	};
	dmui::TextViewState textState;

	(void)client.HostPresent();
	(void)client.DrawSearchInput("search", "Search", search);
	(void)client.DrawSearchInput("search", "Search", search, 255);
	(void)client.DrawTextView("preview", textRequest, textState);
	(void)dmui::ui::BeginCombo("combo", "preview");
	dmui::ui::EndCombo();
	(void)dmui::ui::Button("button");
	(void)dmui::ui::InputTextMultiline(
		"comments",
		multiline,
		sizeof(multiline),
		{ 320.0f, dmui::ui::GetTextLineHeightWithSpacing() * 3.0f });
	(void)dmui::ui::IsItemDeactivatedAfterEdit();
	(void)dmui::ui::CollapsingHeader("header");
	(void)dmui::ui::CollapsingHeader("header", &selected);
	dmui::ui::PlotLines("plot", values, 2);
	dmui::ui::PushID("id");
	dmui::ui::PushID(1);
	dmui::ui::PopID();
	dmui::ui::PushStyleColor(
		dmui::ui::Color::kText,
		dmui::ui::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	dmui::ui::PopStyleColor();
	dmui::ui::PushStyleVar(dmui::ui::StyleVar::kAlpha, 0.5f);
	dmui::ui::PushStyleVar(
		dmui::ui::StyleVar::kFramePadding,
		dmui::ui::Vec2{ 4.0f, 2.0f });
	dmui::ui::PopStyleVar(2);
	dmui::ui::ListClipper clipper;
	clipper.Begin(1000);
	while (clipper.Step())
		for (int32_t i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
			dmui::ui::Text("%d: row", i);
	clipper.End();
	dmui::ui::PushTextWrapPos(0.0f);
	dmui::ui::TextUnformatted("wrapped");
	dmui::ui::PopTextWrapPos();
	(void)dmui::ui::Selectable("selectable", &selected);
	dmui::ui::Text("value: %d", 1);
	dmui::ui::TextColored(
		dmui::ui::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
		"text");
	dmui::ui::SetTooltip("tooltip");
	dmui::ui::SetCursorPos(dmui::ui::GetCursorPos());
	dmui::ui::SetCursorPosX(dmui::ui::GetCursorPosX());
	dmui::ui::SetCursorPosY(dmui::ui::GetCursorPosY());
	dmui::ui::TextAligned(0.5f, 200.0f, "Aligned");
	if (dmui::ui::BeginItemTooltip())
		dmui::ui::EndTooltip();
	(void)dmui::ui::Image(dmui::ImageHandle{}, { 20.0f, 20.0f });
	dmui::ui::PlotAnnotated("annotated", {});
	dmui::ui::OpenPopup("Popup");
	if (dmui::ui::PopupScope popup{ "Popup" })
		dmui::ui::CloseCurrentPopup();
	bool open{ true };
	if (dmui::ui::ModalScope modal{ "Modal", open, false })
		dmui::ui::CloseCurrentPopup();
	(void)dmui::ui::IsPopupOpen("Modal");
}
