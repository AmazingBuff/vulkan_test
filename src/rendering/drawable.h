#pragma once
#include "rendering/rhi/rhi.h"
#include "rhi/interface.h"

ENGINE_NAMESPACE_BEGIN

struct RHIRenderInfos
{
	bool framebuffer_resized = false;
};



class Drawable
{
public:
	Drawable() = default;
	~Drawable();
	void initialize();
	void draw(const RHIRenderInfos& info);
public:
	// these can be fecthed diectly from the renderer
	std::shared_ptr<Instance>											m_instance;
	std::shared_ptr<PhysicalDevice>										m_physical_device;
	std::shared_ptr<Device>												m_device;
	std::shared_ptr<SwapChain>											m_swap_chain;
	std::shared_ptr<CommandBuffer>										m_command_buffer;


	// a temporary solution
	std::shared_ptr<PipelineLayout>										m_pipeline_layout;
	std::shared_ptr<RenderPass>											m_render_pass;	
	std::shared_ptr<Pipeline>											m_pipeline;
	//std::unordered_map<std::string, std::shared_ptr<Pipeline>>			m_pipelines;
private:
	void recreate_swap_chain();
};


ENGINE_NAMESPACE_END