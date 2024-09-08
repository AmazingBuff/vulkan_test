#pragma once

#include "base/macro.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "rendering/rhi/interface.h"
#include "system/structure.h"

ENGINE_NAMESPACE_BEGIN

static const char* Window_Title = "Vulkan Window";
static constexpr int Window_Width = 960;
static constexpr int Window_Height = 540;

class Window final
{
public:
	Window() = default;
	~Window();
	void initialize();
	// return false if the window is closed
	void present(GlobalRuntimeInfo& global_info) const;
private:
	SDL_Window* m_window = nullptr;

	friend class RHI_USING_CLASS(Instance);
	friend class RHI_USING_CLASS(SwapChain);
};


ENGINE_NAMESPACE_END