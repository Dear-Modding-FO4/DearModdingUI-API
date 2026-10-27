#include <DearModdingUI/Client.h>

#if defined(ERROR)
#error "Client.h leaked the Windows ERROR macro"
#endif

#if defined(MAX_PATH)
#error "Client.h leaked the Windows MAX_PATH macro"
#endif

#include <Windows.h>

static_assert(std::is_same_v<
	decltype(dmui::ui::Text(
		std::string_view{},
		std::span<const dmui::TextSpan>{},
		dmui::TextOptions{})),
	void>);

// A literal must not collide with the printf-style overload.
static_assert(std::is_same_v<
	decltype(dmui::ui::Text("literal", dmui::TextStyle{})),
	void>);

void CompileNoWindowsMacros()
{}
