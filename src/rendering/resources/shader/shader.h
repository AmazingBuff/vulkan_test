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

ENGINE_NAMESPACE_END