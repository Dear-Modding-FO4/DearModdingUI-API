#include <DearModdingUI/Client.h>

#include <cstring>
#include <limits>
#include <string>
#include <string_view>

namespace
{
	static_assert(std::is_same_v<std::invoke_result_t<DMUI_ActionCallback, void*>, DMUI_Result>);
	bool TestTextInputBuffer()
	{
		dmui::TextInputBuffer growable{ "seed" };
		auto* growableBuffer = growable.Get();
		if (growable.Result() != DMUI_RESULT_OK ||
			!growableBuffer ||
			!growableBuffer->resize ||
			growableBuffer->capacity < 64)
			return false;

		auto* data = growableBuffer->data;
		auto capacity = growableBuffer->capacity;
		if (growableBuffer->resize(
				growableBuffer->userData,
				128,
				&data,
				&capacity) != DMUI_RESULT_OK ||
			capacity < 128 ||
			std::string_view{ data, 4 } != "seed")
			return false;
		std::memcpy(data, "grown", 6);
		std::string committed{ "original" };
		if (growable.CommitTo(committed) != DMUI_RESULT_OK ||
			committed != "grown")
			return false;

		dmui::TextInputBuffer fixed{ "query", 512 };
		const auto* fixedBuffer = fixed.Get();
		if (fixed.Result() != DMUI_RESULT_OK ||
			!fixedBuffer ||
			fixedBuffer->capacity != 513 ||
			fixedBuffer->resize)
			return false;

		constexpr char embeddedNul[]{ 'a', '\0', 'b' };
		dmui::TextInputBuffer invalid{
			std::string_view{ embeddedNul, sizeof(embeddedNul) }
		};
		if (invalid.Result() != DMUI_RESULT_INVALID_ARGUMENT)
			return false;

		std::string original{ "stable" };
		dmui::TextInputBuffer failedGrowth{ original };
		auto* failedBuffer = failedGrowth.Get();
		auto* previousData = failedBuffer->data;
		auto previousCapacity = failedBuffer->capacity;
		auto* callbackData = previousData;
		auto callbackCapacity = previousCapacity;
		const auto tooLarge =
			static_cast<size_t>((std::numeric_limits<int>::max)()) + 1;
		return failedBuffer->resize(
				   failedBuffer->userData,
				   tooLarge,
				   &callbackData,
				   &callbackCapacity) ==
				DMUI_RESULT_INVALID_ARGUMENT &&
			callbackData == previousData &&
			callbackCapacity == previousCapacity &&
			std::string_view{ previousData, 6 } == "stable" &&
			failedGrowth.Result() == DMUI_RESULT_INVALID_ARGUMENT &&
			failedGrowth.CommitTo(original) ==
				DMUI_RESULT_INVALID_ARGUMENT &&
			original == "stable";
	}
}

int main()
{
	const DMUI_ImageDescriptor descriptor{
		1,
		1,
		DMUI_PIXEL_FORMAT_RGBA8_UNORM,
		0,
		4,
		4,
		nullptr
	};
	DMUI_HostAPI api{};
	api.createImage = nullptr;
	api.loadImageFile = nullptr;
	api.resetOverlay = nullptr;
	api.updateImage = nullptr;
	api.resolveIconGlyph = nullptr;
	api.beginSettingsRow = nullptr;
	api.endSettingsRow = nullptr;
	api.beginSettingsRowEx = nullptr;
	api.beginField = nullptr;
	api.setFieldFeedback = nullptr;
	api.endField = nullptr;
	api.drawTextView = nullptr;
	api.drawSearchInputBuffer = nullptr;
	const DMUI_IconResolutionRequest iconRequest{
		"wrench",
		"Graphics Settings",
		"General"
	};
	const DMUI_CategoryDescriptor category{
		"lighting",
		"Lighting",
		0,
		0,
		"sun-horizon"
	};
	const DMUI_PageDescriptor page{
		"settings",
		"Settings",
		"lighting",
		nullptr,
		0,
		DMUI_PAGE_KIND_SETTINGS,
		nullptr,
		nullptr,
		"sliders-horizontal"
	};
	const size_t lineOffsets[]{ 0 };
	const DMUI_TextViewDescriptor textView{
		"preview",
		"",
		0,
		lineOffsets,
		1,
		nullptr,
		0,
		0,
		1,
		1,
		{ 0.0f, 320.0f }
	};
	const DMUI_TextViewState textState{
		1,
		1,
		DMUI_TEXT_VIEW_NO_OFFSET,
		DMUI_TEXT_VIEW_NO_OFFSET
	};
	const DMUI_TextBuffer textBuffer{
		nullptr,
		1,
		nullptr,
		nullptr
	};
	dmui::Client client{ "compile.test", "Compile test", { 1, 0 } };
	dmui::DialogSession dialog;
	const DMUI_DialogDescriptor dialogDescriptor{
		DMUI_DIALOG_KIND_CONFIRM, "Confirm", nullptr, "OK", "Cancel"
	};
	(void)client.ResetOverlay(1);
	(void)dialog.Open(client, dialogDescriptor,
		[](std::string_view) -> std::optional<std::string> { return std::nullopt; });
	dialog.Poll();
	dialog.Cancel();
	static_assert(!std::is_copy_constructible_v<dmui::DialogSession>);
	return TestTextInputBuffer() && !dialog.Active() &&
		dialog.LastResult() == DMUI_RESULT_CLIENT_NOT_FOUND ? 0 : 1;
}
