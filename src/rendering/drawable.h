#pragma once
#include "rendering/rhi/rhi.h"
#include "rhi/interface.h"


RENDERING_NAMESPACE_BEGIN

class Drawable
{
public:
	Drawable() = default;
	~Drawable() = default;
	void initialize();
public:
	std::shared_ptr<Instance>		m_instance;
	std::shared_ptr<PhysicalDevice> m_physical_device;
	std::shared_ptr<Device>			m_device;
	std::shared_ptr<SwapChain>		m_swap_chain;
};


RENDERING_NAMESPACE_END