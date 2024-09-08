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

void Renderer::render(const RenderInfos& info) const
{
	RHIRenderInfos rhi_info;
	rhi_info.framebuffer_resized = info.framebuffer_resized;
	m_drawable->draw(rhi_info);
}

ENGINE_NAMESPACE_END


