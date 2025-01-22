#include "swap_chain.h"
#include "system/system.h"
#include "render/renderer.h"
#include "render/drawable.h"
#include "window/window.h"
#include "benchmark/image.h"
#include <SDL2/SDL_vulkan.h>


ENGINE_NAMESPACE_BEGIN

VK_CLASS(Framebuffer)::~VK_CLASS(Framebuffer)()
{
	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
	vkDestroyFramebuffer(device, m_frame_buffer, nullptr);
}

void VK_CLASS(Framebuffer)::initialize(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass, const std::vector<VkImageView>& image_views)
{
	create_frame_buffer(render_pass, image_views);
}

void VK_CLASS(Framebuffer)::create_frame_buffer(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass, const std::vector<VkImageView>& image_views)
{
	auto& extent = g_system_context->g_render_system->m_drawable->m_swap_chain->m_details.extent.value();
	VkFramebufferCreateInfo create_info{
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.renderPass = render_pass->m_render_pass,
		.attachmentCount = static_cast<uint32_t>(image_views.size()),
		.pAttachments = image_views.data(),
		.width = extent.width,
		.height = extent.height,
		.layers = 1
	};

	VK_CHECK_RESULT(vkCreateFramebuffer(g_system_context->g_render_system->m_drawable->m_device->m_device, &create_info, nullptr, &m_frame_buffer));
}


VK_CLASS(SwapChain)::~VK_CLASS(SwapChain)()
{
	auto device = g_system_context->g_render_system->m_drawable->m_device;

    vmaDestroyImage(device->m_allocator, m_depth_image, m_depth_image_allocation);
    vkDestroyImageView(device->m_device, m_depth_image_view, nullptr);
    for (auto& image_view : m_image_views)
        vkDestroyImageView(device->m_device, image_view, nullptr);
	vkDestroySwapchainKHR(device->m_device, m_swap_chain, nullptr);
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
		std::numeric_limits<uint64_t>::max(),
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
	for (auto& image_view : m_image_views)
	{
		std::shared_ptr<VK_CLASS(Framebuffer)> frame_buffer = std::make_shared<VK_CLASS(Framebuffer)>();
		frame_buffer->initialize(render_pass, std::vector{ image_view, m_depth_image_view });
        m_frame_buffers.emplace_back(frame_buffer);
	}
}

void VK_CLASS(SwapChain)::initialize(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass)
{
	choose_swap_chain_details();
	create_swap_chain();
	create_color_resources();
    create_depth_resources();

	if (render_pass)
        create_frame_buffers(render_pass);
}

void VK_CLASS(SwapChain)::choose_swap_chain_details()
{
	m_support_details = query_swap_chain_support(g_system_context->g_render_system->m_drawable->m_physical_device->m_device, g_system_context->g_render_system->m_drawable->m_instance->m_surface);

	if (!std::ranges::any_of(m_support_details.formats,
	                         [&](const VkSurfaceFormatKHR& available_format) -> bool
	                         {
		                         if (available_format.format == VK_FORMAT_R8G8B8A8_SRGB &&
			                         available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		                         {
			                         m_details.format = available_format;
			                         return true;
		                         }
		                         else
			                         return false;
	                         }))
		m_details.format = m_support_details.formats[0];

	if (!std::ranges::any_of(m_support_details.present_modes,
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
}

void VK_CLASS(SwapChain)::create_color_resources()
{
	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;

	m_images = vkEnumerateProperties(vkGetSwapchainImagesKHR, device, m_swap_chain);
	for (size_t i = 0; i < m_images.size(); ++i)
	{
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

		VkImageView image_view;
		VK_CHECK_RESULT(vkCreateImageView(device, &create_info, nullptr, &image_view));
		m_image_views.emplace_back(image_view);
	}
}

void VK_CLASS(SwapChain)::create_depth_resources()
{
	auto drawable = g_system_context->g_render_system->m_drawable;
	auto device = drawable->m_device;

    VkFormat depth_format = drawable->m_physical_device->find_supported_format(
		{VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, 
		VK_FORMAT_D16_UNORM, VK_FORMAT_D16_UNORM_S8_UINT},
		VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	VkImageCreateInfo image_create_info{
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = depth_format,
		.extent = {
			.width = static_cast<uint32_t>(m_details.extent->width),
			.height = static_cast<uint32_t>(m_details.extent->height),
			.depth = 1,
		},
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	};

	VmaAllocationCreateInfo allocation_create_info{
        .usage = VMA_MEMORY_USAGE_GPU_ONLY
	};

	VK_CHECK_RESULT(vmaCreateImage(device->m_allocator, &image_create_info, &allocation_create_info, &m_depth_image, &m_depth_image_allocation, nullptr));

    VK_CLASS(Image)::transition_image_layout(m_depth_image, depth_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);

    // image view
	VkImageViewCreateInfo create_info{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = m_depth_image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = depth_format,
			.components = {
				.r = VK_COMPONENT_SWIZZLE_IDENTITY,
				.g = VK_COMPONENT_SWIZZLE_IDENTITY,
				.b = VK_COMPONENT_SWIZZLE_IDENTITY,
				.a = VK_COMPONENT_SWIZZLE_IDENTITY
			},
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
	};

	VK_CHECK_RESULT(vkCreateImageView(device->m_device, &create_info, nullptr, &m_depth_image_view));
}

ENGINE_NAMESPACE_END