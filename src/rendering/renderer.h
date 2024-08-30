#pragma once

#include "render_info.h"

AMAZING_NAMESPACE_BEGIN

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;
	void initialize();

public:
	std::shared_ptr<Drawable>			m_drawable;
	std::shared_ptr<RenderResources>	m_render_resources;
};


AMAZING_NAMESPACE_END