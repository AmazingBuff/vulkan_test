#pragma once

#include "base/macro.h"

ENGINE_NAMESPACE_BEGIN

struct RenderPassResource;

class RenderPassManager
{
public:
	RenderPassManager() = default;
	~RenderPassManager() = default;

	void initialize();
    NODISCARD const std::shared_ptr<RenderPassResource>& get_render_pass_resource(const std::string_view& name);
private:
	void load_render_pass_files();
private:
    std::unordered_map<std::string, std::shared_ptr<RenderPassResource>> m_render_pass_resources;
};


ENGINE_NAMESPACE_END