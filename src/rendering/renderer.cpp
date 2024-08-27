#include "renderer.h"

RENDERING_NAMESPACE_BEGIN


void Renderer::initialize()
{
	m_drawable = std::make_shared<Drawable>();
	m_drawable->initialize();
}



RENDERING_NAMESPACE_END


