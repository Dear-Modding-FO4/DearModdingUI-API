#include <DearModdingUI/Client.h>
#include <DearModdingUI/UI.h>

#include <cstdint>
#include <type_traits>

static_assert(std::is_standard_layout_v<dmui::ui::Vec2>);
static_assert(std::is_standard_layout_v<dmui::ui::Vec4>);
static_assert(sizeof(dmui::ui::Vec2) == sizeof(float) * 2);
static_assert(sizeof(dmui::ui::Vec4) == sizeof(float) * 4);
static_assert(
	static_cast<uint32_t>(dmui::ui::Color::kText) ==
		DMUI_UI_COLOR_TEXT);
static_assert(DMUI_UI_COLOR_TEXT != 0u);

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
			.minimumUIAPISize = DMUI_UI_API_REQUIRED_SIZE
		}
	};

	bool selected{};
	char multiline[128]{};
	const float values[]{ 1.0f, 2.0f };

	(void)client.HostPresent();
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
