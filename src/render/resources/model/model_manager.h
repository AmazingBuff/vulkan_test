#pragma once

#include "render/resources/resource_types.h"

ENGINE_NAMESPACE_BEGIN

struct ModelResource
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