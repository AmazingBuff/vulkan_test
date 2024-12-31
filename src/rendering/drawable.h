#pragma once

#include "rhi/interface.h"
#include "system/structure.h"

ENGINE_NAMESPACE_BEGIN

class Drawable
{
public:
	Drawable() = default;
	~Drawable();
	void initialize();
	void draw(GlobalRuntimeInfo& global_info);
public:
	// these can be fetched directly from the renderer
	std::shared_ptr<Instance>											m_instance;
	std::shared_ptr<PhysicalDevice>										m_physical_device;
	std::shared_ptr<Device>												m_device;
	std::shared_ptr<SwapChain>											m_swap_chain;
	std::shared_ptr<CommandBuffer>										m_command_buffer;
	std::shared_ptr<PipelineResources>									m_pipeline_resources;
private:
	void recreate_swap_chain(GlobalRuntimeInfo& global_info);

	// temporary
	NODISCARD const std::shared_ptr<RenderPass>& current_render_pass() const;
	NODISCARD const std::shared_ptr<Pipeline>& current_pipeline() const;
	NODISCARD const std::shared_ptr<PipelineLayout>& current_pipeline_layout() const;
	NODISCARD const std::shared_ptr<DescriptorSet>& current_descriptor_set() const;
	NODISCARD const std::shared_ptr<Buffer>& current_vertex_buffers() const;
	NODISCARD const std::shared_ptr<Buffer>& current_index_buffer() const;
	NODISCARD std::vector<BufferInfo> current_vertex_infos() const;
	NODISCARD BufferInfo current_index_info() const;
};


ENGINE_NAMESPACE_END