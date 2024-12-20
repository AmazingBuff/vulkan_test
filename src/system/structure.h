#pragma once

#include "base/macro.h"

ENGINE_NAMESPACE_BEGIN

struct GlobalRuntimeInfo
{
	// window
	bool window_quit = false;
	bool window_resized = false;
	bool window_minimized = false;
	// rendering
};


ENGINE_NAMESPACE_END