#include "texture_manager.h"
#include "base/util.h"
#include "rendering/utils/util.h"
#include "rendering/resources/fork/stb_image.h"

ENGINE_NAMESPACE_BEGIN

#define TEXTURE_PATH SOURCES_DIR"/res/textures/"


TextureManager::~TextureManager()
{
	for (auto& resource : std::ranges::views::values(m_texture_resources))
	{
		if (resource.data)
			stbi_image_free(resource.data);
	}
}

void TextureManager::load_texture_files()
{
	for (auto& dir_entry : std::filesystem::directory_iterator{ TEXTURE_PATH })
	{
		if (!dir_entry.is_directory())
			continue;
		TextureResource resource;
		for (auto& dir : std::filesystem::directory_iterator{ dir_entry })
		{
			const std::string file_name = dir.path().filename().generic_string();
			resource.data = stbi_load(dir.path().generic_string().c_str(), &resource.width, &resource.height, &resource.channels, STBI_rgb_alpha);
			if (!resource.data)
				RENDERING_LOG_ERROR("failed to load texture file: " + file_name);
		}
		if (resource)
		{
			std::string dir_name = dir_entry.path().filename().generic_string();
			m_texture_resources.emplace(dir_name, resource);
		}
	}
}

void TextureManager::initialize()
{
	load_texture_files();
}

const TextureResource& TextureManager::get_texture_resource(const std::string_view& name)
{
	auto it = m_texture_resources.find(name.data());
	if (it == m_texture_resources.end())
		RENDERING_LOG_ERROR("texture resource not found: " + std::string(name));

	return it->second;
}

#undef SHADER_PATH

ENGINE_NAMESPACE_END