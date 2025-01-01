//
// Created by AmazingBuff on 2024/12/30.
//

#pragma once

#include "buffer.h"

ENGINE_NAMESPACE_BEGIN

struct TextureResource;
struct SampledImageLayout;


class VK_CLASS(Image) final : public VK_CLASS(IBuffer)
{
public:
    VK_CLASS(Image)() = default;
    ~VK_CLASS(Image)() override;
    NODISCARD constexpr RHIFlag flag() const override
    {
        return RHIFlag::e_image;
    }

    void initialize(const std::shared_ptr<VK_CLASS(Sampler)>& sampler);
    void create_image(const std::string& name, const TextureResource& resource, const std::shared_ptr<VK_CLASS(Buffer)>& src_buffer);
    void create_sampler(const std::string& name);

    void update_descriptor_set(const std::vector<SampledImageLayout>& layouts, const std::unordered_map<std::string, std::string>& name_to_res_name_map, const std::unordered_map<uint32_t, VkDescriptorSet>& descriptor_sets);
private:
    static void transition_image_layout(VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout);
    static void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
private:
    struct Image
    {
        VK_TYPE_INIT(VkImage, image);
        VK_TYPE_INIT(VkImageView, image_view);
    };

    std::unordered_map<std::string, Image>              m_images;
    std::shared_ptr<VK_CLASS(Sampler)>                  m_sampler;
    std::vector<VkSampler>                              m_occupied_samplers;
};


class VK_CLASS(Sampler) final : public RHI
{
public:
    VK_CLASS(Sampler)() = default;
    ~VK_CLASS(Sampler)() override;
    NODISCARD constexpr RHIFlag flag() const override
    {
        return RHIFlag::e_sampler;
    }

    void initialize();
    NODISCARD VkSampler get_sampler();
    void release_sampler(VkSampler sampler);
private:
    void create_sampler();
private:
    struct Sampler
    {
        VK_TYPE_INIT(VkSampler,     sampler);
        bool                        is_available;
    };

    std::queue<Sampler>           m_available_queue;
    std::list<Sampler>            m_occupied_list;
};


ENGINE_NAMESPACE_END