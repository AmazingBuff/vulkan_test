#include "system.h"
#include "window/window.h"
#include "rendering/renderer.h"


AMAZING_NAMESPACE_BEGIN

std::unique_ptr<System> g_system_context = std::make_unique<System>();

void System::initialize()
{
	g_window_system = std::make_shared<Window>();
	g_window_system->initialize();
	g_render_system = std::make_shared<Renderer>();
	g_render_system->initialize();
}

AMAZING_NAMESPACE_END