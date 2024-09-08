#pragma once

#include "render_types.h"

ENGINE_NAMESPACE_BEGIN

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;
	void initialize();
	void render(const RenderInfos& info) const;
public:
	std::shared_ptr<Drawable>			m_drawable;
	std::shared_ptr<RenderResources>	m_render_resources;
};


ENGINE_NAMESPACE_END