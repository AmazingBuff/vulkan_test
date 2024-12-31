#pragma once

#include "render_types.h"
#include "resources/resource_types.h"

ENGINE_NAMESPACE_BEGIN

class RenderResources final
{
public:
	RenderResources() = default;
	~RenderResources() = default;
	void initialize();
	NODISCARD const ShaderResource& get_shader_resource(const std::string_view& name) const;
    NODISCARD const RenderPassResource& get_render_pass_resource(const std::string_view& name) const;
    NODISCARD const PipelineResource& get_pipeline_resource(const std::string_view& name) const;
    NODISCARD const PipelineLayoutResource& get_pipeline_layout_resource(const std::string_view& name) const;
private:
	std::shared_ptr<ShaderManager>				m_shader_manager;
	std::shared_ptr<PipelineLayoutManager>		m_pipeline_layout_manager;
	std::shared_ptr<RenderPassManager>			m_render_pass_manager;
	std::shared_ptr<PipelineManager>			m_pipeline_manager;
};

ENGINE_NAMESPACE_END