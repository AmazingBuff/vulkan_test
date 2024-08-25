#pragma once

#include "base/macro.h"

#include <glm/glm.hpp>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#define WINDOW_NAMESPACE_BEGIN				AMAZING_NAMESPACE_BEGIN(Window)
#define WINDOW_NAMESPACE_END				AMAZING_NAMESPACE_END(Window)
#define WINDOW_NAMESPACE					AMAZING_NAMESPACE_CONCAT(Window)
#define WINDOW_NAMESPACE_VARIABLE(var)		NAMESPACE_VARIABLE(WINDOW_NAMESPACE, var)

WINDOW_NAMESPACE_BEGIN

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


WINDOW_NAMESPACE_END