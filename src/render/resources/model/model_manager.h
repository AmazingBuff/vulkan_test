#pragma once

#include "base/macro.h"

ENGINE_NAMESPACE_BEGIN

struct ModelResource;

class ModelManager final
{
public:
	ModelManager() = default;
	~ModelManager();

	void initialize();
    NODISCARD const std::shared_ptr<ModelResource>& get_model_resource(const std::string_view& name);
private:
	void load_model_files();
private:
    std::unordered_map<std::string, std::shared_ptr<ModelResource>> m_model_resources;
};

ENGINE_NAMESPACE_END