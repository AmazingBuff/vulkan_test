#pragma once

#include "types.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(DescriptorSet) final : public RHI
{
public:
    VK_CLASS(DescriptorSet)() = default;
    ~VK_CLASS(DescriptorSet)() override;
    NODISCARD constexpr RHIFlag flag() const override
    {
        return RHIFlag::e_descriptor_set;
    }

    void initialize(const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout);
private:
    VK_TYPE_INIT(VkDescriptorPool,                                                          m_descriptor_pool);
    // set <---> descriptor set
    std::array<std::unordered_map<uint32_t, VkDescriptorSet>, k_Max_Frames_In_Flight>       m_descriptor_sets;

    friend class VK_CLASS(CommandBuffer);
    friend class VK_CLASS(PipelineResources);
};



ENGINE_NAMESPACE_END