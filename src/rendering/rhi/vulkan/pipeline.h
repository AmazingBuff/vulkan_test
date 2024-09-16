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
	~VK_CLASS(PipelineLayout)();
	void initialize() override;

	NODISCARD constexpr RHIFlag flag() const override;
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
	~VK_CLASS(RenderPass)();
	void initialize() override;

	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_render_pass();
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
	~VK_CLASS(Pipeline)();
	void set_pipeline_layout(const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout);
	void set_render_pass(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass);
	void initialize() override;


	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_pipeline();
private:
	std::shared_ptr<VK_CLASS(PipelineLayout)>	m_pipeline_layout;
	std::shared_ptr<VK_CLASS(RenderPass)>		m_render_pass;
	std::shared_ptr<VK_CLASS(VertexBuffer)>		m_vertex_buffer;
private:
	VK_TYPE_INIT(VkPipeline,					m_pipeline);

	friend class VK_CLASS(CommandBuffer);
	friend class VK_CLASS(SwapChain);
};


ENGINE_NAMESPACE_END