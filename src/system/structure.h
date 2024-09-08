#pragma once

#include "base/macro.h"

ENGINE_NAMESPACE_BEGIN

#ifdef HIGH_PRECISION_FLOAT
using Float = double;
#else
using Float = float;
#endif // HIGH_PRECISION



struct GlobalRuntimeInfo
{
	// window
	bool window_quit = false;
	bool window_resized = false;
	bool window_minimized = false;
	// rendering
};


ENGINE_NAMESPACE_END