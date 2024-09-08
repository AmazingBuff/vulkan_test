#pragma once

#include "render_types.h"
#include "system/structure.h"

ENGINE_NAMESPACE_BEGIN

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;
	void initialize();
	void render(GlobalRuntimeInfo& global_info) const;
public:
	std::shared_ptr<Drawable>			m_drawable;
	std::shared_ptr<RenderResources>	m_render_resources;
};


ENGINE_NAMESPACE_END