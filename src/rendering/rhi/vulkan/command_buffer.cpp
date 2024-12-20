#include "command_buffer.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"

ENGINE_NAMESPACE_BEGIN


VK_CLASS(CommandBuffer)::~VK_CLASS(CommandBuffer)()
{
	vkDestroyCommandPool(g_system_context->g_render_system->m_drawable->m_device->m_device, m_command_pool, nullptr);
}

void VK_CLASS(CommandBuffer)::wait() const
{
	m_in_flight_fences[m_current_frame]->wait();
}

void VK_CLASS(CommandBuffer)::reset() const
{
	m_in_flight_fences[m_current_frame]->reset();
	VK_CHECK_RESULT(vkResetCommandBuffer(m_command_buffers[m_current_frame], 0));
}

void VK_CLASS(CommandBuffer)::begin_render_pass(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass, const std::shared_ptr<VK_CLASS(Framebuffer)>& framebuffer) const
{
	VkClearValue clear_color{
		.color = {0.0f, 0.0f, 0.0f, 1.0f}
	};
	VkRenderPassBeginInfo begin_info{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = render_pass->m_render_pass,
		.framebuffer = framebuffer->m_frame_buffer,
		.renderArea = {
			.offset = {0, 0},
			.extent = g_system_context->g_render_system->m_drawable->m_swap_chain->m_details.extent.value()
		},
		.clearValueCount = 1,
		.pClearValues = &clear_color
	};

	vkCmdBeginRenderPass(m_command_buffers[m_current_frame], &begin_info, VK_SUBPASS_CONTENTS_INLINE);
}

void VK_CLASS(CommandBuffer)::end_render_pass() const
{
	vkCmdEndRenderPass(m_command_buffers[m_current_frame]);
}

void VK_CLASS(CommandBuffer)::bind_pipeline(const std::shared_ptr<VK_CLASS(Pipeline)>& pipeline) const
{
	vkCmdBindPipeline(m_command_buffers[m_current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->m_pipeline);

	auto& extent = g_system_context->g_render_system->m_drawable->m_swap_chain->m_details.extent.value();
	VkViewport viewport{
		.x = 0.0f,
		.y = 0.0f,
		.width = static_cast<float>(extent.width),
		.height = static_cast<float>(extent.height),
		.minDepth = 0.0f,
		.maxDepth = 1.0f
	};
	vkCmdSetViewport(m_command_buffers[m_current_frame], 0, 1, &viewport);

	VkRect2D scissor{
		.offset = {0, 0},
		.extent = extent
	};
	vkCmdSetScissor(m_command_buffers[m_current_frame], 0, 1, &scissor);
}

constexpr NODISCARD RHIFlag VK_CLASS(CommandBuffer)::flag() const
{
	return RHIFlag::e_command_buffer;
}

void VK_CLASS(CommandBuffer)::begin_record_command() const
{
	VkCommandBufferBeginInfo begin_info{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		.pInheritanceInfo = nullptr
	};

	VK_CHECK_RESULT(vkBeginCommandBuffer(m_command_buffers[m_current_frame], &begin_info));
}

void VK_CLASS(CommandBuffer)::end_record_command() const
{
	VK_CHECK_RESULT(vkEndCommandBuffer(m_command_buffers[m_current_frame]));
}

void VK_CLASS(CommandBuffer)::bind_vertex_buffers(const std::vector<std::shared_ptr<VK_CLASS(VertexBuffer)>>& vertex_buffers) const
{

}

void VK_CLASS(CommandBuffer)::draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) const
{
	vkCmdDraw(m_command_buffers[m_current_frame], vertex_count, instance_count, first_vertex, first_instance);
}

void VK_CLASS(CommandBuffer)::submit() const
{
	VkSemaphore wait_semaphores[] = { m_image_available_semaphores[m_current_frame]->m_semaphore};
	VkSemaphore signal_semaphores[] = { m_render_finished_semaphores[m_current_frame]->m_semaphore };
	VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submit_info{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = wait_semaphores,
		.pWaitDstStageMask = wait_stages,
		.commandBufferCount = 1,
		.pCommandBuffers = &m_command_buffers[m_current_frame],
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = signal_semaphores
	};

	VK_CHECK_RESULT(vkQueueSubmit(g_system_context->g_render_system->m_drawable->m_device->m_graphics_queue, 1, &submit_info, m_in_flight_fences[m_current_frame]->m_fence));
}

void VK_CLASS(CommandBuffer)::refresh_frame()
{
	m_current_frame = (m_current_frame + 1) % k_Max_Frames_In_Flight;
}

void VK_CLASS(CommandBuffer)::initialize()
{
	create_command_pool();
	create_command_buffer();
	create_sync_objects();
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
		.commandBufferCount = k_Max_Frames_In_Flight
	};

	VK_CHECK_RESULT(vkAllocateCommandBuffers(drawable->m_device->m_device, &allocate_info, m_command_buffers.data()));
}

void VK_CLASS(CommandBuffer)::create_sync_objects()
{
	for (uint32_t i = 0; i < k_Max_Frames_In_Flight; i++)
	{
		m_image_available_semaphores[i] = std::make_shared<VK_CLASS(Semaphore)>();
		m_image_available_semaphores[i]->initialize();
		m_render_finished_semaphores[i] = std::make_shared<VK_CLASS(Semaphore)>();
		m_render_finished_semaphores[i]->initialize();
		m_in_flight_fences[i] = std::make_shared<VK_CLASS(Fence)>();
		m_in_flight_fences[i]->initialize();
	}
}

ENGINE_NAMESPACE_END