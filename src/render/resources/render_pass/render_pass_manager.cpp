#include "render_pass_manager.h"
#include "render/utils/util.h"
#include "render/rhi/vulkan/trans/structure_rfl.h"

#include <rfl/yaml.hpp>

template <>
struct rfl::Reflector<ENGINE_NAMESPACE_CONCAT(RenderPassResource)>
{
    struct ReflType
    {
        ENGINE_NAMESPACE_CONCAT(RenderPassInfo)   render_pass;
    };

    static ENGINE_NAMESPACE_CONCAT(RenderPassResource) to(const ReflType& v) noexcept
    {
        return { std::make_shared<ENGINE_NAMESPACE_CONCAT(RenderPassInfo)>(v.render_pass) };
    }

    static ReflType from(const ENGINE_NAMESPACE_CONCAT(RenderPassResource)& v)
    {
        return { *v.render_pass };
    }
};


ENGINE_NAMESPACE_BEGIN
#define RENDER_PASS_PATH SOURCES_DIR"/res/render_passes/"

void RenderPassManager::initialize()
{
    load_render_pass_files();
}

const RenderPassResource& RenderPassManager::get_render_pass_resource(const std::string_view& name)
{
    auto it = m_render_pass_resources.find(name.data());
    if (it == m_render_pass_resources.end())
        RENDERING_LOG_ERROR("render pass resource not found: " + std::string(name));

    return it->second;
}

void RenderPassManager::load_render_pass_files()
{
    for (auto& file : std::filesystem::directory_iterator{ RENDER_PASS_PATH })
    {
        const std::string file_name = file.path().filename().generic_string();
        if (file_name.find(".yaml") == std::string::npos)
            continue;
        const std::string name = file_name.substr(0, file_name.find(".yaml"));

        YAML::Node config = YAML::LoadFile(file.path().generic_string());
        RenderPassResource resource = rfl::yaml::read<RenderPassResource, rfl::DefaultIfMissing>(config).value();
        m_render_pass_resources.emplace(name, resource);
    }
}

#undef RENDER_PASS_PATH

ENGINE_NAMESPACE_END