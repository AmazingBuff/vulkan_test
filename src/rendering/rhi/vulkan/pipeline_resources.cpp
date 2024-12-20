#include "pipeline_resouces.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"
#include "rendering/render_resouces.h"
#include "rendering/resources/shader/shader_manager.h"
#include "rendering/resources/render_pass/render_pass_manager.h"
#include "rendering/rhi/vulkan/trans/enum_trans.h"
#include "rendering/rhi/vulkan/trans/structure_trans.h"


ENGINE_NAMESPACE_BEGIN

void VK_CLASS(PipelineResources)::initialize()
{
}

constexpr RHIFlag VK_CLASS(PipelineResources)::flag() const
{
	return RHIFlag::e_pipeline_resources;
}

std::shared_ptr<VK_CLASS(Pipeline)> VK_CLASS(PipelineResources)::get_pipeline(const std::string& name) const
{
	return m_pipelines.at(name);
}

void VK_CLASS(PipelineResources)::create_render_pass(const std::string_view& name)
{
	std::shared_ptr<VK_CLASS(RenderPass)> render_pass = std::make_shared<VK_CLASS(RenderPass)>();
	render_pass->init(name);
	m_render_passes.emplace(name, render_pass);
}

void VK_CLASS(PipelineResources)::create_pipeline_layout(const std::string& name)
{

}

void VK_CLASS(PipelineResources)::create_pipelines(const std::string& name)
{

}

void VK_CLASS(PipelineResources)::create_vertex_buffers(const std::string& name)
{

}


ENGINE_NAMESPACE_END