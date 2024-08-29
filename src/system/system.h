#pragma once

#include "base/macro.h"
#include "base/template.h"

AMAZING_NAMESPACE_BEGIN

class Window;
class Renderer;

class System final
{
public:
	System() = default;
	~System() = default;
	void initialize();
public:
	std::shared_ptr<Window>		g_window_system;
	std::shared_ptr<Renderer>	g_render_system;
};

extern std::unique_ptr<System> g_system_context;

AMAZING_NAMESPACE_END