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
	DMUI_HOST_SERVICE_NONE,
	DMUI_UI_REVISION_1,
	DMUI_UI_API_REQUIRED_SIZE
};
static_assert(
	kPositionalClientOptions.minimumHostAPISize ==
	DMUI_HOST_API_REGISTER_CLIENT_SIZE);

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
			.capabilities = DMUI_CLIENT_CAPABILITY_RENDERER_REPLACEMENT,
			.requiredServices =
				DMUI_HOST_SERVICE_FRAME_CONTROL |
				DMUI_HOST_SERVICE_IMAGE_RESOURCES,
			.minimumUIRevision = DMUI_UI_REVISION_1,
			.minimumUIAPISize = DMUI_UI_API_REQUIRED_SIZE,
			.minimumHostAPISize = DMUI_HOST_API_REGISTER_CLIENT_SIZE
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
	dmui::ui::PushTextWrapPos(0.0f);
	dmui::ui::TextUnformatted("wrapped");
	dmui::ui::PopTextWrapPos();
	(void)dmui::ui::Selectable("selectable", &selected);
	dmui::ui::Text("value: %d", 1);
	dmui::ui::TextColored(
		dmui::ui::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
		"text");
	dmui::ui::SetTooltip("tooltip");
}
