#include "shader_manager.h"
#include "base/util.h"
#include "render/utils/util.h"
#include "render/resources/resource_types.h"

ENGINE_NAMESPACE_BEGIN

#define SHADER_PATH SOURCES_DIR"/res/shaders/"

enum class ShaderType : uint8_t
{
	e_vertex,
	e_fragment,
	e_geometry,
	e_tessellation_control,
	e_tessellation_evaluation,
	e_compute
};

static const std::unordered_map<std::string, ShaderType> ShaderTypeMap =
{
	{ "vert.spv", ShaderType::e_vertex },
	{ "frag.spv", ShaderType::e_fragment },
	{ "geom.spv", ShaderType::e_geometry },
	{ "tessellation_control", ShaderType::e_tessellation_control },
	{ "tessellation_evaluation", ShaderType::e_tessellation_evaluation },
	{ "comp.spv", ShaderType::e_compute }
};

void ShaderManager::load_shader_files()
{
	for (auto& dir_entry : std::filesystem::directory_iterator{ SHADER_PATH })
	{
		if (!dir_entry.is_directory())
			continue;
        std::shared_ptr<ShaderResource> resources = std::make_shared<ShaderResource>();
		for (auto& dir : std::filesystem::directory_iterator{ dir_entry })
		{
			const std::string file_name = dir.path().filename().generic_string();
			auto it = ShaderTypeMap.find(file_name);
			if (it == ShaderTypeMap.end())
				continue;
			auto shader = read_file(dir.path().generic_string());
			switch (it->second)
			{
			case ShaderType::e_vertex:
				resources->vertex_shader = std::move(shader);
				break;
			case ShaderType::e_fragment:
				resources->fragment_shader = std::move(shader);
				break;
			default:
				break;
			}
		}
		if (*resources)
		{
			std::string dir_name = dir_entry.path().filename().generic_string();
			m_shader_resources.emplace(dir_name, resources);
		}
	}
}

void ShaderManager::initialize()
{
	load_shader_files();
}

const std::shared_ptr<ShaderResource>& ShaderManager::get_shader_resource(const std::string_view& name)
{
	auto it = m_shader_resources.find(name.data());
	if (it == m_shader_resources.end())
		RENDERING_LOG_ERROR("shader resource not found: " + std::string(name));

	return it->second;
}

#undef SHADER_PATH

ENGINE_NAMESPACE_END