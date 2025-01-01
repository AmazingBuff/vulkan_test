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
    for (auto& sampler : m_occupied_samplers)
        m_sampler->release_sampler(sampler);

    auto device = g_system_context->g_render_system->m_drawable->m_device;
    for (auto& image : std::views::values(m_images))
    {
        vkDestroyImageView(device->m_device, image.image_view, nullptr);
        vkDestroyImage(device->m_device, image.image, nullptr);
    }
    vmaFreeMemory(device->m_allocator, m_allocation);
}

void VK_CLASS(Image)::initialize(const std::shared_ptr<VK_CLASS(Sampler)>& sampler) 
{
    m_sampler = sampler;
}

void VK_CLASS(Image)::create_image(const std::string& name, const TextureResource& resource, const std::shared_ptr<VK_CLASS(Buffer)>& src_buffer)
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

    auto drawable = g_system_context->g_render_system->m_drawable;
    auto device = drawable->m_device;

    // image
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

    VkImage image;
    VmaAllocationInfo alloc_info;
    VK_CHECK_RESULT(vmaCreateImage(device->m_allocator, &image_create_info, &alloc_create_info, &image, &m_allocation, &alloc_info));
    m_allocate_size = alloc_info.size;

    VK_CHECK_RESULT(vmaBindImageMemory2(device->m_allocator, m_allocation, m_current_offset, image, nullptr));

    VkDeviceSize buffer_size = static_cast<VkDeviceSize>(resource.width * resource.height * resource.channels);
    set_info(name, static_cast<VkDeviceSize>(resource.width * resource.height * resource.channels), 1);
    src_buffer->map_memory(resource.data, buffer_size, 0);

    transition_image_layout(image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copy_buffer_to_image(src_buffer->m_buffer, image, static_cast<uint32_t>(resource.width), static_cast<uint32_t>(resource.height));
    transition_image_layout(image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // image view
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


    Image img{ image, image_view };
    m_images.emplace(name, img);
}

void VK_CLASS(Image)::update_descriptor_set(const std::vector<SampledImageLayout>& layouts, const std::unordered_map<std::string, std::string>& name_to_res_name_map, const std::unordered_map<uint32_t, VkDescriptorSet>& descriptor_sets)
{
    for (auto& layout : layouts)
    {
        VkSampler sampler = m_sampler->get_sampler();
        m_occupied_samplers.push_back(sampler);

        VkDescriptorImageInfo image_info{
            .sampler = sampler,
            .imageView = m_images.at(name_to_res_name_map.at(layout.resource_name)).image_view,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };


        VkWriteDescriptorSet writeInfo{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptor_sets.at(layout.set),
            .dstBinding = layout.binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &image_info,
            .pBufferInfo = nullptr,
            .pTexelBufferView = nullptr,
        };

        auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
        vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
    }
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

    VkPipelineStageFlags source_stage = VK_PIPELINE_STAGE_NONE;
    VkPipelineStageFlags destination_stage = VK_PIPELINE_STAGE_NONE;

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
        RENDERING_LOG_ERROR("unsupported image layout transition");


    vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

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

    vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);

    command_buffers->end_single_command(command_buffer);
}


VK_CLASS(Sampler)::~VK_CLASS(Sampler)()
{
    if (!m_occupied_list.empty())
        RENDERING_LOG_ERROR("sampler is still in use");

    auto device = g_system_context->g_render_system->m_drawable->m_device;
    while (!m_available_queue.empty())
    {
        vkDestroySampler(device->m_device, m_available_queue.front().sampler, nullptr);
        m_available_queue.pop();
    }
}

void VK_CLASS(Sampler)::initialize(){}

VkSampler VK_CLASS(Sampler)::get_sampler()
{
    if (m_available_queue.empty())
        create_sampler();

    Sampler& sampler = m_available_queue.front();
    m_available_queue.pop();
    sampler.is_available = false;
    m_occupied_list.push_back(sampler);

    return sampler.sampler;
}

void VK_CLASS(Sampler)::release_sampler(VkSampler sampler)
{
    for (auto it = m_occupied_list.begin(); it != m_occupied_list.end(); it++)
    {
        if (it->sampler == sampler)
        {
            it->is_available = true;
            m_available_queue.push(*it);
            m_occupied_list.erase(it);
            break;
        }
    }
}

void VK_CLASS(Sampler)::create_sampler()
{
    auto drawable = g_system_context->g_render_system->m_drawable;
    auto device = drawable->m_device;

    VkSamplerCreateInfo sampler_info{
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .mipLodBias = 0.0f,
        .anisotropyEnable = VK_TRUE,
        .maxAnisotropy = drawable->m_physical_device->m_properties.limits.maxSamplerAnisotropy,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .minLod = 0.0f,
        .maxLod = 0.0f,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE,
    };

    VkSampler sampler;
    VK_CHECK_RESULT(vkCreateSampler(device->m_device, &sampler_info, nullptr, &sampler));

    Sampler sampler_obj{ sampler, true };
    m_available_queue.push(sampler_obj);
}

ENGINE_NAMESPACE_END