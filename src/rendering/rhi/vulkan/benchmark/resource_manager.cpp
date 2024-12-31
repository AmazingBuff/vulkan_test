//
// Created by AmazingBuff on 2024/12/27.
//

#include "resource_manager.h"

ENGINE_NAMESPACE_BEGIN

void VK_CLASS(ResourceManager)::initialize(VkDeviceSize buffer_size)
{
    m_staging_buffer = std::make_unique<VK_CLASS(Buffer)>();
    m_staging_buffer->init(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    m_vertex_buffer = std::make_unique<VK_CLASS(Buffer)>();
    m_vertex_buffer->init(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_index_buffer = std::make_unique<VK_CLASS(Buffer)>();
    m_index_buffer->init(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}

void VK_CLASS(ResourceManager)::configure_uniform_buffer(const std::string& res_name, const std::string& name, uint32_t set, uint32_t binding, uint32_t count, VkDeviceSize size, const std::array<std::unordered_map<uint32_t, VkDescriptorSet>, k_Max_Frames_In_Flight>& descriptor_sets)
{
    auto it = m_uniform_buffers.find(res_name);
    if (it != m_uniform_buffers.end())
    {
        auto& uniform_buffers = it->second;
        for (size_t i = 0; i < k_Max_Frames_In_Flight; ++i)
        {
            uniform_buffers[i]->set_info(name, size, count);
            uniform_buffers[i]->set_uniform_infos(name, set, binding);
            uniform_buffers[i]->update(descriptor_sets[i]);
        }
    }
    else
    {
        std::array<std::shared_ptr<VK_CLASS(UniformBuffer)>, k_Max_Frames_In_Flight> uniform_buffers;

        for (size_t i = 0; i < k_Max_Frames_In_Flight; ++i)
        {
            uniform_buffers[i] = std::make_shared<VK_CLASS(UniformBuffer)>();
            uniform_buffers[i]->init();
            uniform_buffers[i]->set_info(name, size, count);
            uniform_buffers[i]->set_uniform_infos(name, set, binding);
            uniform_buffers[i]->update(descriptor_sets[i]);
        }
        m_uniform_buffers[res_name] = uniform_buffers;
    }
}


ENGINE_NAMESPACE_END
