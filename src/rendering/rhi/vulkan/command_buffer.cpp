#include "command_buffer.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"

RHI_NAMESPACE_BEGIN


VK_CLASS(CommandBuffer)::~VK_CLASS(CommandBuffer)()
{
	vkDestroyCommandPool(g_system_context->g_render_system->m_drawable->m_device->m_device, m_command_pool, nullptr);
}

void VK_CLASS(CommandBuffer)::begin_record_command() const
{
	VkCommandBufferBeginInfo begin_info{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		.pInheritanceInfo = nullptr
	};

	VK_CHECK_RESULT(vkBeginCommandBuffer(m_command_buffer, &begin_info));
}

void VK_CLASS(CommandBuffer)::initialize()
{
	create_command_pool();
	create_command_buffer();
}

void VK_CLASS(CommandBuffer)::create_command_pool()
{
	auto drawable = g_system_context->g_render_system->m_drawable;
	const VkCommandPoolCreateInfo create_info{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = drawable->m_physical_device->m_indices.graphics_family.value(),
	};

	VK_CHECK_RESULT(vkCreateCommandPool(drawable->m_device->m_device, &create_info, nullptr, &m_command_pool));
}

void VK_CLASS(CommandBuffer)::create_command_buffer()
{
	auto drawable = g_system_context->g_render_system->m_drawable;
	VkCommandBufferAllocateInfo allocate_info{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = m_command_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1
	};

	VK_CHECK_RESULT(vkAllocateCommandBuffers(drawable->m_device->m_device, &allocate_info, &m_command_buffer));
}

RHI_NAMESPACE_END