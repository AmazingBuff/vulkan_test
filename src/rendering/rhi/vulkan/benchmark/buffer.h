#pragma once

#include "rendering/rhi/vulkan/types.h"
#include "rendering/rhi/vulkan/fork/vk_mem_alloc.h"
#include "rendering/resources/geometry/geometry_data.h"

ENGINE_NAMESPACE_BEGIN

struct BufferInfo
{
	VkDeviceSize	offset;
	VkDeviceSize	size;
	// for vertex and index buffer
	uint32_t		count;
};

class VK_CLASS(IBuffer) : public RHI
{
public:
	VK_CLASS(IBuffer)() = default;

	// generate offset via m_last_offset automatically
	void set_info(const std::string& name, VkDeviceSize size, uint32_t count);
	NODISCARD const BufferInfo& get_info(const std::string_view& name) const;

	void clear_up_buffer();
protected:
	VK_TYPE_INIT(VmaAllocation,						m_allocation);

	std::unordered_map<std::string, BufferInfo>		m_infos;
	VkDeviceSize									m_current_offset = 0;
	VkDeviceSize									m_allocate_size = 0;
};

class VK_CLASS(Buffer) : public VK_CLASS(IBuffer)
{
public:
	VK_CLASS(Buffer)() = default;
	~VK_CLASS(Buffer)() override;
	NODISCARD constexpr RHIFlag flag() const override
	{
		return RHIFlag::e_buffer;
	}

	void initialize(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	void map_memory(const void* src_data, VkDeviceSize size, VkDeviceSize dst_offset) const;
	void copy_buffer_from(const std::shared_ptr<VK_CLASS(Buffer)>& src_buffer, VkDeviceSize src_offset, VkDeviceSize dst_offset, VkDeviceSize size) const;
protected:
	VK_TYPE_INIT(VkBuffer,							m_buffer);

	friend class VK_CLASS(CommandBuffer);
};


class VK_CLASS(UniformBuffer) final : public VK_CLASS(Buffer)
{
public:
	VK_CLASS(UniformBuffer)() = default;
	~VK_CLASS(UniformBuffer)() override = default;

	void initialize(VkDeviceSize size = Default_Buffer_Size);
	void set_uniform_infos(const std::string& name, uint32_t set, uint32_t binding);
	void update(const std::unordered_map<uint32_t, VkDescriptorSet>& descriptor_sets) const;

private:
	struct UniformBufferInfo
	{
		// for uniform buffer
		uint32_t		set;
		uint32_t		binding;
	};
	std::unordered_map<std::string, UniformBufferInfo>		m_uniform_infos;
};


ENGINE_NAMESPACE_END