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


VK_CLASS(Buffer)::~VK_CLASS(Buffer)()
{
	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
	vkDestroyBuffer(device, m_buffer, nullptr);
	vkFreeMemory(device, m_buffer_memory, nullptr);
}

void VK_CLASS(Buffer)::initialize()
{
}


constexpr RHIFlag VK_CLASS(Buffer)::flag() const
{
	return RHIFlag::e_buffer;
}


VK_CLASS(VertexBuffer)::~VK_CLASS(VertexBuffer)()
{
}

void VK_CLASS(VertexBuffer)::initialize()
{
	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;

	VkBufferCreateInfo buffer_info{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = sizeof(VK_CLASS(Vertex)) * m_vertices.size(),
		.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	VK_CHECK_RESULT(vkCreateBuffer(device, &buffer_info, nullptr, &m_buffer));

	VkMemoryRequirements mem_requirements;
	vkGetBufferMemoryRequirements(device, m_buffer, &mem_requirements);

	VkMemoryAllocateInfo alloc_info{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = mem_requirements.size,
		.memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	};

	VK_CHECK_RESULT(vkAllocateMemory(device, &alloc_info, nullptr, &m_buffer_memory));
	vkBindBufferMemory(device, m_buffer, m_buffer_memory, 0);

	void* data;
	vkMapMemory(device, m_buffer_memory, 0, buffer_info.size, 0, &data);
	memcpy(data, m_vertices.data(), buffer_info.size);
	vkUnmapMemory(device, m_buffer_memory);
}

constexpr RHIFlag VK_CLASS(VertexBuffer)::flag() const
{
	return RHIFlag::e_vertex_buffer;
}


ENGINE_NAMESPACE_END