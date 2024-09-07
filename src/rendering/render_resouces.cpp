#include "render_resouces.h"
#include "resources/shader/shader_manager.h"


ENGINE_NAMESPACE_BEGIN

void RenderResources::initialize()
{
	m_shader_manager = std::make_shared<ShaderManager>();
	m_shader_manager->initialize();
}

NODISCARD const ShaderResource& RenderResources::get_shader_resource(const std::string& name)
{
	return m_shader_manager->get_shader_resource(name);
}


ENGINE_NAMESPACE_END