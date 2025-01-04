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

void Renderer::render(GlobalRuntimeInfo& global_info) const
{
	m_drawable->draw(global_info);
}

ENGINE_NAMESPACE_END


