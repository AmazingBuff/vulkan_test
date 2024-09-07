#include "swap_chain.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"


ENGINE_NAMESPACE_BEGIN

VK_CLASS(FrameBuffer)::~VK_CLASS(FrameBuffer)()
{
	vkDestroyFramebuffer(g_system_context->g_render_system->m_drawable->m_device->m_device, m_frame_buffer, nullptr);
}

constexpr NODISCARD RHIFlag VK_CLASS(FrameBuffer)::flag() const
{
	return RHIFlag::e_framebuffer;
}

void VK_CLASS(FrameBuffer)::initialize()
{
	
}

void VK_CLASS(FrameBuffer)::initialize(VkImageView image_view, VkRenderPass render_pass)
{
	create_frame_buffer(image_view, render_pass);
}

void VK_CLASS(FrameBuffer)::create_frame_buffer(VkImageView image_view, VkRenderPass render_pass)
{
	auto& extent = g_system_context->g_render_system->m_drawable->m_swap_chain->m_info.extent.value();
	VkFramebufferCreateInfo create_info{
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.renderPass = render_pass,
		.attachmentCount = 1,
		.pAttachments = &image_view,
		.width = extent.width,
		.height = extent.height,
		.layers = 1
	};

	VK_CHECK_RESULT(vkCreateFramebuffer(g_system_context->g_render_system->m_drawable->m_device->m_device, &create_info, nullptr, &m_frame_buffer));
}


VK_CLASS(SwapChain)::~VK_CLASS(SwapChain)()
{
	VkDevice divece = g_system_context->g_render_system->m_drawable->m_device->m_device;
	for (auto& image_view : m_image_views)
		vkDestroyImageView(divece, image_view, nullptr);
	vkDestroySwapchainKHR(divece, m_swap_chain, nullptr);
}

void VK_CLASS(SwapChain)::create_frame_buffers(VkRenderPass render_pass)
{
	m_frame_buffers.resize(m_image_views.size());
	for (size_t i = 0; i < m_image_views.size(); ++i)
		m_frame_buffers[i].initialize(m_image_views[i], render_pass);
}

void VK_CLASS(SwapChain)::initialize()
{
	create_swap_chain();
	create_image_views();
}

void VK_CLASS(SwapChain)::create_swap_chain()
{
	auto drawable = g_system_context->g_render_system->m_drawable;
	auto physical_device = drawable->m_physical_device;

	auto& capabilities = physical_device->m_support_details.capabilities;
	uint32_t image_count = capabilities->minImageCount + 1;
	if (capabilities->maxImageCount > 0 && image_count > capabilities->maxImageCount)
		image_count = capabilities->maxImageCount;

	VkSwapchainCreateInfoKHR create_info{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = drawable->m_instance->m_surface,
		.minImageCount = image_count,
		.imageFormat = physical_device->m_swap_chain_details.format.value().format,
		.imageColorSpace = physical_device->m_swap_chain_details.format.value().colorSpace,
		.imageExtent = physical_device->m_swap_chain_details.extent.value(),
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.preTransform = capabilities->currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = physical_device->m_swap_chain_details.present_mode.value(),
		.clipped = VK_TRUE,
		.oldSwapchain = VK_NULL_HANDLE
	};

	if (physical_device->m_indices.is_same_family())
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	else
	{
		uint32_t queue_family_indices[] = {
			physical_device->m_indices.graphics_family.value(),
			physical_device->m_indices.present_family.value()
		};
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = queue_family_indices;
	}

	VK_CHECK_RESULT(vkCreateSwapchainKHR(drawable->m_device->m_device, &create_info, nullptr, &m_swap_chain));

	m_images = vkEnumerateProperties(vkGetSwapchainImagesKHR, drawable->m_device->m_device, m_swap_chain);
	m_info.format = create_info.imageFormat;
	m_info.extent = create_info.imageExtent;
}

void VK_CLASS(SwapChain)::create_image_views()
{
	m_image_views.resize(m_images.size());
	for (size_t i = 0; i < m_images.size(); ++i)
	{
		VkImageViewCreateInfo create_info{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = m_images[i],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = m_info.format.value(),
			.components = {
				.r = VK_COMPONENT_SWIZZLE_IDENTITY,
				.g = VK_COMPONENT_SWIZZLE_IDENTITY,
				.b = VK_COMPONENT_SWIZZLE_IDENTITY,
				.a = VK_COMPONENT_SWIZZLE_IDENTITY
			},
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};
		VK_CHECK_RESULT(vkCreateImageView(g_system_context->g_render_system->m_drawable->m_device->m_device, &create_info, nullptr, &m_image_views[i]));
	}
}

constexpr RHIFlag VK_CLASS(SwapChain)::flag() const
{
	return RHIFlag::e_swap_chain;
}


ENGINE_NAMESPACE_END
