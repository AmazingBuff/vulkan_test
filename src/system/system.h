#pragma once

#include "base/macro.h"
#include "base/template.h"

ENGINE_NAMESPACE_BEGIN

class Window;
class Renderer;

class System final
{
public:
	System() = default;
	~System();
	void initialize();
	void run();
public:
	std::shared_ptr<Window>		g_window_system;
	std::shared_ptr<Renderer>	g_render_system;
};

extern std::unique_ptr<System> g_system_context;

ENGINE_NAMESPACE_END