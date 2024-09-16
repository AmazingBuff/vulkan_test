#pragma once

#include "rendering/resources/resource_types.h"

ENGINE_NAMESPACE_BEGIN

struct ShaderResource
{
	std::vector<char> vertex_shader;
	std::vector<char> fragment_shader;
	// more for geometry shader, tessellation shader, compute shader, etc.

	NODISCARD constexpr operator bool() const
	{
		return !vertex_shader.empty() && !fragment_shader.empty();
	}
};


class ShaderManager final
{
public:
	ShaderManager() = default;
	~ShaderManager() = default;

	void initialize();
	NODISCARD const ShaderResource& get_shader_resource(const std::string& name);
private:
	void load_shader_files();
private:
	std::unordered_map<std::string, ShaderResource>		m_shader_resources;
};

ENGINE_NAMESPACE_END