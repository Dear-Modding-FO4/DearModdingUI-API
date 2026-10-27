#pragma once

#include <DearModdingUI/API.h>

#include <cstdint>
#include <string>

namespace dmui
{
	enum class KeyCaptureSlot : uint32_t
	{
		kKeyboardMouse = DMUI_KEY_CAPTURE_SLOT_KEYBOARD_MOUSE,
		kGamepad = DMUI_KEY_CAPTURE_SLOT_GAMEPAD
	};

	// Non-owning; the DMUI_KeyCaptureDescriptor comment defines each field.
	struct KeyCaptureOptions
	{
		const char* id{};
		// A chord such as "Ctrl+F5"; empty or "none" is unbound.
		const char* binding{};
		const char* displayText{};
		const char* details{};
		const char* warning{};
		KeyCaptureSlot slot{ KeyCaptureSlot::kKeyboardMouse };
		bool allowClear{ true };
		bool readOnly{};
		float width{};

		[[nodiscard]] constexpr DMUI_KeyCaptureDescriptor Abi() const noexcept
		{
			return {
				id,
				binding,
				displayText,
				details,
				warning,
				static_cast<DMUI_KeyCaptureSlot>(slot),
				(allowClear ? DMUI_KEY_CAPTURE_ALLOW_CLEAR : 0u) |
					(readOnly ? DMUI_KEY_CAPTURE_READ_ONLY : 0u),
				width,
				0u
			};
		}
	};

	struct KeyCaptureResult
	{
		bool changed{};
		// The new binding when changed; "none" when it was cleared.
		std::string chord;
	};
}
