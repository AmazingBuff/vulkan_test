#pragma once

#include "types.h"
#include "utils/util.h"

RHI_NAMESPACE_BEGIN

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
};


class VK_CLASS(Pipeline) final : public RHI
{
public:
	VK_CLASS(Pipeline)() = default;
	~VK_CLASS(Pipeline)();
	void initialize() override;


	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_pipeline();
private:
	std::shared_ptr<VK_CLASS(PipelineLayout)>	m_pipeline_layout;
	std::shared_ptr<VK_CLASS(RenderPass)>		m_render_pass;
private:
	VK_TYPE_INIT(VkPipeline,					m_pipeline);
};


RHI_NAMESPACE_END