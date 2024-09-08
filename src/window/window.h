#pragma once

#include "base/macro.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "rendering/rhi/interface.h"

ENGINE_NAMESPACE_BEGIN

static const char* Window_Title = "Vulkan Window";
static constexpr int Window_Width = 960;
static constexpr int Window_Height = 540;

struct WindowEvents
{
	bool framebuffer_resized = false;
	bool quit = false;
};


class Window final
{
public:
	Window() = default;
	~Window();
	void initialize();
	// return false if the window is closed
	void present(WindowEvents& events) const;
private:
	SDL_Window* m_window = nullptr;

	friend class RHI_USING_CLASS(Instance);
	friend class RHI_USING_CLASS(SwapChain);
};


ENGINE_NAMESPACE_END