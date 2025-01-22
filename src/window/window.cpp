#include "window.h"
#include "utils/util.h"
#include "render/rhi/rhi.h"
#include "system/system.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

ENGINE_NAMESPACE_BEGIN

Window::~Window()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Window::initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
		WINDOW_LOG_ERROR(SDL_GetError());

    Uint32 flags = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;
    switch (Hardware_API)
    {
	case GraphicsAPI::e_opengl:
		flags |= SDL_WINDOW_OPENGL;
		break;
	case GraphicsAPI::e_vulkan:
		flags |= SDL_WINDOW_VULKAN;
		break;
	case GraphicsAPI::e_metal:
		flags |= SDL_WINDOW_METAL;
		break;
	default:
		flags |= SDL_WINDOW_OPENGL;
		break;
    }

	m_window = SDL_CreateWindow(Window_Title, Window_Width, Window_Height, flags);
    if (!m_window || !SDL_SetWindowRelativeMouseMode(m_window, true)) // for mouse postion capture
        WINDOW_LOG_ERROR(SDL_GetError());
}

void Window::present(GlobalRuntimeInfo& global_info) const
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			global_info.window_quit = true;
			break;
		case SDL_EVENT_WINDOW_RESIZED:
			global_info.window_resized = true;
			break;
		case SDL_EVENT_WINDOW_MINIMIZED:
			global_info.window_minimized = true;
			break;
		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP:
		{
			global_info.events.emplace_back(event);
		}
			break;
		default:
		{
			global_info.events.emplace_back(event);
		}
			break;
		}
	}
}

void Window::window_size(int* width, int* height) const
{
	SDL_GetWindowSize(m_window, width, height);
}

ENGINE_NAMESPACE_END