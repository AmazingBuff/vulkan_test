#include "renderer.h"
#include "render_resouces.h"
#include "drawable.h"

AMAZING_NAMESPACE_BEGIN


void Renderer::initialize()
{
	m_drawable = std::make_shared<RENDERING_NAMESPACE::Drawable>();
	m_drawable->initialize();
}



AMAZING_NAMESPACE_END


