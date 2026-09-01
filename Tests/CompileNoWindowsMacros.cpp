#include <DearModdingUI/Client.h>

#if defined(ERROR)
#error "Client.h leaked the Windows ERROR macro"
#endif

#if defined(MAX_PATH)
#error "Client.h leaked the Windows MAX_PATH macro"
#endif

#include <Windows.h>

void CompileNoWindowsMacros()
{}
