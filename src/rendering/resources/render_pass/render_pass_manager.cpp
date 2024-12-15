#include "render_pass_manager.h"
#include "rendering/utils/util.h"
#include "rendering/rhi/vulkan/trans/structure_rfl.h"

namespace rfl
{
    // self
    template <>
    struct Reflector<Amazing::Engine::Attachments>
    {
        struct ReflType
        {
            std::vector<Amazing::Engine::AttachmentDescription>		descriptions;
            std::vector<Amazing::Engine::AttachmentReference>		references;
        };
        static Amazing::Engine::Attachments to(const ReflType& v) noexcept
        {
            return { v.descriptions, v.references };
        }
        static ReflType from(const Amazing::Engine::Attachments& v)
        {
            return { v.descriptions, v.references };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::Subpasses>
    {
        struct ReflType
        {
            std::vector<Amazing::Engine::SubpassDescription>	descriptions;
            std::vector<Amazing::Engine::SubpassDependency>		dependencies;
        };
        static Amazing::Engine::Subpasses to(const ReflType& v) noexcept
        {
            return { v.descriptions, v.dependencies };
        }
        static ReflType from(const Amazing::Engine::Subpasses& v)
        {
            return { v.descriptions, v.dependencies };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::RenderPassResource>
    {
        struct ReflType
        {
            Amazing::Engine::Attachments		attachments;
            Amazing::Engine::Subpasses		    subpasses;
        };

        static Amazing::Engine::RenderPassResource to(const ReflType& v) noexcept
        {
            return { v.attachments, v.subpasses};
        }

        static ReflType from(const Amazing::Engine::RenderPassResource& v)
        {
            return { v.attachments, v.subpasses};
        }
    };
}


ENGINE_NAMESPACE_BEGIN
#define RENDER_PASS_PATH SOURCES_DIR"/res/render_passes/"

void RenderPassManager::initialize()
{
	load_render_pass_files();
}

const RenderPassResource& RenderPassManager::get_render_pass_resource(const std::string& name)
{
	auto it = m_render_pass_resources.find(name);
	if (it == m_render_pass_resources.end())
		RENDERING_LOG_ERROR("render pass resource not found: " + name);

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
        RenderPassResource resource = rfl::yaml::read<RenderPassResource, rfl::DefaultIfMissing>(config["render_pass"]).value();
        m_render_pass_resources.emplace(name, resource);
    }
}

#undef RENDER_PASS_PATH

ENGINE_NAMESPACE_END