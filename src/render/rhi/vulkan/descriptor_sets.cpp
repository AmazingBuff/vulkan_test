#include "descriptor_sets.h"
#include "system/system.h"
#include "render/renderer.h"
#include "render/drawable.h"

ENGINE_NAMESPACE_BEGIN

VK_CLASS(DescriptorSet)::~VK_CLASS(DescriptorSet)()
{
    auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
    vkDestroyDescriptorPool(device, m_descriptor_pool, nullptr);
}

void VK_CLASS(DescriptorSet)::initialize(const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout)
{
    uint32_t uniform_buffer_descriptor_count = k_Max_Frames_In_Flight * static_cast<uint32_t>(pipeline_layout->m_shader_resources_layout.uniform_buffers.size());
    uint32_t sampled_image_descriptor_count = k_Max_Frames_In_Flight * static_cast<uint32_t>(pipeline_layout->m_shader_resources_layout.sampled_images.size());

    std::vector<VkDescriptorPoolSize> pool_sizes{
        {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = uniform_buffer_descriptor_count
        },
        {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = sampled_image_descriptor_count
        }
    };

    uint32_t descriptor_set_count = k_Max_Frames_In_Flight * static_cast<uint32_t>(pipeline_layout->m_descriptor_set_layouts.size());
    VkDescriptorPoolCreateInfo pool_info{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = descriptor_set_count,
        .poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
        .pPoolSizes = pool_sizes.data(),
    };

    auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &pool_info, nullptr, &m_descriptor_pool));

    for (size_t i = 0; i < k_Max_Frames_In_Flight; ++i)
    {
        for (auto& [set, set_layout] : pipeline_layout->m_descriptor_set_layouts)
        {
            VkDescriptorSetAllocateInfo alloc_info{
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .descriptorPool = m_descriptor_pool,
                .descriptorSetCount = 1,
                .pSetLayouts = &set_layout,
            };

            VkDescriptorSet descriptor_set;
            VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &alloc_info, &descriptor_set));
            m_descriptor_sets[i][set] = descriptor_set;
        }
    }
}


ENGINE_NAMESPACE_END
