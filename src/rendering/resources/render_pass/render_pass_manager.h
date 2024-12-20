#pragma once

#include "rendering/resources/resource_types.h"
#include "rendering/rhi/structs.h"

ENGINE_NAMESPACE_BEGIN

// self
struct Attachments
{
	std::vector<AttachmentDescription>		descriptions;
	std::vector<AttachmentReference>		references;

	NODISCARD constexpr explicit operator bool() const
	{
		return !descriptions.empty() && !references.empty();
	}
};

struct Subpasses
{
	std::vector<SubpassDescription>			descriptions;
	std::vector<SubpassDependency>			dependencies;

	NODISCARD constexpr explicit operator bool() const
	{
		return !descriptions.empty();
	}
};

struct RenderPassResource
{
	Attachments		attachments;
	Subpasses		subpasses;

	NODISCARD constexpr explicit operator bool() const
	{
		return !attachments && !subpasses;
	}
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