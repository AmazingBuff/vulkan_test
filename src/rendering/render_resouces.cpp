#include "render_resouces.h"


RENDERING_NAMESPACE_BEGIN

void RenderResources::initialize()
{

}

ShaderResource& RenderResources::get_shader_resource(const std::string& name)
{
	return m_shader_resources[name];
}


RENDERING_NAMESPACE_END