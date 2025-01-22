#pragma once

#include "pipeline.h"

ENGINE_NAMESPACE_BEGIN

struct GlobalRuntimeInfo;

class VK_CLASS(PipelineResources) final : public RHI
{
public:
	VK_CLASS(PipelineResources)() = default;
	~VK_CLASS(PipelineResources)() override = default;
	NODISCARD constexpr RHIFlag flag() const override
	{
		return RHIFlag::e_pipeline_resources;
	}

	void initialize();
	NODISCARD const std::shared_ptr<VK_CLASS(Pipeline)>& get_pipeline(const std::string_view& name) const;
	NODISCARD const std::shared_ptr<VK_CLASS(PipelineLayout)>& get_pipeline_layout(const std::string_view& name) const;
	NODISCARD const std::shared_ptr<VK_CLASS(DescriptorSet)>& get_descriptor_set(const std::string_view& name) const;
	NODISCARD const std::shared_ptr<VK_CLASS(RenderPass)>& get_render_pass(const std::string_view& name) const;
	NODISCARD const std::shared_ptr<VK_CLASS(Buffer)>& get_vertex_buffer() const;
	NODISCARD const std::shared_ptr<VK_CLASS(Buffer)>& get_index_buffer() const;

	NODISCARD const BufferInfo& get_vertex_info(const std::string_view& name) const;
	NODISCARD const BufferInfo& get_index_info(const std::string_view& name) const;

	void update_uniform_buffer(const std::string_view& name, const GlobalRuntimeInfo& info) const;
private:
	void create_render_pass(const std::string& name);
	void create_pipeline_layout(const std::string& name);
	void create_pipeline(const std::string& name);
	void create_resource_manager();
private:
	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(Pipeline)>>		m_pipelines;

	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(PipelineLayout)>>	m_pipeline_layouts;
	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(DescriptorSet)>>	m_descriptor_sets;
	std::unordered_map<std::string, std::shared_ptr<VK_CLASS(RenderPass)>>		m_render_passes;
	std::shared_ptr<VK_CLASS(ResourceManager)>									m_resource_manager;
};

ENGINE_NAMESPACE_END