#include "system.h"
#include "window/window.h"
#include "rendering/renderer.h"


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
	WindowEvents events;
	RenderInfos render_infos;
	while (!events.quit)
	{
		g_window_system->present(events);
		render_infos.framebuffer_resized = events.framebuffer_resized;
		g_render_system->render(render_infos);


		events.framebuffer_resized = false;
	}
}

ENGINE_NAMESPACE_END