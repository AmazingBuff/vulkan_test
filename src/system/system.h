#pragma once

#include "base/macro.h"
#include "base/template.h"

#define SYSTEM_NAMESPACE_BEGIN				AMAZING_NAMESPACE_BEGIN
#define SYSTEM_NAMESPACE_END				AMAZING_NAMESPACE_END
#define SYSTEM_NAMESPACE					AMAZING_NAMESPACE
#define SYSTEM_NAMESPACE_VARIABLE(var)		NAMESPACE_VARIABLE(SYSTEM_NAMESPACE, var)

SYSTEM_NAMESPACE_BEGIN

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

SYSTEM_NAMESPACE_END