//
// Created by AmazingBuff on 2024/12/30.
//

#include "image.h"
#include "rendering/resources/texture/texture_manager.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"

ENGINE_NAMESPACE_BEGIN

VK_CLASS(Image)::~VK_CLASS(Image)()
{
    auto device = g_system_context->g_render_system->m_drawable->m_device;
    for (auto& image : std::ranges::views::values(m_images))
    {
        vkDestroyImageView(device->m_device, image.image_view, nullptr);
        vkDestroyImage(device->m_device, image.image, nullptr);
    }
    vmaFreeMemory(device->m_allocator, m_allocation);
}

void VK_CLASS(Image)::initialize() {}


void VK_CLASS(Image)::create_image(const std::string& name, const TextureResource& resource)
{
    VkFormat format = VK_FORMAT_UNDEFINED;
    switch (resource.channels)
    {
    case 1:
        format = VK_FORMAT_R8_SRGB;
        break;
    case 3:
        format = VK_FORMAT_R8G8B8_SRGB;
        break;
    case 4:
        format = VK_FORMAT_R8G8B8A8_SRGB;
        break;
    default:
        RENDERING_LOG_ERROR("unsupported image channels");
        break;
    }

    VkImageCreateInfo image_create_info{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent = {
            .width = static_cast<uint32_t>(resource.width),
            .height = static_cast<uint32_t>(resource.height),
            .depth = 1,
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    VmaAllocationCreateInfo alloc_create_info{
        .flags = VMA_ALLOCATION_CREATE_DONT_BIND_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    auto device = g_system_context->g_render_system->m_drawable->m_device;

    VkImage image;
    VmaAllocationInfo alloc_info;
    VK_CHECK_RESULT(vmaCreateImage(device->m_allocator, &image_create_info, &alloc_create_info, &image, &m_allocation, &alloc_info));
    m_allocate_size = alloc_info.size;

    VK_CHECK_RESULT(vmaBindImageMemory2(device->m_allocator, m_allocation, m_current_offset, image, nullptr));

    VkImageViewCreateInfo create_info{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,
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
    VK_CHECK_RESULT(vkCreateImageView(device->m_device, &create_info, nullptr, &image_view));

    Image img{ image, image_view, nullptr };
    m_images.emplace(name, img);
}

void VK_CLASS(Image)::transition_image_layout(VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout)
{
    auto command_buffers = g_system_context->g_render_system->m_drawable->m_command_buffer;

    VkCommandBuffer command_buffer = command_buffers->begin_single_command();

    VkImageMemoryBarrier barrier{
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = 0,
        .dstAccessMask = 0,
        .oldLayout = old_layout,
        .newLayout = new_layout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        },
    };

    vkCmdPipelineBarrier(command_buffer, 0, 0, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    command_buffers->end_single_command(command_buffer);
}

void VK_CLASS(Image)::copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    auto command_buffers = g_system_context->g_render_system->m_drawable->m_command_buffer;

    VkCommandBuffer command_buffer = command_buffers->begin_single_command();

    VkBufferImageCopy copy_region{
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
        .imageOffset = {0, 0, 0},
        .imageExtent = {width, height, 1},
    };

    command_buffers->end_single_command(command_buffer);
}

ENGINE_NAMESPACE_END