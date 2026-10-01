#include <DearModdingUI/API.h>

#include <cstddef>
#include <cstdint>

// The single ABI layout guard for DMUI_HostAPI; the host compiles this file too.
#if UINTPTR_MAX == UINT64_MAX
static_assert(DMUI_ABI_VERSION == 2);
static_assert(offsetof(DMUI_HostAPI, abiVersion) == 0);
static_assert(offsetof(DMUI_HostAPI, ui) == 8);
static_assert(offsetof(DMUI_HostAPI, registerClient) == 16);
static_assert(sizeof(DMUI_HostAPI) == 464);
static_assert(offsetof(DMUI_HostAPI, drawSearchInputBuffer) == 448);
static_assert(offsetof(DMUI_HostAPI, loadImageFile) == 456);
#endif
