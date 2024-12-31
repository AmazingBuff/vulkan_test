#include "render_resouces.h"
#include "resources/shader/shader_manager.h"
#include "resources/pipeline_layout/pipeline_layout_manager.h"
#include "resources/render_pass/render_pass_manager.h"
#include "resources/pipeline/pipeline_manager.h"

ENGINE_NAMESPACE_BEGIN

void RenderResources::initialize()
{
	m_shader_manager = std::make_shared<ShaderManager>();
	m_shader_manager->initialize();
	m_pipeline_layout_manager = std::make_shared<PipelineLayoutManager>();
	m_pipeline_layout_manager->initialize();
	m_render_pass_manager = std::make_shared<RenderPassManager>();
    m_render_pass_manager->initialize();
	m_pipeline_manager = std::make_shared<PipelineManager>();
	m_pipeline_manager->initialize();
}

NODISCARD const ShaderResource& RenderResources::get_shader_resource(const std::string_view& name) const
{
	return m_shader_manager->get_shader_resource(name);
}

NODISCARD const PipelineLayoutResource& RenderResources::get_pipeline_layout_resource(const std::string_view& name) const
{
	return m_pipeline_layout_manager->get_pipeline_layout_resource(name);
}

NODISCARD const RenderPassResource& RenderResources::get_render_pass_resource(const std::string_view& name) const
{
    return m_render_pass_manager->get_render_pass_resource(name);
}

NODISCARD const PipelineResource& RenderResources::get_pipeline_resource(const std::string_view& name) const
{
	return m_pipeline_manager->get_pipeline_resource(name);
}


ENGINE_NAMESPACE_END