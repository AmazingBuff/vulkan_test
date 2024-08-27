#pragma once

#include "render_types.h"
#include "rhi/interface.h"

RENDERING_NAMESPACE_BEGIN

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;
	void initialize();

public:
	std::shared_ptr<Drawable> m_drawable;
};


RENDERING_NAMESPACE_END