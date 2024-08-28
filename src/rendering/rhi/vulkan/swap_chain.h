#pragma once

#include "types.h"
#include "utils/util.h"

VK_NAMESPACE_BEGIN

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

	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_swap_chain();
	void create_image_views();
private:
	VK_TYPE_INIT(VkSwapchainKHR,		m_swap_chain);
	SwapChainInfo						m_info;	
	std::vector<VkImage>				m_images;
	std::vector<VkImageView>			m_image_views;
};

VK_NAMESPACE_END