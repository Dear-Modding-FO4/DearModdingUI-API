#define dmui DMUICompileForwardingNoHost
#include <DearModdingUI/Client.h>
#undef dmui

#include <string>

// This translation unit intentionally includes no Dear ImGui headers or sources.
// A missing host leaves non-UI configuration and shader work available.
[[nodiscard]] bool CompileForwardingNoHostExample()
{
	static DMUICompileForwardingNoHost::Client client{
		"example.forwarding.only",
		"Forwarding-only example",
		DMUICompileForwardingNoHost::Version{ 1, 0 },
		DMUICompileForwardingNoHost::kForwardingClient,
		{},
		{},
		{
			.capabilities = DMUI_CLIENT_CAPABILITY_RENDERER_REPLACEMENT,
			.requiredServices =
				DMUI_HOST_SERVICE_FRAME_CONTROL |
				DMUI_HOST_SERVICE_IMAGE_RESOURCES,
			.minimumForwardingVersion =
				DMUI_FORWARDING_VERSION_CURRENT
		}
	};

	const auto connected = client.Connect();
	const auto nonUiShaderConfigured = true;
	if (!connected && !client.HostPresent())
		return nonUiShaderConfigured &&
			client.LastResult() == DMUI_RESULT_OK;
	return nonUiShaderConfigured;
}
