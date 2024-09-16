#pragma once

#include "rendering/rhi/enums.h"

ENGINE_NAMESPACE_BEGIN

struct AttachmentDescription
{
    std::string				type;
	Format					format;
	SampleCountFlags		samples;
	AttachmentLoadOp		load_op;
	AttachmentStoreOp		store_op;
	AttachmentLoadOp		stencil_load_op;
	AttachmentStoreOp		stencil_store_op;
	ImageLayout				initial_layout;
	ImageLayout				final_layout;
};

struct AttachmentReference
{
	std::string				type;
	uint32_t				attachment;
	ImageLayout				layout;
};

struct SubpassDescription
{
	std::string							type;
	PipelineBindPoint					pipeline_bind_point;
	std::vector<AttachmentReference>	input_attachments;
	std::vector<AttachmentReference>	color_attachments;
	std::vector<AttachmentReference>	resolve_attachments;
	AttachmentReference					depth_stencil_attachment;
	std::vector<uint32_t>				preserve_attachments;
};

struct SubpassDependency
{
	std::string				type;
	uint32_t				src_subpass;
	uint32_t				dst_subpass;
	PipelineStageFlags		src_stage_mask;
	PipelineStageFlags		dst_stage_mask;
	AccessFlags				src_access_mask;
	AccessFlags				dst_access_mask;
	DependencyFlags			dependency_flags;
};

ENGINE_NAMESPACE_END