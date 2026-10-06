#include <DearModdingUI/Client.h>

namespace
{
	constexpr DMUI_HostAPI api{ DMUI_ABI_MAJOR };

	constexpr bool NegotiatesOlderMinor()
	{
		uint32_t minor{};
		uint32_t requests{};
		const auto* result = dmui::detail::NegotiateAPI(
			[&](uint32_t version) {
				++requests;
				return version == DMUI_MAKE_ABI_VERSION(DMUI_ABI_MAJOR, 0u) ?
					&api : nullptr;
			},
			minor);
		return result == &api && minor == 0u && requests == DMUI_ABI_MINOR + 1u;
	}

	static_assert(NegotiatesOlderMinor());
	static_assert(dmui::detail::SlotAbiMinor<&DMUI_HostAPI::requestOverlayFocus> == 1u);
	static_assert(dmui::detail::SlotAbiMinor<&DMUI_UIAPI::inputTextEditor> == 1u);
	static_assert(dmui::detail::SlotAbiMinor<&DMUI_UIAPI::beginTooltipAt> == 1u);
	static_assert(dmui::detail::SlotAbiMinor<&DMUI_UIAPI::text> == 0u);
}
