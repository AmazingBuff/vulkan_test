#pragma once

#include "types.h"
#include "sync.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(CommandBuffer) final : public RHI
{
public:
	VK_CLASS(CommandBuffer)() = default;
	~VK_CLASS(CommandBuffer)() override;
	NODISCARD constexpr RHIFlag flag() const override
	{
		return RHIFlag::e_command_buffer;
	}

	void initialize();
	// wait for previous frame
	void wait() const;
	// reset command buffer and fence
	void reset() const;
	void begin_record_command() const;
	void begin_render_pass(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass, const std::shared_ptr<VK_CLASS(Framebuffer)>& framebuffer) const;
	void end_render_pass() const;
	void end_record_command() const;
	void bind_pipeline(const std::shared_ptr<VK_CLASS(Pipeline)>&  pipeline) const;
	void bind_vertex_buffers(uint32_t first_binding, uint32_t binding_count, const std::shared_ptr<VK_CLASS(Buffer)>& vertex_buffer, const std::vector<VkDeviceSize>& offsets) const;
	void bind_index_buffers(const std::shared_ptr<VK_CLASS(Buffer)>& index_buffer, VkDeviceSize offset) const;
	void bind_descriptor_sets(const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout, const std::shared_ptr<VK_CLASS(DescriptorSet)>& descriptor_set) const;
	void draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) const;
	void draw_indexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance) const;
	void submit() const;
	void refresh_frame();

	// single command buffer
	VkCommandBuffer begin_single_command();
	void end_single_command(VkCommandBuffer command_buffer);
private:
	void create_command_pool();
	void create_render_command_buffer();
	void create_sync_objects();
private:
	std::array<VkCommandBuffer, k_Max_Frames_In_Flight>								m_render_command_buffers{};
	std::array<std::shared_ptr<VK_CLASS(Semaphore)>, k_Max_Frames_In_Flight>		m_image_available_semaphores;
	std::array<std::shared_ptr<VK_CLASS(Semaphore)>, k_Max_Frames_In_Flight>		m_render_finished_semaphores;
	std::array<std::shared_ptr<VK_CLASS(Fence)>, k_Max_Frames_In_Flight>			m_in_flight_fences;

	uint32_t																		m_current_frame = 0;

    // this two queues are used to manage command buffers for single command
    std::queue<VkCommandBuffer>			                                            m_single_command_buffers;
private:
	VK_TYPE_INIT(VkCommandPool,														m_command_pool);

	friend class VK_CLASS(Device);
	friend class VK_CLASS(SwapChain);
	friend class VK_CLASS(Buffer);
	friend class VK_CLASS(PipelineResources);
};


ENGINE_NAMESPACE_END