#pragma once

#include "rendering/resources/resource_types.h"
#include "rendering/rhi/structs.h"

ENGINE_NAMESPACE_BEGIN

struct RenderPassResource
{
	std::shared_ptr<RenderPassState>	render_pass;
};

class RenderPassManager
{
public:
	RenderPassManager() = default;
	~RenderPassManager() = default;

	void initialize();
	NODISCARD const RenderPassResource& get_render_pass_resource(const std::string_view& name);
private:
	void load_render_pass_files();
private:
	std::unordered_map<std::string, RenderPassResource>		m_render_pass_resources;
};


ENGINE_NAMESPACE_END