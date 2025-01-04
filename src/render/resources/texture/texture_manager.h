#pragma once

#include "render/resources/resource_types.h"

ENGINE_NAMESPACE_BEGIN

struct TextureResource
{
	int				width		=	0;
	int				height		=	0;
	int				channels	=	0;
	uint8_t*		data		=	nullptr;

	NODISCARD explicit operator bool() const
	{
		return width > 0 && height > 0 && channels > 0 && data != nullptr;
	}
};


class TextureManager final
{
public:
	TextureManager() = default;
	~TextureManager();

	void initialize();
	NODISCARD const TextureResource& get_texture_resource(const std::string_view& name);
private:
	void load_texture_files();
private:
	std::unordered_map<std::string, TextureResource>		m_texture_resources;
};

ENGINE_NAMESPACE_END