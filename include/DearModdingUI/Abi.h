#pragma once

#include <DearModdingUI/API.h>

namespace dmui::detail
{
	template <auto Slot>
	inline constexpr uint32_t SlotAbiMinor{ 0u };

	template <>
	inline constexpr uint32_t SlotAbiMinor<&DMUI_HostAPI::requestOverlayFocus>{ 1u };
	template <>
	inline constexpr uint32_t SlotAbiMinor<&DMUI_HostAPI::releaseOverlayFocus>{ 1u };
	template <>
	inline constexpr uint32_t SlotAbiMinor<&DMUI_HostAPI::queryOverlayFocus>{ 1u };

	template <class GetAPI>
	[[nodiscard]] constexpr const DMUI_HostAPI* NegotiateAPI(
		GetAPI a_getAPI, uint32_t& a_minor) noexcept
	{
		a_minor = DMUI_ABI_MINOR;
		for (;;)
		{
			if (const auto* api = a_getAPI(DMUI_MAKE_ABI_VERSION(DMUI_ABI_MAJOR, a_minor)))
				return api;
			if (a_minor == 0u)
				return nullptr;
			--a_minor;
		}
	}
}
