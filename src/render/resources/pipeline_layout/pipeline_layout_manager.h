//
// Created by AmazingBuff on 2024/12/28.
//

#pragma once

#include "render/resources/resource_types.h"

ENGINE_NAMESPACE_BEGIN

struct PipelineLayoutInfo;

struct PipelineLayoutResource
{
    std::shared_ptr<PipelineLayoutInfo>	pipeline_layout;
};

class PipelineLayoutManager
{
public:
    PipelineLayoutManager() = default;
    ~PipelineLayoutManager() = default;

    void initialize();
    NODISCARD const PipelineLayoutResource& get_pipeline_layout_resource(const std::string_view& name);
private:
    void load_pipeline_layout_files();
private:
    std::unordered_map<std::string, PipelineLayoutResource>		m_pipeline_layout_resources;
};



ENGINE_NAMESPACE_END