#include "pipeline_resouces.h"

ENGINE_NAMESPACE_BEGIN

void VK_CLASS(PipelineResources)::initialize()
{
	create_pipeline_layout();
	create_render_pass();
	create_vertex_buffers();
	create_pipelines();
}

constexpr RHIFlag VK_CLASS(PipelineResources)::flag() const
{
	return RHIFlag::e_pipeline_resources;
}

std::shared_ptr<VK_CLASS(Pipeline)> VK_CLASS(PipelineResources)::get_pipeline(const std::string& name) const
{
	return m_pipelines.at(name);
}

void VK_CLASS(PipelineResources)::create_render_pass()
{

}


void VK_CLASS(PipelineResources)::create_pipeline_layout()
{

}

void VK_CLASS(PipelineResources)::create_pipelines()
{

}

void VK_CLASS(PipelineResources)::create_vertex_buffers()
{

}


ENGINE_NAMESPACE_END