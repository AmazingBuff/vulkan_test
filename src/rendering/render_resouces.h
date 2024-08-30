#pragma once

#include "render_types.h"
#include "resources/resource.h"

RENDERING_NAMESPACE_BEGIN

class RenderResources final
{
public:
	RenderResources() = default;
	~RenderResources() = default;
	void initialize();
	NODISCARD const ShaderResource& get_shader_resource(const std::string& name);
private:
	std::unique_ptr<ShaderManager>		m_shader_manager;
};

RENDERING_NAMESPACE_END