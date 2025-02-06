#include "pipeline_manager.h"
#include "render/utils/util.h"
#include "render/resources/resource_types.h"
#include "render/rhi/vulkan/trans/structure_rfl.h"

#include <rfl/yaml.hpp>

namespace rfl
{
    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineResource)>
    {
        struct ReflType
        {
            ENGINE_NAMESPACE_CONCAT(PipelineInfo) pipeline;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineResource) to(const ReflType& v) noexcept
        {
            return { std::make_shared<ENGINE_NAMESPACE_CONCAT(PipelineInfo)>(v.pipeline) };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineResource)& v)
        {
            return { *v.pipeline };
        }
    };
}


ENGINE_NAMESPACE_BEGIN
#define PIPELINE_PATH SOURCES_DIR"/res/pipelines/"

void PipelineManager::initialize()
{
    load_pipeline_files();
}

const std::shared_ptr<PipelineResource>& PipelineManager::get_pipeline_resource(const std::string_view& name)
{
    auto it = m_pipeline_resources.find(name.data());
    if (it == m_pipeline_resources.end())
        RENDERING_LOG_ERROR("pipeline resource not found: " + std::string(name));

    return it->second;
}

void PipelineManager::load_pipeline_files()
{
    for (auto& file : std::filesystem::directory_iterator{ PIPELINE_PATH })
    {
        const std::string file_name = file.path().filename().generic_string();
        if (file_name.find(".yaml") == std::string::npos)
            continue;
        const std::string name = file_name.substr(0, file_name.find(".yaml"));

        YAML::Node config = YAML::LoadFile(file.path().generic_string());
        std::shared_ptr<PipelineResource> resource = std::make_shared<PipelineResource>(
            rfl::yaml::read<PipelineResource, rfl::DefaultIfMissing>(config).value());
        m_pipeline_resources.emplace(name, resource);
    }
}

#undef PIPELINE_PATH

ENGINE_NAMESPACE_END