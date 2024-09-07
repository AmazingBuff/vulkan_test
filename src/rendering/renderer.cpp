#include "renderer.h"
#include "render_resouces.h"
#include "drawable.h"

ENGINE_NAMESPACE_BEGIN

void Renderer::initialize()
{
	m_render_resources = std::make_shared<RenderResources>();
	m_render_resources->initialize();
	m_drawable = std::make_shared<Drawable>();
	m_drawable->initialize();
}



ENGINE_NAMESPACE_END


