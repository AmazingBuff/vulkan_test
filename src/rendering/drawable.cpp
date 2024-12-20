#include "drawable.h"
#include "window/window.h"

ENGINE_NAMESPACE_BEGIN

Drawable::~Drawable()
{
	m_device->wait_idle();
}

void Drawable::initialize()
{
	m_instance = std::make_shared<Instance>();
	m_instance->init();

	m_physical_device = std::make_shared<PhysicalDevice>();
	m_physical_device->init();

	m_device = std::make_shared<Device>();
	m_device->init();

	m_command_buffer = std::make_shared<CommandBuffer>();
	m_command_buffer->init();

	m_swap_chain = std::make_shared<SwapChain>();
	m_swap_chain->init();

	m_pipeline_layout = std::make_shared<PipelineLayout>();
	m_pipeline_layout->init();

	m_render_pass = std::make_shared<RenderPass>();
	m_render_pass->init("basic");

	m_swap_chain->create_frame_buffers(m_render_pass);

	m_pipeline = std::make_shared<Pipeline>();
	m_pipeline->init(m_pipeline_layout, m_render_pass);
}

void Drawable::draw(GlobalRuntimeInfo& global_info)
{
	// wait for previous frame
	m_command_buffer->wait();
	// acquire next image
	if (!m_swap_chain->acquire_next_image())
	{
		recreate_swap_chain(global_info);
		return;
	}

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
	if(!m_device->present() || global_info.window_resized)
		recreate_swap_chain(global_info);
	
	// refresh frame
	m_command_buffer->refresh_frame();
}

void Drawable::recreate_swap_chain(GlobalRuntimeInfo& global_info)
{
	// minimization
	SDL_Event event;
	while (global_info.window_minimized)
	{
		SDL_WaitEvent(&event);
		if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESTORED)
			global_info.window_minimized = false;
	}

	m_device->wait_idle();
	m_swap_chain = std::make_shared<SwapChain>();
	m_swap_chain->init();
	m_swap_chain->create_frame_buffers(m_render_pass);
}

ENGINE_NAMESPACE_END