#include "system.h"
#include "window/window.h"
#include "camera/camera.h"
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
	g_camera_system = std::make_shared<Camera>();
	g_camera_system->initialize();
}

void System::run()
{
	GlobalRuntimeInfo global_info, stored_info;
	global_info.start_point = std::chrono::high_resolution_clock::now();
	stored_info.start_point = global_info.start_point;
	stored_info.current_point = stored_info.start_point;
	while (!global_info.window_quit)
	{
		if (!stored_info.render_stop)
		{
			global_info.current_point = std::chrono::high_resolution_clock::now();
			global_info.current_time = std::chrono::duration<Float>(global_info.current_point - global_info.start_point).count();
		}


		g_window_system->present(global_info);
		g_camera_system->update(global_info);
		g_render_system->render(global_info);


		global_info.window_resized = false;
		global_info.events.clear();

		if (!stored_info.render_stop)
			global_info.previous_time = global_info.current_time;

		if (global_info.render_stop)
		{
			if (stored_info.render_stop)
			{
				global_info.current_point = std::chrono::high_resolution_clock::now();
				auto diff = global_info.current_point - stored_info.current_point;
				global_info.start_point += diff;
				stored_info.start_point += diff;

				global_info.render_stop = false;
			}

			std::swap(global_info, stored_info);

			global_info.current_time = stored_info.current_time;
			global_info.previous_time = stored_info.previous_time;
			global_info.render_stop = false;
		}
	}
}

ENGINE_NAMESPACE_END