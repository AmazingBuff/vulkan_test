//
// Created by AmazingBuff on 2024/12/28.
//

#pragma once

#include "base/macro.h"

ENGINE_NAMESPACE_BEGIN

struct PipelineLayoutResource;

class PipelineLayoutManager
{
public:
    PipelineLayoutManager() = default;
    ~PipelineLayoutManager() = default;

    void initialize();
    NODISCARD const std::shared_ptr<PipelineLayoutResource>& get_pipeline_layout_resource(const std::string_view& name);
private:
    void load_pipeline_layout_files();
private:
    std::unordered_map<std::string, std::shared_ptr<PipelineLayoutResource>> m_pipeline_layout_resources;
};



ENGINE_NAMESPACE_END