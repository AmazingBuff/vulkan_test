#pragma once

#include "render_types.h"

RENDERING_NAMESPACE_BEGIN
class Drawable;
class RenderResources;
RENDERING_NAMESPACE_END

AMAZING_NAMESPACE_BEGIN


class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;
	void initialize();

public:
	std::shared_ptr<RENDERING_NAMESPACE::Drawable>			m_drawable;
	std::shared_ptr<RENDERING_NAMESPACE::RenderResources>	m_render_resources;
};


AMAZING_NAMESPACE_END