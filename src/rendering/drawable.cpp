#include "drawable.h"

ENGINE_NAMESPACE_BEGIN

void Drawable::initialize()
{
	m_instance = std::make_shared<Instance>();
	m_instance->initialize();

	m_physical_device = std::make_shared<PhysicalDevice>();
	m_physical_device->initialize();

	m_device = std::make_shared<Device>();
	m_device->initialize();

	m_command_buffer = std::make_shared<CommandBuffer>();
	m_command_buffer->initialize();

	m_swap_chain = std::make_shared<SwapChain>();
	m_swap_chain->initialize();

	m_pipeline_layout = std::make_shared<PipelineLayout>();
	m_pipeline_layout->initialize();

	m_render_pass = std::make_shared<RenderPass>();
	m_render_pass->initialize();

	m_swap_chain->create_frame_buffers(m_render_pass);

	m_pipeline = std::make_shared<Pipeline>();
	m_pipeline->set_pipeline_layout(m_pipeline_layout);
	m_pipeline->set_render_pass(m_render_pass);
	m_pipeline->initialize();
}

void Drawable::draw() const
{
	// wait for previous frame
	m_command_buffer->wait();
	// acquire next image
	m_swap_chain->acquire_next_image();

	m_command_buffer->reset();
	// begin record command
	m_command_buffer->begin_record_command();
	m_command_buffer->begin_render_pass(m_render_pass, m_swap_chain->current_frame_buffer());
	m_command_buffer->bind_pipeline(m_pipeline);
	m_command_buffer->draw(3, 1, 0, 0);
	m_command_buffer->end_render_pass();
	m_command_buffer->end_record_command();

	// submit command
	m_command_buffer->submit();

	// present
	m_device->present();

	m_command_buffer->refresh_frame();
}

void Drawable::wait_idle() const
{
	m_device->wait_idle();
}

ENGINE_NAMESPACE_END