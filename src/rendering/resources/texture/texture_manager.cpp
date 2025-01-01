#include "texture_manager.h"
#include "base/util.h"
#include "rendering/utils/util.h"
#include "rendering/resources/fork/stb_image.h"

ENGINE_NAMESPACE_BEGIN

#define TEXTURE_PATH SOURCES_DIR"/res/textures/"


TextureManager::~TextureManager()
{
	for (auto& resource : std::views::values(m_texture_resources))
	{
		if (resource.data)
			stbi_image_free(resource.data);
	}
}

void TextureManager::load_texture_files()
{
	for (auto& dir_entry : std::filesystem::directory_iterator{ TEXTURE_PATH })
	{
		const std::string file_name = dir_entry.path().filename().generic_string();
		if (file_name.find(".png") == std::string::npos)
			continue;

		TextureResource resource;
		resource.data = stbi_load(dir_entry.path().generic_string().c_str(), &resource.width, &resource.height, &resource.channels, STBI_rgb_alpha);
        resource.channels = 4;
		if (!resource.data)
			RENDERING_LOG_ERROR("failed to load texture file: " + file_name);

		if (resource)
		{
			const std::string name = file_name.substr(0, file_name.find(".png"));
			m_texture_resources.emplace(name, resource);
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

#undef TEXTURE_PATH

ENGINE_NAMESPACE_END