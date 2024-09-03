#include "drawable.h"

RENDERING_NAMESPACE_BEGIN

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
	m_pipeline = std::make_shared<Pipeline>();
	m_pipeline->initialize();
}

RENDERING_NAMESPACE_END