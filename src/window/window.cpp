#include "window.h"
#include "utils/util.h"
#include "render/rhi/rhi.h"
#include "system/system.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

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

    Uint32 flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
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

    if ((m_window = SDL_CreateWindow(Window_Title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, Window_Width, Window_Height, flags)) == nullptr)
        WINDOW_LOG_ERROR(SDL_GetError());

	// for mouse postion capture
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::present(GlobalRuntimeInfo& global_info) const
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			global_info.window_quit = true;
			break;
		case SDL_WINDOWEVENT:
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				global_info.window_resized = true;
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				global_info.window_minimized = true;
				break;
			default:
				break;
			}
		}
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
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