#pragma once

#include "render/resources/resource_types.h"
#include "render/resources/geometry/geometry_data.h"

ENGINE_NAMESPACE_BEGIN

struct ModelResource
{
	std::vector<Vertex>				vertices;
	std::vector<uint32_t>			indices;
	std::vector<TextureResource>	textures;

	NODISCARD explicit operator bool() const
	{
		return !vertices.empty();
	}
};


class ModelManager final
{
public:
	ModelManager() = default;
	~ModelManager();

	void initialize();
	NODISCARD const ModelResource& get_model_resource(const std::string_view& name);
private:
	void load_model_files();
private:
	std::unordered_map<std::string, ModelResource>		m_model_resources;
};

ENGINE_NAMESPACE_END