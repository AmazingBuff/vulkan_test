#pragma once

#include "types.h"
#include "utils/util.h"
#include "benchmark/buffer.h"
#include "rendering/render_types.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(PipelineLayout) final : public RHI
{
public:
	VK_CLASS(PipelineLayout)() = default;
	~VK_CLASS(PipelineLayout)() override;
	NODISCARD constexpr RHIFlag flag() const override;

	void initialize();
private:
	void create_pipeline_layout();
private:
	VK_TYPE_INIT(VkPipelineLayout, m_pipeline_layout);

	friend class VK_CLASS(Pipeline);
};


class VK_CLASS(RenderPass) final : public RHI
{
public:
	VK_CLASS(RenderPass)() = default;
	~VK_CLASS(RenderPass)() override;
	NODISCARD constexpr RHIFlag flag() const override;

	void initialize(const std::string_view& name);
private:
	void create_render_pass(const std::string_view& name);
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
	NODISCARD constexpr RHIFlag flag() const override;

	void initialize(const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout, const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass);
private:
	void create_pipeline(const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout, const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass);
private:
	VK_TYPE_INIT(VkPipeline,					m_pipeline);

	friend class VK_CLASS(CommandBuffer);
	friend class VK_CLASS(SwapChain);
};


ENGINE_NAMESPACE_END