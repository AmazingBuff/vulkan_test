//
// Created by AmazingBuff on 2024/12/30.
//

#pragma once

#include "buffer.h"

ENGINE_NAMESPACE_BEGIN

struct MemoryRequirements
{
    VkDeviceSize alignment;
    uint32_t     memoryTypeBits;

    NODISCARD constexpr bool operator==(const MemoryRequirements& other) const
    {
        return alignment == other.alignment && memoryTypeBits == other.memoryTypeBits;
    }
};

ENGINE_NAMESPACE_END


template <>
struct std::hash<Amazing::Engine::MemoryRequirements>
{
    size_t operator()(const Amazing::Engine::MemoryRequirements& memory_requirements) const noexcept
    {
        size_t seed = 0;
        seed = Amazing::hash_combine(seed, memory_requirements.alignment);
        seed = Amazing::hash_combine(seed, static_cast<size_t>(memory_requirements.memoryTypeBits));
        return seed;
    }
};


ENGINE_NAMESPACE_BEGIN

struct TextureResource;
struct SampledImageLayout;


class VK_CLASS(Image) final : public VK_CLASS(IBuffer)
{
public:
    static void transition_image_layout(VkImage image, VkFormat format, VkImageLayout old_layout,
                                        VkImageLayout new_layout, uint32_t mip_levels);
    static void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    static void generate_mipmaps(VkImage image, VkFormat format, int32_t width, int32_t height, uint32_t mip_levels);

public:
    VK_CLASS(Image)() = default;
    ~VK_CLASS(Image)() override;
    NODISCARD constexpr RHIFlag flag() const override { return RHIFlag::e_image; }

    void initialize(MemoryRequirements memory_requirements);
    void map_memory(const std::string& name, VkImage image, const std::shared_ptr<TextureResource>& resource,
                    const std::shared_ptr<VK_CLASS(Buffer)>& src_buffer, uint32_t mip_levels);
    void update_descriptor_set(const std::vector<SampledImageLayout>&               layouts,
                               const std::unordered_map<std::string, std::string>&  name_to_res_name_map,
                               const std::unordered_map<uint32_t, VkDescriptorSet>& descriptor_sets,
                               const std::shared_ptr<VK_CLASS(Sampler)>&            sampler);
    void release_sampler(const std::shared_ptr<VK_CLASS(Sampler)>& sampler);

private:
    struct Image
    {
        VK_TYPE_INIT(VkImage, image);
        VK_TYPE_INIT(VkImageView, image_view);
        uint32_t mip_levels;
    };

    std::unordered_map<std::string, Image> m_images;
};


class VK_CLASS(Sampler) final : public RHI
{
public:
    VK_CLASS(Sampler)() = default;
    ~VK_CLASS(Sampler)() override;
    NODISCARD constexpr RHIFlag flag() const override { return RHIFlag::e_sampler; }

    void                initialize();
    NODISCARD VkSampler get_sampler(VkImage image, uint32_t mip_levels);
    void                recycle_sampler(VkImage image);

private:
    void create_sampler(uint32_t mip_levels);

private:
    std::queue<VkSampler>                  m_available_queue;
    std::unordered_map<VkImage, VkSampler> m_occupied_map;
};


ENGINE_NAMESPACE_END
