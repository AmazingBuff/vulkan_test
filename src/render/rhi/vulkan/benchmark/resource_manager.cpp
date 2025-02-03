//
// Created by AmazingBuff on 2024/12/27.
//

#include "resource_manager.h"
#include "render/drawable.h"
#include "render/renderer.h"
#include "system/system.h"

ENGINE_NAMESPACE_BEGIN

VK_CLASS(ResourceManager)::~VK_CLASS(ResourceManager)()
{
    for (auto& image : std::views::values(m_images))
        image->release_sampler(m_samplers);
}

void VK_CLASS(ResourceManager)::initialize(VkDeviceSize buffer_size)
{
    m_staging_buffer = std::make_unique<VK_CLASS(Buffer)>();
    m_staging_buffer->init(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    m_vertex_buffer = std::make_unique<VK_CLASS(Buffer)>();
    m_vertex_buffer->init(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_index_buffer = std::make_unique<VK_CLASS(Buffer)>();
    m_index_buffer->init(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_samplers = std::make_unique<VK_CLASS(Sampler)>();
    m_samplers->init();
}

void VK_CLASS(ResourceManager)::map_vertex_buffer(const std::string& name, const std::vector<Vertex>& vertices) const
{
    VkDeviceSize buffer_size = sizeof(Vertex) * vertices.size();
    m_staging_buffer->map_memory(vertices.data(), buffer_size, 0);
    m_vertex_buffer->set_info(name, buffer_size, static_cast<uint32_t>(vertices.size()));
    auto& info = m_vertex_buffer->get_info(name);
    m_vertex_buffer->copy_buffer_from(m_staging_buffer, info.offset, buffer_size);
}

void VK_CLASS(ResourceManager)::map_index_buffer(const std::string& name, const std::vector<uint32_t>& indices) const
{
    VkDeviceSize buffer_size = sizeof(uint32_t) * indices.size();
    m_staging_buffer->map_memory(indices.data(), buffer_size, 0);
    m_index_buffer->set_info(name, buffer_size, static_cast<uint32_t>(indices.size()));
    auto& info = m_index_buffer->get_info(name);
    m_index_buffer->copy_buffer_from(m_staging_buffer, info.offset, buffer_size);
}

void VK_CLASS(ResourceManager)::configure_uniform_buffer(
    const std::string& res_name, const std::vector<UniformBufferLayout>& layouts,
    const std::array<std::unordered_map<uint32_t, VkDescriptorSet>, k_Max_Frames_In_Flight>& descriptor_sets)
{
    auto it = m_uniform_buffers.find(res_name);
    if (it != m_uniform_buffers.end())
    {
        auto& uniform_buffers = it->second;
        for (uint32_t i = 0; i < k_Max_Frames_In_Flight; ++i)
            uniform_buffers[i]->update_descriptor_set(layouts, descriptor_sets[i]);
    }
    else
    {
        std::array<std::shared_ptr<VK_CLASS(UniformBuffer)>, k_Max_Frames_In_Flight> uniform_buffers;

        for (uint32_t i = 0; i < k_Max_Frames_In_Flight; ++i)
        {
            uniform_buffers[i] = std::make_shared<VK_CLASS(UniformBuffer)>();
            uniform_buffers[i]->init();
            uniform_buffers[i]->update_descriptor_set(layouts, descriptor_sets[i]);
        }
        m_uniform_buffers[res_name] = uniform_buffers;
    }
}

void VK_CLASS(ResourceManager)::create_image(const std::string& res_name, const TextureResource& resource, bool mipmap)
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

    uint32_t width = static_cast<uint32_t>(resource.width);
    uint32_t height = static_cast<uint32_t>(resource.height);
    uint32_t mip_levels = 1;
    if (mipmap)
        mip_levels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1u;

    // image
    VkImageCreateInfo image_create_info{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent =
            {
                .width = width,
                .height = height,
                .depth = 1,
            },
        .mipLevels = mip_levels,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    VkImage vk_image;
    auto    device = g_system_context->g_render_system->m_drawable->m_device;
    VK_CHECK_RESULT(vkCreateImage(device->m_device, &image_create_info, nullptr, &vk_image));

    VkMemoryRequirements memory_requirements;
    vkGetImageMemoryRequirements(device->m_device, vk_image, &memory_requirements);

    MemoryRequirements requirements{
        .alignment = memory_requirements.alignment,
        .memoryTypeBits = memory_requirements.memoryTypeBits,
    };
    auto it = m_images.find(requirements);
    if (it != m_images.end())
        it->second->map_memory(res_name, vk_image, resource, m_staging_buffer, mip_levels);
    else
    {
        std::shared_ptr<VK_CLASS(Image)> image = std::make_shared<VK_CLASS(Image)>();
        image->init(requirements);
        image->map_memory(res_name, vk_image, resource, m_staging_buffer, mip_levels);
        m_images[requirements] = image;
    }
}

void VK_CLASS(ResourceManager)::configure_image(
    const std::vector<SampledImageLayout>&                                                   layouts,
    const std::unordered_map<std::string, std::string>&                                      name_to_res_name_map,
    const std::array<std::unordered_map<uint32_t, VkDescriptorSet>, k_Max_Frames_In_Flight>& descriptor_sets)
{
    for (uint32_t i = 0; i < k_Max_Frames_In_Flight; i++)
    {
        for (auto& image : std::views::values(m_images))
            image->update_descriptor_set(layouts, name_to_res_name_map, descriptor_sets[i], m_samplers);
    }
}


ENGINE_NAMESPACE_END
