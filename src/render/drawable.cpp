#include "drawable.h"
#include "window/window.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

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

	m_pipeline_resources = std::make_shared<PipelineResources>();
	m_pipeline_resources->init();

	m_swap_chain->create_frame_buffers(current_render_pass());
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
	m_command_buffer->begin_render_pass(current_render_pass(), m_swap_chain->current_frame_buffer());
	m_command_buffer->bind_pipeline(current_pipeline());

	m_pipeline_resources->update_uniform_buffer("basic", global_info);
	std::vector<VkDeviceSize> offsets;
	for (auto& info : current_vertex_infos())
		offsets.emplace_back(info.offset);

	m_command_buffer->bind_vertex_buffers(0, 1, current_vertex_buffers(), offsets);
	m_command_buffer->bind_index_buffers(current_index_buffer(), current_index_info().offset);
	m_command_buffer->bind_descriptor_sets(current_pipeline_layout(), current_descriptor_set());
	m_command_buffer->draw_indexed(current_index_info().count, 1, 0, 0, 0);
	//m_command_buffer->draw(static_cast<uint32_t>(current_vertex_buffers()[0]->size()), 1, 0, 0);
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
	m_swap_chain->init(current_render_pass());
}

const std::shared_ptr<RenderPass>& Drawable::current_render_pass() const
{
	return m_pipeline_resources->get_render_pass("basic");
}

const std::shared_ptr<Pipeline>& Drawable::current_pipeline() const
{
	return m_pipeline_resources->get_pipeline("basic");
}

const std::shared_ptr<PipelineLayout>& Drawable::current_pipeline_layout() const
{
	return m_pipeline_resources->get_pipeline_layout("basic");
}

const std::shared_ptr<DescriptorSet>& Drawable::current_descriptor_set() const
{
	return m_pipeline_resources->get_descriptor_set("basic");

}

const std::shared_ptr<Buffer>& Drawable::current_vertex_buffers() const
{
	return m_pipeline_resources->get_vertex_buffer();
}

const std::shared_ptr<Buffer>& Drawable::current_index_buffer() const
{
	return m_pipeline_resources->get_index_buffer();
}

std::vector<BufferInfo> Drawable::current_vertex_infos() const
{
	std::vector<BufferInfo> infos;
	infos.emplace_back(m_pipeline_resources->get_vertex_info("basic"));
	return infos;
}

BufferInfo Drawable::current_index_info() const
{
	return m_pipeline_resources->get_index_info("basic");
}

ENGINE_NAMESPACE_END