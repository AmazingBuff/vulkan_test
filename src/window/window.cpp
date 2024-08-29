#include "window.h"
#include "utils/util.h"
#include "rendering/rhi/rhi.h"

AMAZING_NAMESPACE_BEGIN

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
    switch (HARDWARE_API)
    {
	case RENDERING_NAMESPACE::GraphicsAPI::e_opengl:
		flags = SDL_WINDOW_OPENGL;
		break;
	case RENDERING_NAMESPACE::GraphicsAPI::e_vulkan:
		flags = SDL_WINDOW_VULKAN;
		break;
	case RENDERING_NAMESPACE::GraphicsAPI::e_metal:
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

NODISCARD SDL_Window* Window::get_window() const
{
	return m_window;
}

AMAZING_NAMESPACE_END