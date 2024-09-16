#include "render_pass_manager.h"
#include "rendering/utils/util.h"
#include "rendering/rhi/vulkan/trans/structure_trans.h"
#include <rfl.hpp>
#include <rfl/yaml.hpp>

ENGINE_NAMESPACE_BEGIN

#define RENDER_PASS_PATH SOURCES_DIR"/res/render_passes/"

void RenderPassManager::initialize()
{
	load_render_pass_files();
}

const RenderPassResource& RenderPassManager::get_render_pass_resource(const std::string& name)
{
	auto it = m_render_pass_resources.find(name);
	if (it != m_render_pass_resources.end())
		return it->second;
	else
	{
		RENDERING_LOG_ERROR("render pass resource not found: " + name);
		return {};
	}
}

void RenderPassManager::load_render_pass_files()
{
	RenderPassResource resources;
	for (auto& file : std::filesystem::directory_iterator{ RENDER_PASS_PATH })
	{
		const std::string file_name = file.path().filename().generic_string();
		if (file_name.find(".yaml") == std::string::npos)
			continue;
        YAML::Node config = YAML::LoadFile(file.path().generic_string());
		int j = 0;
        //resources.attachments = config["attachments"].as<std::vector<AttachmentDescription>>();
	}
}


#undef RENDER_PASS_PATH

ENGINE_NAMESPACE_END