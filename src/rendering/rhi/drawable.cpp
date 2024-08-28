#include "drawable.h"

RHI_NAMESPACE_BEGIN

void Drawable::initialize()
{
	m_instance = std::make_shared<Instance>();
	m_instance->initialize();
	m_physical_device = std::make_shared<PhysicalDevice>();
	m_physical_device->initialize();
	m_device = std::make_shared<Device>();
	m_device->initialize();
	m_swap_chain = std::make_shared<SwapChain>();
	m_swap_chain->initialize();
}

RHI_NAMESPACE_END