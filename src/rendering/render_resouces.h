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
	NODISCARD const ShaderResource& get_shader_resource(const std::string& name);
private:
	std::shared_ptr<ShaderManager>		m_shader_manager;
};

ENGINE_NAMESPACE_END