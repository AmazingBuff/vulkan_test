#pragma once

#include "render/resources/resource_types.h"

ENGINE_NAMESPACE_BEGIN

struct RenderPassInfo;

struct RenderPassResource
{
	std::shared_ptr<RenderPassInfo>	render_pass;
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