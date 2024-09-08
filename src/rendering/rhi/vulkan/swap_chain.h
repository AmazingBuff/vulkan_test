#pragma once

#include "types.h"
#include "utils/util.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(Framebuffer) final : public RHI
{
public:
	VK_CLASS(Framebuffer)() = default;
	~VK_CLASS(Framebuffer)() override;
	void set_render_pass(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass);
	void initialize() override;

	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_frame_buffer();
private:
	std::shared_ptr<VK_CLASS(RenderPass)> m_render_pass;
private:
	VK_TYPE_INIT(VkFramebuffer, m_frame_buffer);
	VK_TYPE_INIT(VkImageView, m_image_view);

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

		NODISCARD constexpr operator bool() const
		{
			return format.has_value() && present_mode.has_value() && extent.has_value();
		}
	};

public:
	VK_CLASS(SwapChain)() = default;
	~VK_CLASS(SwapChain)();
	void initialize() override;
	NODISCARD constexpr RHIFlag flag() const override;

	void create_frame_buffers(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass);
	// return false means that a new swap chain need to be created
	NODISCARD bool acquire_next_image();
	// must acquire next image first
	NODISCARD const std::shared_ptr<VK_CLASS(Framebuffer)>& current_frame_buffer() const;
	//
	NODISCARD bool is_minimization() const;
private:
	void choose_swap_chain_details();
	void create_swap_chain();
	void create_image_views();
private:
	VK_TYPE_INIT(VkSwapchainKHR,							m_swap_chain);
	SwapChainSupportDetails									m_support_details;	
	SwapChainDetails										m_details;
	std::vector<VkImage>									m_images;
	std::vector<std::shared_ptr<VK_CLASS(Framebuffer)>>		m_frame_buffers;

	uint32_t												m_image_index = 0;
	bool													m_frame_buffer_resize = false;

	friend class VK_CLASS(PipelineLayout);
	friend class VK_CLASS(RenderPass);
	friend class VK_CLASS(Pipeline);
	friend class VK_CLASS(Framebuffer);
	friend class VK_CLASS(CommandBuffer);
	friend class VK_CLASS(Device);
};

ENGINE_NAMESPACE_END