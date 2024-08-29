#pragma once

#include "render_types.h"

RENDERING_NAMESPACE_BEGIN

struct ShaderResource
{
	std::string vertex_shader;
	std::string fragment_shader;
	// more for geometry shader, tessellation shader, compute shader, etc.
};


class RenderResources
{
public:
	RenderResources() = default;
	~RenderResources() = default;
	void initialize();
	NODISCARD ShaderResource& get_shader_resource(const std::string& name);

private:
	std::unordered_map<std::string, ShaderResource>		m_shader_resources;
};

RENDERING_NAMESPACE_END