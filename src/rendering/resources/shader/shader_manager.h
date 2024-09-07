#pragma once

#include "shader.h"

ENGINE_NAMESPACE_BEGIN

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