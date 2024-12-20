#pragma once

#include "rendering/resources/resource_types.h"
#include "rendering/rhi/structs.h"

ENGINE_NAMESPACE_BEGIN

struct PipelineResource
{
	std::shared_ptr<PipelineStates>	pipeline;
};


class PipelineManager
{
public:
	PipelineManager() = default;
	~PipelineManager() = default;

	void initialize();
	NODISCARD const PipelineResource& get_pipeline_resource(const std::string_view& name);
private:
	void load_pipeline_files();
private:
	std::unordered_map<std::string, PipelineResource>		m_pipeline_resources;
};


ENGINE_NAMESPACE_END