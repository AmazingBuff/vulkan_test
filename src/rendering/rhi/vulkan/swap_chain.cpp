#include "swap_chain.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"
#include "window/window.h"
#include <SDL2/SDL_vulkan.h>


ENGINE_NAMESPACE_BEGIN

VK_CLASS(Framebuffer)::~VK_CLASS(Framebuffer)()
{
	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
	vkDestroyImageView(device, m_image_view, nullptr);
	vkDestroyFramebuffer(device, m_frame_buffer, nullptr);
}

void VK_CLASS(Framebuffer)::set_render_pass(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass)
{
	m_render_pass = render_pass;
}

constexpr NODISCARD RHIFlag VK_CLASS(Framebuffer)::flag() const
{
	return RHIFlag::e_framebuffer;
}

void VK_CLASS(Framebuffer)::initialize()
{
	create_frame_buffer();
}

void VK_CLASS(Framebuffer)::create_frame_buffer()
{
	auto& extent = g_system_context->g_render_system->m_drawable->m_swap_chain->m_details.extent.value();
	VkFramebufferCreateInfo create_info{
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.renderPass = m_render_pass->m_render_pass,
		.attachmentCount = 1,
		.pAttachments = &m_image_view,
		.width = extent.width,
		.height = extent.height,
		.layers = 1
	};

	VK_CHECK_RESULT(vkCreateFramebuffer(g_system_context->g_render_system->m_drawable->m_device->m_device, &create_info, nullptr, &m_frame_buffer));
}


VK_CLASS(SwapChain)::~VK_CLASS(SwapChain)()
{
	vkDestroySwapchainKHR(g_system_context->g_render_system->m_drawable->m_device->m_device, m_swap_chain, nullptr);
}

const std::shared_ptr<VK_CLASS(Framebuffer)>& VK_CLASS(SwapChain)::current_frame_buffer() const
{
	return m_frame_buffers[m_image_index];
}

NODISCARD bool VK_CLASS(SwapChain)::acquire_next_image()
{
	auto drawable = g_system_context->g_render_system->m_drawable;
	VkResult result = vkAcquireNextImageKHR(
		drawable->m_device->m_device,
		m_swap_chain, 
		UINT64_MAX, 
		drawable->m_command_buffer->m_image_available_semaphores[drawable->m_command_buffer->m_current_frame]->m_semaphore,
		VK_NULL_HANDLE, 
		&m_image_index);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		return false;
	else
		VK_CHECK_RESULT(result);

	return true;
}

void VK_CLASS(SwapChain)::create_frame_buffers(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass)
{
	for (auto& frame_buffer : m_frame_buffers)
	{
		frame_buffer->set_render_pass(render_pass);
		frame_buffer->initialize();
	}
}

void VK_CLASS(SwapChain)::initialize()
{
	choose_swap_chain_details();
	create_swap_chain();
	create_image_views();
}

void VK_CLASS(SwapChain)::choose_swap_chain_details()
{
	m_support_details = query_swap_chain_support(g_system_context->g_render_system->m_drawable->m_physical_device->m_device, g_system_context->g_render_system->m_drawable->m_instance->m_surface);

	if (!std::any_of(m_support_details.formats.begin(), m_support_details.formats.end(),
		[&](const VkSurfaceFormatKHR& available_format) -> bool
		{
			if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
				available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				m_details.format = available_format;
				return true;
			}
			else
				return false;
		}))
		m_details.format = m_support_details.formats[0];

	if (!std::any_of(m_support_details.present_modes.begin(), m_support_details.present_modes.end(),
		[&](const VkPresentModeKHR& available_present_mode) -> bool
		{
			if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				m_details.present_mode = available_present_mode;
				return true;
			}
			else
				return false;
		}))
		m_details.present_mode = VK_PRESENT_MODE_FIFO_KHR;

	if (m_support_details.capabilities->currentExtent.width != std::numeric_limits<uint32_t>::max())
		m_details.extent = m_support_details.capabilities->currentExtent;
	else
	{
		int width, height;
		SDL_Vulkan_GetDrawableSize(g_system_context->g_window_system->m_window, &width, &height);
		VkExtent2D actual_extent{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		actual_extent.width = std::clamp(actual_extent.width, m_support_details.capabilities->minImageExtent.width, m_support_details.capabilities->maxImageExtent.width);
		actual_extent.height = std::clamp(actual_extent.height, m_support_details.capabilities->minImageExtent.height, m_support_details.capabilities->maxImageExtent.height);
		m_details.extent = actual_extent;
	}
}

void VK_CLASS(SwapChain)::create_swap_chain()
{
	auto drawable = g_system_context->g_render_system->m_drawable;
	auto physical_device = drawable->m_physical_device;

	auto& capabilities = m_support_details.capabilities;
	uint32_t image_count = capabilities->minImageCount + 1;
	if (capabilities->maxImageCount > 0 && image_count > capabilities->maxImageCount)
		image_count = capabilities->maxImageCount;

	VkSwapchainCreateInfoKHR create_info{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = drawable->m_instance->m_surface,
		.minImageCount = image_count,
		.imageFormat = m_details.format.value().format,
		.imageColorSpace = m_details.format.value().colorSpace,
		.imageExtent = m_details.extent.value(),
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.preTransform = capabilities->currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = m_details.present_mode.value(),
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
}

void VK_CLASS(SwapChain)::create_image_views()
{
	m_frame_buffers.resize(m_images.size());
	for (size_t i = 0; i < m_images.size(); ++i)
	{
		m_frame_buffers[i] = std::make_shared<VK_CLASS(Framebuffer)>();

		VkImageViewCreateInfo create_info{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = m_images[i],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = m_details.format.value().format,
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
		VK_CHECK_RESULT(vkCreateImageView(g_system_context->g_render_system->m_drawable->m_device->m_device, &create_info, nullptr, &m_frame_buffers[i]->m_image_view));
	}
}

constexpr RHIFlag VK_CLASS(SwapChain)::flag() const
{
	return RHIFlag::e_swap_chain;
}

ENGINE_NAMESPACE_END