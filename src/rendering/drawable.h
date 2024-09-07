#pragma once
#include "rendering/rhi/rhi.h"
#include "rhi/interface.h"


ENGINE_NAMESPACE_BEGIN

class Drawable
{
public:
	Drawable() = default;
	~Drawable() = default;
	void initialize();
public:
	std::shared_ptr<Instance>											m_instance;
	std::shared_ptr<PhysicalDevice>										m_physical_device;
	std::shared_ptr<Device>												m_device;
	std::shared_ptr<SwapChain>											m_swap_chain;
	// a temporary solution
	std::shared_ptr<Pipeline>											m_pipeline;
	//std::unordered_map<std::string, std::shared_ptr<Pipeline>>			m_pipelines;
	std::shared_ptr<CommandBuffer>										m_command_buffer;
};


ENGINE_NAMESPACE_END