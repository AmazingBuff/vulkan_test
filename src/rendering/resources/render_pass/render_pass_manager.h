#pragma once

#include "rendering/resources/resource_types.h"
#include "rendering/rhi/structs.h"

ENGINE_NAMESPACE_BEGIN

struct RenderPassResource
{
	std::vector<AttachmentDescription>		attachments;
	std::vector<AttachmentReference>		attachment_references;
	std::vector<SubpassDescription>			subpasses;
	std::vector<SubpassDependency>			dependencies;

	NODISCARD constexpr explicit operator bool() const
	{
		return !attachments.empty() && !attachment_references.empty() && !subpasses.empty();
	}
};


class RenderPassManager
{
public:
	RenderPassManager() = default;
	~RenderPassManager() = default;

	void initialize();
	NODISCARD const RenderPassResource& get_render_pass_resource(const std::string& name);
private:
	void load_render_pass_files();
private:
	std::unordered_map<std::string, RenderPassResource>		m_render_pass_resources;
};


ENGINE_NAMESPACE_END