//
// Created by AmazingBuff on 2024/12/28.
//

#include "pipeline_layout_manager.h"
#include "render/utils/util.h"
#include "render/rhi/vulkan/trans/structure_rfl.h"

#include <rfl/yaml.hpp>

namespace rfl
{
    template <>
    struct Reflector<Amazing::Engine::PipelineLayoutResource>
    {
        struct ReflType
        {
            Amazing::Engine::PipelineLayoutInfo   pipeline_layout;
        };

        static Amazing::Engine::PipelineLayoutResource to(const ReflType& v) noexcept
        {
            return { std::make_shared<Amazing::Engine::PipelineLayoutInfo>(v.pipeline_layout) };
        }

        static ReflType from(const Amazing::Engine::PipelineLayoutResource& v)
        {
            return { *v.pipeline_layout };
        }
    };
}


ENGINE_NAMESPACE_BEGIN
#define PIPELINE_LAYOUT_PATH SOURCES_DIR"/res/pipeline_layouts/"

void PipelineLayoutManager::initialize()
{
    load_pipeline_layout_files();
}

const PipelineLayoutResource& PipelineLayoutManager::get_pipeline_layout_resource(const std::string_view& name)
{
    auto it = m_pipeline_layout_resources.find(name.data());
    if (it == m_pipeline_layout_resources.end())
        RENDERING_LOG_ERROR("render pass resource not found: " + std::string(name));

    return it->second;
}

void PipelineLayoutManager::load_pipeline_layout_files()
{
    for (auto& file : std::filesystem::directory_iterator{ PIPELINE_LAYOUT_PATH })
    {
        const std::string file_name = file.path().filename().generic_string();
        if (file_name.find(".yaml") == std::string::npos)
            continue;
        const std::string name = file_name.substr(0, file_name.find(".yaml"));

        YAML::Node config = YAML::LoadFile(file.path().generic_string());
        PipelineLayoutResource resource = rfl::yaml::read<PipelineLayoutResource, rfl::DefaultIfMissing>(config).value();
        m_pipeline_layout_resources.emplace(name, resource);
    }
}

#undef PIPELINE_LAYOUT_PATH

ENGINE_NAMESPACE_END
