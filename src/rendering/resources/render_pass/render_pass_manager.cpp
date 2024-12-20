#include "render_pass_manager.h"
#include "rendering/utils/util.h"
#include "rendering/rhi/vulkan/trans/structure_rfl.h"

namespace rfl
{
    template <>
    struct Reflector<Amazing::Engine::RenderPassResource>
    {
        struct ReflType
        {
            Amazing::Engine::RenderPassState   render_pass;
        };

        static Amazing::Engine::RenderPassResource to(const ReflType& v) noexcept
        {
            return { std::make_shared<Amazing::Engine::RenderPassState>(v.render_pass) };
        }

        static ReflType from(const Amazing::Engine::RenderPassResource& v)
        {
            return { *v.render_pass };
        }
    };
}


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