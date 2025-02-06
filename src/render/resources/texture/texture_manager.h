#pragma once

#include "base/macro.h"

ENGINE_NAMESPACE_BEGIN

struct TextureResource;

class TextureManager final
{
public:
	TextureManager() = default;
	~TextureManager();

	void initialize();
    NODISCARD const std::shared_ptr<TextureResource>& get_texture_resource(const std::string_view& name);
private:
	void load_texture_files();
private:
    std::unordered_map<std::string, std::shared_ptr<TextureResource>> m_texture_resources;
};

ENGINE_NAMESPACE_END