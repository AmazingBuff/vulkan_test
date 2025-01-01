//
// Created by AmazingBuff on 2024/12/27.
//

#include "resource_manager.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"
#include "rendering/render_resouces.h"
#include "rendering/resources/texture/texture_manager.h"

ENGINE_NAMESPACE_BEGIN

void VK_CLASS(ResourceManager)::initialize(VkDeviceSize buffer_size)
{
    m_staging_buffer = std::make_unique<VK_CLASS(Buffer)>();
    m_staging_buffer->init(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    m_vertex_buffer = std::make_unique<VK_CLASS(Buffer)>();
    m_vertex_buffer->init(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_index_buffer = std::make_unique<VK_CLASS(Buffer)>();
    m_index_buffer->init(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_samplers = std::make_unique<VK_CLASS(Sampler)>();
    m_samplers->init();
    
    m_images = std::make_unique<VK_CLASS(Image)>();
    m_images->init(m_samplers);
}

void VK_CLASS(ResourceManager)::configure_uniform_buffer(const std::string& res_name, const std::vector<UniformBufferLayout>& layouts, const std::array<std::unordered_map<uint32_t, VkDescriptorSet>, k_Max_Frames_In_Flight>& descriptor_sets)
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

void VK_CLASS(ResourceManager)::create_image(const std::string& res_name)
{
    const TextureResource& resource = g_system_context->g_render_system->m_render_resources->get_texture_resource(res_name);

    VkDeviceSize buffer_size = static_cast<VkDeviceSize>(resource.width * resource.height * resource.channels);
    m_staging_buffer->map_memory(resource.data, buffer_size, 0);
    m_images->create_image(res_name, resource, m_staging_buffer);
}

void VK_CLASS(ResourceManager)::configure_image(const std::vector<SampledImageLayout>& layouts, const std::unordered_map<std::string, std::string>& name_to_res_name_map, const std::array<std::unordered_map<uint32_t, VkDescriptorSet>, k_Max_Frames_In_Flight>& descriptor_sets)
{ 
    for (uint32_t i = 0; i < k_Max_Frames_In_Flight; i++)
        m_images->update_descriptor_set(layouts, name_to_res_name_map, descriptor_sets[i]);
}


ENGINE_NAMESPACE_END
