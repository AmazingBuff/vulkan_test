#pragma once

#include "base/macro.h"
#include <SDL2/SDL_events.h>

ENGINE_NAMESPACE_BEGIN

struct GlobalRuntimeInfo
{
	// time
	std::chrono::steady_clock::time_point start_point{};
	std::chrono::steady_clock::time_point current_point{};
	Float previous_time = 0.0;
	Float current_time = 0.0;

	// events
	std::vector<SDL_Event> events;

	// window
	bool window_quit = false;
	bool window_resized = false;
	bool window_minimized = false;

	// rendering
	bool render_stop = false;
};


ENGINE_NAMESPACE_END