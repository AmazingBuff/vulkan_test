//
// Created by AmazingBuff on 2024/12/27.
//

#pragma once

#include "buffer.h"
#include "image.h"
#include "model.h"

#include <func.h>

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(ResourceManager) : public RHI
{
public:
    VK_CLASS(ResourceManager)() = default;
    ~VK_CLASS(ResourceManager)() override;
    NODISCARD constexpr RHIFlag flag() const override { return RHIFlag::e_resource_manager; }

    void initialize(VkDeviceSize buffer_size = Default_Buffer_Size);

    // res_name is the name of current pipeline layout that the uniform buffer is associated with
    void configure_uniform_buffer(
        const std::string& res_name, const std::vector<UniformBufferLayout>& layouts,
        const std::array<std::unordered_map<uint32_t, VkDescriptorSet>, k_Max_Frames_In_Flight>& descriptor_sets);

    // res_name is the name of the texture resource that the image is associated with
    void create_image(const std::string& res_name, const std::shared_ptr<TextureResource>& resource, bool mipmap);

    void configure_image(
        const std::vector<SampledImageLayout>&                                                   layouts,
        const std::unordered_map<std::string, std::string>&                                      name_to_res_name_map,
        const std::array<std::unordered_map<uint32_t, VkDescriptorSet>, k_Max_Frames_In_Flight>& descriptor_sets);

    void map_model(const std::string& name, const std::shared_ptr<ModelResource>& resource, bool mipmap);

    template <typename Ty>
        requires(std::is_object_v<Ty> || Trait::is_same_template_v<Ty, std::vector<void>>)
    // res_name is the name of current pipeline layout that the uniform buffer is associated with
    void map_uniform_buffer(const std::string& res_name, const std::string& name, uint32_t current_image,
                            const Ty& ubo) const;

private:
    std::shared_ptr<VK_CLASS(Buffer)> m_staging_buffer;
    std::shared_ptr<VK_CLASS(Buffer)> m_vertex_buffer;
    std::shared_ptr<VK_CLASS(Buffer)> m_index_buffer;
    std::unordered_map<std::string, std::array<std::shared_ptr<VK_CLASS(UniformBuffer)>, k_Max_Frames_In_Flight>>
                                                                             m_uniform_buffers;
    std::unordered_map<MemoryRequirements, std::shared_ptr<VK_CLASS(Image)>> m_images;
    std::shared_ptr<VK_CLASS(Sampler)>                                       m_samplers;

    friend class VK_CLASS(PipelineResources);
};


template <typename Ty>
    requires(std::is_object_v<Ty> || Trait::is_same_template_v<Ty, std::vector<void>>)
void VK_CLASS(ResourceManager)::map_uniform_buffer(const std::string& res_name, const std::string& name,
                                                   uint32_t current_image, const Ty& ubo) const
{
    auto& info = m_uniform_buffers.at(res_name)[current_image]->get_info(name);
    if constexpr (Trait::is_same_template_v<Ty, std::vector<void>>)
    {
        VkDeviceSize buffer_size = sizeof(Trait::head_type_t<typename Trait::template_traits<Ty>::type>) * ubo.size();
        m_uniform_buffers.at(res_name)[current_image]->map_memory(ubo.data(), buffer_size, info.offset);
    }
    else
    {
        VkDeviceSize buffer_size = sizeof(Ty);
        m_uniform_buffers.at(res_name)[current_image]->map_memory(&ubo, buffer_size, info.offset);
    }
}


ENGINE_NAMESPACE_END
