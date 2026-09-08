#include <DearModdingUI/Client.h>

#if defined(ERROR)
#error "Client.h leaked the Windows ERROR macro"
#endif

#if defined(MAX_PATH)
#error "Client.h leaked the Windows MAX_PATH macro"
#endif

#include <Windows.h>

static_assert(std::is_same_v<
	decltype(dmui::DrawStyledText(
		std::declval<dmui::Client&>(),
		std::string_view{},
		dmui::TextStyle{})),
	bool>);

void CompileNoWindowsMacros()
{}
