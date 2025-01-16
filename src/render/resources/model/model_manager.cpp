#include "model_manager.h"
#include "base/util.h"
#include "render/utils/util.h"
#include "render/resources/fork/stb_image.h"

//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>

ENGINE_NAMESPACE_BEGIN

#define MODEL_PATH SOURCES_DIR"/res/textures/"


ModelManager::~ModelManager()
{
	for (auto& resource : std::views::values(m_model_resources))
	{
		if (resource.data)
			stbi_image_free(resource.data);
	}
}

void ModelManager::load_model_files()
{
	for (auto& dir_entry : std::filesystem::directory_iterator{ MODEL_PATH })
	{
        /*Assimp::Importer importer;
        const aiScene* scene =  importer.ReadFile(dir_entry.path().generic_string(), 0);
		scene->mNumMeshes;*/

		const std::string file_name = dir_entry.path().filename().generic_string();
		if (file_name.find(".png") == std::string::npos)
			continue;

		ModelResource resource;
		resource.data = stbi_load(dir_entry.path().generic_string().c_str(), &resource.width, &resource.height, &resource.channels, STBI_rgb_alpha);
        resource.channels = 4;
		if (!resource.data)
			RENDERING_LOG_ERROR("failed to load texture file: " + file_name);

		if (resource)
		{
			const std::string name = file_name.substr(0, file_name.find(".png"));
			m_model_resources.emplace(name, resource);
		}
	}
}

void ModelManager::initialize()
{
	load_model_files();
}

const ModelResource& ModelManager::get_model_resource(const std::string_view& name)
{
	auto it = m_model_resources.find(name.data());
	if (it == m_model_resources.end())
		RENDERING_LOG_ERROR("model resource not found: " + std::string(name));

	return it->second;
}

#undef MODEL_PATH

ENGINE_NAMESPACE_END