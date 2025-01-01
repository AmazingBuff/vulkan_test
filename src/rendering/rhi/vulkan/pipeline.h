#pragma once

#include "types.h"
#include "utils/util.h"
#include "benchmark/buffer.h"

ENGINE_NAMESPACE_BEGIN

struct PipelineLayoutInfo;
struct RenderPassInfo;
struct PipelineInfo;


struct UniformBufferLayout
{
	std::string		type_name;
	std::string		resource_name;
	uint32_t		set;
	uint32_t		binding;
	uint32_t		array_count;
	size_t			size;
};

struct SampledImageLayout
{
    std::string		resource_name;
	uint32_t		dimension;
    uint32_t		set;
    uint32_t		binding;
	uint32_t		array_count;
};

struct ShaderResourcesLayout
{
    std::vector<UniformBufferLayout>	uniform_buffers;
    std::vector<SampledImageLayout>		sampled_images;
};

class VK_CLASS(PipelineLayout) final : public RHI
{
public:
	VK_CLASS(PipelineLayout)() = default;
	~VK_CLASS(PipelineLayout)() override;
	NODISCARD constexpr RHIFlag flag() const override
	{
		return RHIFlag::e_pipeline_layout;
	}

	void initialize(const std::shared_ptr<PipelineLayoutInfo>& pipeline_layout_info);
private:
	void create_pipeline_layout(const std::shared_ptr<PipelineLayoutInfo>& pipeline_layout_info);
private:
	// set <----> descriptor set layout
	std::unordered_map<uint32_t, VkDescriptorSetLayout>			m_descriptor_set_layouts;
	VK_TYPE_INIT(VkPipelineLayout,								m_pipeline_layout);
	ShaderResourcesLayout										m_shader_resources_layout;

	friend class VK_CLASS(Pipeline);
	friend class VK_CLASS(DescriptorSet);
	friend class VK_CLASS(CommandBuffer);
	friend class VK_CLASS(PipelineResources);
};


class VK_CLASS(RenderPass) final : public RHI
{
public:
	VK_CLASS(RenderPass)() = default;
	~VK_CLASS(RenderPass)() override;
	NODISCARD constexpr RHIFlag flag() const override
	{
		return RHIFlag::e_render_pass;
	}

	void initialize(const std::shared_ptr<RenderPassInfo>& render_pass_resource);
private:
	void create_render_pass(const std::shared_ptr<RenderPassInfo>& render_pass_resource);
private:
	VK_TYPE_INIT(VkRenderPass, m_render_pass);

	friend class VK_CLASS(Pipeline);
	friend class VK_CLASS(CommandBuffer);
	friend class VK_CLASS(SwapChain);
	friend class VK_CLASS(Framebuffer);
};


class VK_CLASS(Pipeline) final : public RHI
{
public:
	VK_CLASS(Pipeline)() = default;
	~VK_CLASS(Pipeline)() override;
	NODISCARD constexpr RHIFlag flag() const override
	{
		return RHIFlag::e_pipeline;
	}

	void initialize(const std::shared_ptr<PipelineInfo>& pipeline, const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout, const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass);
private:
	void create_pipeline(const std::shared_ptr<PipelineInfo>& pipeline, const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout, const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass);
private:
	VK_TYPE_INIT(VkPipeline,					m_pipeline);

	friend class VK_CLASS(CommandBuffer);
	friend class VK_CLASS(SwapChain);
};


ENGINE_NAMESPACE_END