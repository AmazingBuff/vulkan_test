#include "window.h"
#include "utils/util.h"
#include "rendering/rhi/rhi.h"
#include "system/system.h"
#include "rendering/renderer.h"

ENGINE_NAMESPACE_BEGIN

Window::~Window()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Window::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
		WINDOW_LOG_ERROR(SDL_GetError());

    SDL_WindowFlags flags;
    switch (Hardware_API)
    {
	case GraphicsAPI::e_opengl:
		flags = SDL_WINDOW_OPENGL;
		break;
	case GraphicsAPI::e_vulkan:
		flags = SDL_WINDOW_VULKAN;
		break;
	case GraphicsAPI::e_metal:
		flags = SDL_WINDOW_METAL;
		break;
	default:
		flags = SDL_WINDOW_OPENGL;
		break;
    }

    if ((m_window = SDL_CreateWindow(Window_Title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, Window_Width, Window_Height, flags)) == nullptr)
        WINDOW_LOG_ERROR(SDL_GetError());
}

void Window::present() const
{
	bool is_running = true;
	while(is_running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				is_running = false;
				break;
			default:
				break;
			}
		}
		g_system_context->g_render_system->render();
	}
}

NODISCARD SDL_Window* Window::get_window() const
{
	return m_window;
}

ENGINE_NAMESPACE_END