#include "system.h"
#include "window/window.h"
#include "render/renderer.h"


ENGINE_NAMESPACE_BEGIN

std::unique_ptr<System> g_system_context = std::make_unique<System>();

System::~System()
{
}

void System::initialize()
{
	g_window_system = std::make_shared<Window>();
	g_window_system->initialize();
	g_render_system = std::make_shared<Renderer>();
	g_render_system->initialize();
}

void System::run()
{
	GlobalRuntimeInfo global_info;
	while (!global_info.window_quit)
	{
		g_window_system->present(global_info);
		g_render_system->render(global_info);


		global_info.window_resized = false;
	}
}

ENGINE_NAMESPACE_END