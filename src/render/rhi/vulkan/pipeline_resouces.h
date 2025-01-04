#pragma once

#include "types.h"
#include "pipeline.h"

ENGINE_NAMESPACE_BEGIN


class VK_CLASS(PipelineResources) : public RHI
{
public:
	VK_CLASS(PipelineResources)() = default;
	~VK_CLASS(PipelineResources)() override = default;
	NODISCARD constexpr RHIFlag flag() const override;

	void initialize();
	NODISCARD std::shared_ptr<VK_CLASS(Pipeline)> get_pipeline(const std::string & name) const;
private:
	void create_render_pass(const std::string_view& name);
	void create_pipeline_layout(const std::string & name);
	void create_pipelines(const std::string & name);
	void create_vertex_buffers(const std::string & name);
private:
	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(Pipeline)>>		m_pipelines;

	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(PipelineLayout)>>	m_pipeline_layouts;
	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(RenderPass)>>		m_render_passes;
	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(VertexBuffer)>>	m_vertex_buffers;
};

ENGINE_NAMESPACE_END