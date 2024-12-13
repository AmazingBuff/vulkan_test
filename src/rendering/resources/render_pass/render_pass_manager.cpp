#include "render_pass_manager.h"
#include "rendering/utils/util.h"
#include "rendering/rhi/vulkan/trans/structure_trans.h"
#include "base/enum_string/enum_to_string.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

#include <rfl.hpp>
#include <rfl/yaml.hpp>

#include "rendering/rhi/vulkan/trans/enum_trans.h"
#include "rendering/rhi/vulkan/trans/enum_map.h"

ENGINE_NAMESPACE_BEGIN
#define RENDER_PASS_PATH SOURCES_DIR"/res/render_passes/"


struct Person 
{
	rfl::Rename<"firstName", std::string> first_name;
	rfl::Rename<"lastName", std::string> last_name;
	rfl::Timestamp<"%Y-%m-%d"> birthday;
	std::vector<Person> children;
};

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
	RenderPassResource resources;
	for (auto& file : std::filesystem::directory_iterator{ RENDER_PASS_PATH })
	{
		const std::string file_name = file.path().filename().generic_string();
		if (file_name.find(".yaml") == std::string::npos)
			continue;
        YAML::Node config = YAML::LoadFile(file.path().generic_string());

        const std::string attachment_descriptions = YAML::Dump(config["render_pass"]["attachments"]["descriptions"]);
        const std::string attachment_references = YAML::Dump(config["render_pass"]["attachments"]["references"]);
        const std::string subpass_descriptions = YAML::Dump(config["render_pass"]["subpasses"]["descriptions"]);
        const std::string subpass_dependencies = YAML::Dump(config["render_pass"]["subpasses"]["dependencies"]);

		const auto& d = config["render_pass"]["attachments"]["descriptions"];

		const rfl::Result<Person> result = rfl::yaml::read<Person>("");

	}
}


#undef RENDER_PASS_PATH

ENGINE_NAMESPACE_END