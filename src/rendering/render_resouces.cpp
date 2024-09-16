#include "render_resouces.h"
#include "resources/shader/shader_manager.h"
#include "resources/render_pass/render_pass_manager.h"
#include "rendering/rhi/vulkan/trans/structure_trans.h"


ENGINE_NAMESPACE_BEGIN

void RenderResources::initialize()
{
	m_shader_manager = std::make_shared<ShaderManager>();
	m_shader_manager->initialize();
	m_render_pass_manager = std::make_shared<RenderPassManager>();
    m_render_pass_manager->initialize();
}

NODISCARD const ShaderResource& RenderResources::get_shader_resource(const std::string& name)
{
	return m_shader_manager->get_shader_resource(name);
}

NODISCARD const RenderPassResource& RenderResources::get_render_pass_resource(const std::string& name)
{
    return m_render_pass_manager->get_render_pass_resource(name);
}


ENGINE_NAMESPACE_END