#include "pipeline_manager.h"
#include "rendering/utils/util.h"
#include "rendering/rhi/vulkan/trans/structure_rfl.h"

namespace rfl
{
    template <>
    struct Reflector<Amazing::Engine::PipelineResource>
    {
        struct ReflType
        {
            Amazing::Engine::PipelineStates pipeline;
        };

        static Amazing::Engine::PipelineResource to(const ReflType& v) noexcept
        {
            return { std::make_shared<Amazing::Engine::PipelineStates>(v.pipeline) };
        }

        static ReflType from(const Amazing::Engine::PipelineResource& v)
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

const PipelineResource& PipelineManager::get_pipeline_resource(const std::string_view& name)
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
        PipelineResource resource = rfl::yaml::read<PipelineResource, rfl::DefaultIfMissing>(config).value();
        m_pipeline_resources.emplace(name, resource);
    }
}

#undef PIPELINE_PATH

ENGINE_NAMESPACE_END