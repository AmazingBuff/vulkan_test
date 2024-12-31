//
// Created by AmazingBuff on 2024/12/30.
//

#pragma once

#include "buffer.h"

ENGINE_NAMESPACE_BEGIN

struct TextureResource;

struct Image
{
    VK_TYPE_INIT(VkImage,           image);
    VK_TYPE_INIT(VkImageView,       image_view);
    VK_TYPE_INIT(VkSampler,         sampler);
};

class VK_CLASS(Image) final : public VK_CLASS(IBuffer)
{
public:
    VK_CLASS(Image)() = default;
    ~VK_CLASS(Image)() override;
    NODISCARD constexpr RHIFlag flag() const override
    {
        return RHIFlag::e_image;
    }

    void initialize();
    void create_image(const std::string& name, const TextureResource& resource);
private:
    static void transition_image_layout(VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout);
    static void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
private:
    std::unordered_map<std::string, Image>  m_images;
};



ENGINE_NAMESPACE_END