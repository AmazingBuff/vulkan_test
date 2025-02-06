#pragma once

#include "base/macro.h"

ENGINE_NAMESPACE_BEGIN

struct ShaderResource;
class ShaderManager;
struct PipelineLayoutResource;
class PipelineLayoutManager;
struct RenderPassResource;
class RenderPassManager;
struct PipelineResource;
class PipelineManager;
struct TextureResource;
class TextureManager;
struct ModelResource;
class ModelManager;


class RenderResources final
{
public:
	RenderResources() = default;
	~RenderResources() = default;
	void initialize();
	NODISCARD const std::shared_ptr<ShaderResource>& get_shader_resource(const std::string_view& name) const;
    NODISCARD const std::shared_ptr<RenderPassResource>& get_render_pass_resource(const std::string_view& name) const;
    NODISCARD const std::shared_ptr<PipelineResource>& get_pipeline_resource(const std::string_view& name) const;
    NODISCARD const std::shared_ptr<PipelineLayoutResource>& get_pipeline_layout_resource(const std::string_view& name) const;
    NODISCARD const std::shared_ptr<TextureResource>& get_texture_resource(const std::string_view& name) const;
    NODISCARD const std::shared_ptr<ModelResource>& get_model_resource(const std::string_view& name) const;
private:
	std::shared_ptr<ShaderManager>				m_shader_manager;
	std::shared_ptr<PipelineLayoutManager>		m_pipeline_layout_manager;
	std::shared_ptr<RenderPassManager>			m_render_pass_manager;
	std::shared_ptr<PipelineManager>			m_pipeline_manager;
	std::shared_ptr<TextureManager>				m_texture_manager;
	std::shared_ptr<ModelManager>				m_model_manager;
};

ENGINE_NAMESPACE_END