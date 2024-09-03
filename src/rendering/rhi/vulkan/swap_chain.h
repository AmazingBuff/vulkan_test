#pragma once

#include "types.h"
#include "utils/util.h"

RHI_NAMESPACE_BEGIN

class VK_CLASS(FrameBuffer) final : public RHI
{
public:
	VK_CLASS(FrameBuffer)() = default;
	~VK_CLASS(FrameBuffer)() override;
	void initialize() override;
	void initialize(VkImageView image_view, VkRenderPass render_pass);

	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_frame_buffer(VkImageView image_view, VkRenderPass render_pass);
private:
	VK_TYPE_INIT(VkFramebuffer, m_frame_buffer);
};


class VK_CLASS(SwapChain) final : public RHI
{
public:
	struct SwapChainInfo
	{
		std::optional<VkFormat> format;
		std::optional<VkExtent2D> extent;

		NODISCARD constexpr operator bool() const
		{
			return format.has_value() && extent.has_value();
		}
	};

public:
	VK_CLASS(SwapChain)() = default;
	~VK_CLASS(SwapChain)();
	void initialize() override;
	void create_frame_buffers(VkRenderPass render_pass);

	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_swap_chain();
	void create_image_views();
private:
	VK_TYPE_INIT(VkSwapchainKHR,		m_swap_chain);
	SwapChainInfo						m_info;	
	std::vector<VkImage>				m_images;
	std::vector<VkImageView>			m_image_views;
	std::vector<VK_CLASS(FrameBuffer)>	m_frame_buffers;

	friend class VK_CLASS(PipelineLayout);
	friend class VK_CLASS(RenderPass);
	friend class VK_CLASS(Pipeline);
	friend class VK_CLASS(FrameBuffer);
};

RHI_NAMESPACE_END