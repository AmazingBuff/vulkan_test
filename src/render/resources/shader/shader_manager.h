#pragma once

#include "base/macro.h"

ENGINE_NAMESPACE_BEGIN

struct ShaderResource;

class ShaderManager final
{
public:
	ShaderManager() = default;
	~ShaderManager() = default;

	void initialize();
    NODISCARD const std::shared_ptr<ShaderResource>& get_shader_resource(const std::string_view& name);
private:
	void load_shader_files();
private:
    std::unordered_map<std::string, std::shared_ptr<ShaderResource>> m_shader_resources;
};

ENGINE_NAMESPACE_END