#pragma once
#include "rendering/rhi/rhi.h"
#include "vulkan/vk.h"


RHI_NAMESPACE_BEGIN

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


RHI_NAMESPACE_END