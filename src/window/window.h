#pragma once

#include "base/macro.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

ENGINE_NAMESPACE_BEGIN

static const char* Window_Title = "Vulkan Window";
static constexpr int Window_Width = 1280;
static constexpr int Window_Height = 720;

class Window final
{
public:
	Window() = default;
	~Window();
	void initialize();
	NODISCARD SDL_Window* get_window() const;
private:
	SDL_Window* m_window = nullptr;
};


ENGINE_NAMESPACE_END