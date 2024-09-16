#pragma once

#include "types.h"
#include "pipeline.h"

ENGINE_NAMESPACE_BEGIN


class VK_CLASS(PipelineResources) : public RHI
{
public:
	VK_CLASS(PipelineResources)() = default;
	~VK_CLASS(PipelineResources)() override = default;
	void initialize() override;
	NODISCARD constexpr RHIFlag flag() const override;

public:
	NODISCARD std::shared_ptr<VK_CLASS(Pipeline)> get_pipeline(const std::string & name) const;
private:
	void create_render_pass();
	void create_pipeline_layout();
	void create_pipelines();
	void create_vertex_buffers();
private:
	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(Pipeline)>>		m_pipelines;

	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(PipelineLayout)>>	m_pipeline_layouts;
	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(RenderPass)>>		m_render_passes;
	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(VertexBuffer)>>	m_vertex_buffers;
};

ENGINE_NAMESPACE_END