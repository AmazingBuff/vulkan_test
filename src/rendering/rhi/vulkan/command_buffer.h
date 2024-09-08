#pragma once

#include "types.h"
#include "sync.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(CommandBuffer) final : public RHI
{
public:
	VK_CLASS(CommandBuffer)() = default;
	~VK_CLASS(CommandBuffer)() override;
	void initialize() override;
	// wait for previous frame
	void wait() const;
	// reset command buffer and fence
	void reset() const;
	void begin_record_command() const;
	void begin_render_pass(const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass, const std::shared_ptr<VK_CLASS(Framebuffer)>& framebuffer) const;
	void end_render_pass() const;
	void end_record_command() const;
	void bind_pipeline(const std::shared_ptr<VK_CLASS(Pipeline)>&  pipeline) const;
	void draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) const;
	void submit() const;
	void refresh_frame();
	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_command_pool();
	void create_command_buffer();
	void create_sync_objects();
private:
	std::array<VkCommandBuffer, k_Max_Frames_In_Flight>								m_command_buffers{};
	std::array<std::shared_ptr<VK_CLASS(Semaphore)>, k_Max_Frames_In_Flight>		m_image_available_semaphores;
	std::array<std::shared_ptr<VK_CLASS(Semaphore)>, k_Max_Frames_In_Flight>		m_render_finished_semaphores;
	std::array<std::shared_ptr<VK_CLASS(Fence)>, k_Max_Frames_In_Flight>			m_in_flight_fences;

	uint32_t																		m_current_frame = 0;
private:
	VK_TYPE_INIT(VkCommandPool,														m_command_pool);

	friend class VK_CLASS(Device);
	friend class VK_CLASS(SwapChain);
};


ENGINE_NAMESPACE_END