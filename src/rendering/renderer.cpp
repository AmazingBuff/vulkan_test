#include "renderer.h"
#include "render_resouces.h"
#include "drawable.h"

AMAZING_NAMESPACE_BEGIN

void Renderer::initialize()
{
	m_drawable = std::make_shared<Drawable>();
	m_drawable->initialize();
	m_render_resources = std::make_shared<RenderResources>();
	m_render_resources->initialize();
}



AMAZING_NAMESPACE_END


