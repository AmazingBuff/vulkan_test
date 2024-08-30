#include "render_resouces.h"


RENDERING_NAMESPACE_BEGIN

void RenderResources::initialize()
{
	m_shader_manager = std::make_unique<ShaderManager>();
	m_shader_manager->initialize();
}

NODISCARD const ShaderResource& RenderResources::get_shader_resource(const std::string& name)
{
	return m_shader_manager->get_shader_resource(name);
}


RENDERING_NAMESPACE_END