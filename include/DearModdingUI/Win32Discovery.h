#pragma once

#include <cstdint>

struct HINSTANCE__;

namespace dmui::detail
{
#if defined(_WIN32) && defined(_MSC_VER)
	using Win32Module = HINSTANCE__*;
	using Win32Procedure = intptr_t (__stdcall*)();

	extern "C" __declspec(dllimport) Win32Module __stdcall GetModuleHandleW(
		const wchar_t* moduleName);
	extern "C" __declspec(dllimport) Win32Procedure __stdcall GetProcAddress(
		Win32Module module,
		const char* symbol);

	[[nodiscard]] inline Win32Module HostModule() noexcept
	{
		static const auto module = GetModuleHandleW(L"DearModdingUI.dll");
		return module;
	}

	template <class Function>
	[[nodiscard]] Function ResolveHostSymbol(const char* symbol) noexcept
	{
		const auto module = HostModule();
		return module ?
			reinterpret_cast<Function>(GetProcAddress(module, symbol)) :
			nullptr;
	}
#else
	template <class Function>
	[[nodiscard]] Function ResolveHostSymbol(const char*) noexcept
	{
		return nullptr;
	}
#endif
}
