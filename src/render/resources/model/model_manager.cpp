#include "model_manager.h"
#include "base/util.h"
#include "render/utils/util.h"
#include "render/resources/fork/stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

ENGINE_NAMESPACE_BEGIN

#define MODEL_PATH SOURCES_DIR"/asset/obj/"


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
		for (auto& file : std::filesystem::directory_iterator{ dir_entry })
		{
			const std::string file_name = file.path().filename().generic_string();
			if (file_name.find(".obj") == std::string::npos)
				continue;

			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(file.path().generic_string(), 0);
			for (uint32_t i = 0; i < scene->mNumMeshes; i++)
			{
				const aiMesh* mesh = scene->mMeshes[i];
				ModelResource resource;
				resource.vertices.resize(mesh->mNumVertices);

				if (mesh->HasPositions())
				{
					for (uint32_t j = 0; j < mesh->mNumVertices; j++)
					{
						resource.vertices[j].position.x() = mesh->mVertices[j].x;
						resource.vertices[j].position.y() = mesh->mVertices[j].y;
						resource.vertices[j].position.z() = mesh->mVertices[j].z;
					}
				}


                if (mesh->HasTextureCoords(0))
                {
                    for (uint32_t j = 0; j < mesh->mNumVertices; j++)
                    {
                        resource.vertices[j].texcoord.x() = mesh->mTextureCoords[0][j].x;
                        resource.vertices[j].texcoord.y() = mesh->mTextureCoords[0][j].y;
                    }
                }

				if (mesh->HasNormals())
				{
					for (uint32_t j = 0; j < mesh->mNumVertices; j++)
					{
						resource.vertices[j].normal.x() = mesh->mNormals[j].x;
						resource.vertices[j].normal.y() = mesh->mNormals[j].y;
						resource.vertices[j].normal.z() = mesh->mNormals[j].z;
					}
				}
			}
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