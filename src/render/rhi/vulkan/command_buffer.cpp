#include "command_buffer.h"
#include "system/system.h"
#include "render/renderer.h"
#include "render/drawable.h"
#include "benchmark/buffer.h"

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
	std::array<VkClearValue, 2> clear_color{};
	clear_color[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clear_color[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo begin_info{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = render_pass->m_render_pass,
		.framebuffer = framebuffer->m_frame_buffer,
		.renderArea = {
			.offset = {0, 0},
			.extent = g_system_context->g_render_system->m_drawable->m_swap_chain->m_details.extent.value()
		},
		.clearValueCount = static_cast<uint32_t>(clear_color.size()),
		.pClearValues = clear_color.data()
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

void VK_CLASS(CommandBuffer)::bind_vertex_buffers(uint32_t first_binding, uint32_t binding_count, const std::shared_ptr<VK_CLASS(Buffer)>& vertex_buffer, const std::vector<VkDeviceSize>& offsets) const
{
	std::vector<VkBuffer> buffers;
	for (uint32_t i = first_binding; i < first_binding + binding_count; ++i)
		buffers.push_back(vertex_buffer->m_buffer);

	vkCmdBindVertexBuffers(m_command_buffers[m_current_frame], 0, binding_count, buffers.data(), offsets.data());
}

void VK_CLASS(CommandBuffer)::bind_index_buffers(const std::shared_ptr<VK_CLASS(Buffer)>& index_buffer, VkDeviceSize offset) const
{
	vkCmdBindIndexBuffer(m_command_buffers[m_current_frame], index_buffer->m_buffer, offset, VK_INDEX_TYPE_UINT32);
}

void VK_CLASS(CommandBuffer)::bind_descriptor_sets(const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout, const std::shared_ptr<VK_CLASS(DescriptorSet)>& descriptor_set) const
{
	std::vector<VkDescriptorSet> descriptor_sets;
	uint32_t first_set = std::numeric_limits<uint32_t>::max();
	uint32_t set_count = 0;
	for (auto& [set, descriptor] : descriptor_set->m_descriptor_sets[m_current_frame])
	{
		first_set = std::min(first_set, set);
		descriptor_sets.push_back(descriptor);
		set_count++;
	}

	vkCmdBindDescriptorSets(m_command_buffers[m_current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout->m_pipeline_layout, first_set, set_count, descriptor_sets.data(), 0, nullptr);
}

void VK_CLASS(CommandBuffer)::draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) const
{
	vkCmdDraw(m_command_buffers[m_current_frame], vertex_count, instance_count, first_vertex, first_instance);
}

void VK_CLASS(CommandBuffer)::draw_indexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance) const
{
	vkCmdDrawIndexed(m_command_buffers[m_current_frame], index_count, instance_count, first_index, vertex_offset, first_instance);
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

VkCommandBuffer VK_CLASS(CommandBuffer)::begin_single_command() const
{
	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;

	VkCommandBufferAllocateInfo alloc_info{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = m_command_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};

	VkCommandBuffer command_buffer;
	VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &alloc_info, &command_buffer));

	VkCommandBufferBeginInfo begin_info{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
	};

	VK_CHECK_RESULT(vkBeginCommandBuffer(command_buffer, &begin_info));

	return command_buffer;
}

void VK_CLASS(CommandBuffer)::end_single_command(VkCommandBuffer command_buffer) const
{
	VK_CHECK_RESULT(vkEndCommandBuffer(command_buffer));

	VkSubmitInfo submit_info{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &command_buffer,
	};


	auto device = g_system_context->g_render_system->m_drawable->m_device;
	auto queue = device->m_graphics_queue;
	VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE));
	VK_CHECK_RESULT(vkQueueWaitIdle(queue));

	vkFreeCommandBuffers(device->m_device, m_command_pool, 1, &command_buffer);
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
	VkCommandBufferAllocateInfo allocate_info{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = m_command_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = k_Max_Frames_In_Flight
	};

	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
	VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &allocate_info, m_command_buffers.data()));
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