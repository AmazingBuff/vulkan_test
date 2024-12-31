#include "buffer.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"

ENGINE_NAMESPACE_BEGIN

static uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties)
{
	auto& mem_properties = g_system_context->g_render_system->m_drawable->m_physical_device->m_memory_properties;
	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
	{
		if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	RENDERING_LOG_ERROR("failed to find suitable memory type!");
	return std::numeric_limits<uint32_t>::max();
}


void VK_CLASS(IBuffer)::set_info(const std::string& name, VkDeviceSize size, uint32_t count)
{
	if (m_current_offset + size > m_allocate_size)
		RENDERING_LOG_ERROR("allocation size can't fill so much data!");

	m_infos[name] = {m_current_offset, size, count};
	m_current_offset += aligned_size(size, g_system_context->g_render_system->m_drawable->m_physical_device->m_properties.limits.minUniformBufferOffsetAlignment);
}

const BufferInfo& VK_CLASS(IBuffer)::get_info(const std::string_view& name) const
{
	auto it = m_infos.find(name.data());
	if (it == m_infos.end())
		RENDERING_LOG_ERROR("failed to find buffer info!");
	return it->second;
}

void VK_CLASS(IBuffer)::clear_up_buffer()
{
	m_infos.clear();
	m_current_offset = 0;
}


VK_CLASS(Buffer)::~VK_CLASS(Buffer)()
{
	auto allocator = g_system_context->g_render_system->m_drawable->m_device->m_allocator;
	vmaDestroyBuffer(allocator, m_buffer, m_allocation);
}

void VK_CLASS(Buffer)::initialize(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	auto allocator = g_system_context->g_render_system->m_drawable->m_device->m_allocator;

	VkBufferCreateInfo buffer_info{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	VmaAllocationCreateInfo alloc_create_info{
		.usage = VMA_MEMORY_USAGE_AUTO,
	};

	if (buffer_info.usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
		alloc_create_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

	if (buffer_info.usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
		alloc_create_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

	VmaAllocationInfo alloc_info;
	VK_CHECK_RESULT(vmaCreateBuffer(allocator, &buffer_info, &alloc_create_info, &m_buffer, &m_allocation, &alloc_info));
	m_allocate_size = alloc_info.size;
}

void VK_CLASS(Buffer)::map_memory(const void* src_data, VkDeviceSize size, VkDeviceSize dst_offset) const
{
	auto allocator = g_system_context->g_render_system->m_drawable->m_device->m_allocator;

	VmaAllocationInfo alloc_info;
	vmaGetAllocationInfo(allocator, m_allocation, &alloc_info);

	VkMemoryPropertyFlags properties;
	vmaGetAllocationMemoryProperties(allocator, m_allocation, &properties);

	if (properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		memcpy(static_cast<uint8_t*>(alloc_info.pMappedData) + dst_offset, src_data, size);
	else
	{
		auto staging_buffer = std::make_shared<VK_CLASS(Buffer)>();
		staging_buffer->init(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		staging_buffer->map_memory(src_data, size, dst_offset);

		copy_buffer_from(staging_buffer, 0, dst_offset, size);
	}
}

void VK_CLASS(Buffer)::copy_buffer_from(const std::shared_ptr<VK_CLASS(Buffer)>& src_buffer, VkDeviceSize src_offset, VkDeviceSize dst_offset, VkDeviceSize size) const
{
	auto command_buffers = g_system_context->g_render_system->m_drawable->m_command_buffer;

	VkCommandBuffer command_buffer = command_buffers->begin_single_command();

	VkBufferCopy copy_region{
	    .srcOffset = src_offset,
	    .dstOffset = dst_offset,
	    .size = size,
	};
	vkCmdCopyBuffer(command_buffer, src_buffer->m_buffer, m_buffer, 1, &copy_region);

	command_buffers->end_single_command(command_buffer);
}


void VK_CLASS(UniformBuffer)::initialize(VkDeviceSize size)
{
	VK_CLASS(Buffer)::initialize(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void VK_CLASS(UniformBuffer)::set_uniform_infos(const std::string& name, uint32_t set, uint32_t binding)
{
	m_uniform_infos[name] = {set, binding};
}

void VK_CLASS(UniformBuffer)::update(const std::unordered_map<uint32_t, VkDescriptorSet>& descriptor_sets) const
{
	for (auto& [name, info] : m_infos)
	{
		VkDescriptorBufferInfo bufferInfo{
			.buffer = m_buffer,
			.offset = info.offset,
			.range = info.size,
		};

		const UniformBufferInfo& ubo_info = m_uniform_infos.at(name);
		VkWriteDescriptorSet writeInfo{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = descriptor_sets.at(ubo_info.set),
			.dstBinding = ubo_info.binding,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.pImageInfo = nullptr,
			.pBufferInfo = &bufferInfo,
			.pTexelBufferView = nullptr,
		};

		auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
		vkUpdateDescriptorSets(device, 1, &writeInfo, 0, nullptr);
	}
}


ENGINE_NAMESPACE_END