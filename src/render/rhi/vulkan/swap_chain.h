#pragma once

#include "types.h"
#include "utils/util.h"
#include "fork/vk_mem_alloc.h"


ENGINE_NAMESPACE_BEGIN

class VK_CLASS(Framebuffer) final : public RHI
{
public:
	VK_CLASS(Framebuffer)() = default;
	~VK_CLASS(Framebuffer)() override;
	NODISCARD constexpr RHIFlag flag() const override
	{
		return RHIFlag::e_framebuffer;
	}

	void initialize(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass, const std::vector<VkImageView>& image_views);
private:
	void create_frame_buffer(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass, const std::vector<VkImageView>& image_views);
private:
	VK_TYPE_INIT(VkFramebuffer,		m_frame_buffer);

	friend class VK_CLASS(CommandBuffer);
	friend class VK_CLASS(SwapChain);
};


class VK_CLASS(SwapChain) final : public RHI
{
public:
	struct SwapChainDetails
	{
		std::optional<VkSurfaceFormatKHR>		format;
		std::optional<VkPresentModeKHR>			present_mode;
		std::optional<VkExtent2D>				extent;

		NODISCARD constexpr explicit operator bool() const
		{
			return format.has_value() && present_mode.has_value() && extent.has_value();
		}
	};

public:
	VK_CLASS(SwapChain)() = default;
	~VK_CLASS(SwapChain)() override;
	NODISCARD constexpr RHIFlag flag() const override
	{
		return RHIFlag::e_swap_chain;
	}

	void initialize(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass = nullptr);
	void create_frame_buffers(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass);
	// return false means that a new swap chain need to be created
	NODISCARD bool acquire_next_image();
	// must acquire next image first
	NODISCARD const std::shared_ptr<VK_CLASS(Framebuffer)>& current_frame_buffer() const;
private:
	void choose_swap_chain_details();
	void create_swap_chain();
	void create_color_resources();
    void create_depth_resources();
private:
	VK_TYPE_INIT(VkSwapchainKHR,							m_swap_chain);
	SwapChainSupportDetails									m_support_details;	
	SwapChainDetails										m_details;
	std::vector<VkImage>									m_images;
	std::vector<VkImageView>								m_image_views;
	std::vector<std::shared_ptr<VK_CLASS(Framebuffer)>>		m_frame_buffers;

    VK_TYPE_INIT(VkImage,									m_depth_image);
	VK_TYPE_INIT(VkImageView,								m_depth_image_view);
    VK_TYPE_INIT(VmaAllocation,								m_depth_image_allocation);

	uint32_t												m_image_index = 0;
	bool													m_frame_buffer_resize = false;

	friend class VK_CLASS(PipelineLayout);
	friend class VK_CLASS(RenderPass);
	friend class VK_CLASS(Pipeline);
	friend class VK_CLASS(PipelineResources);
	friend class VK_CLASS(Framebuffer);
	friend class VK_CLASS(CommandBuffer);
	friend class VK_CLASS(Device);
};

ENGINE_NAMESPACE_END