#pragma once

#include "base/macro.h"

ENGINE_NAMESPACE_BEGIN

struct PipelineResource;

class PipelineManager
{
public:
	PipelineManager() = default;
	~PipelineManager() = default;

	void initialize();
    NODISCARD const std::shared_ptr<PipelineResource>& get_pipeline_resource(const std::string_view& name);
private:
	void load_pipeline_files();
private:
	std::unordered_map<std::string, std::shared_ptr<PipelineResource>>		m_pipeline_resources;
};


ENGINE_NAMESPACE_END