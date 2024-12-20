#pragma once

#include "base/geometry.h"
#include "rendering/rhi/enums.h"

ENGINE_NAMESPACE_BEGIN

using Bool = uint32_t;

struct Offset2D
{
	int32_t				x;
	int32_t				y;
};

struct Extent2D
{
	uint32_t			width;
	uint32_t			height;
};

struct Rect2D
{
	Offset2D	offset;
	Extent2D	extent;
};

struct Viewport
{
	float    x;
	float    y;
	float    width;
	float    height;
	float    min_depth;
	float    max_depth;
};

struct StencilOpState
{
	StencilOp	fail_op;
	StencilOp	pass_op;
	StencilOp	depth_fail_op;
	CompareOp	compare_op;
	uint32_t	compare_mask;
	uint32_t	write_mask;
	uint32_t	reference;
};

// render pass
struct AttachmentDescription
{
	AttachmentType			type;
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
	AttachmentType			type;
	uint32_t				attachment;
	ImageLayout				layout;
};

struct SubpassDescription
{
	std::string								name;
	PipelineBindPoint						pipeline_bind_point;
	std::vector<AttachmentReference>		input_attachments;
	std::vector<AttachmentReference>		color_attachments;
	std::vector<AttachmentReference>		resolve_attachments;
	std::optional<AttachmentReference>		depth_stencil_attachment;
	std::vector<uint32_t>					preserve_attachments;
};

struct SubpassDependency
{
	std::string				name;
	uint32_t				src_subpass;
	uint32_t				dst_subpass;
	PipelineStageFlags		src_stage_mask;
	PipelineStageFlags		dst_stage_mask;
	AccessFlags				src_access_mask;
	AccessFlags				dst_access_mask;
	DependencyFlags			dependency_flags;
};


// pipeline
struct PipelineShaderState
{
	ShaderStageFlags	stage;
	std::string			module;
	std::string			name;
};

struct PipelineDynamicState
{
	std::vector<DynamicState>	dynamic_states;
};

struct PipelineInputAssemblyState
{
	PrimitiveTopology	topology;
	Bool				primitive_restart_enable;
};

struct PipelineViewportState
{
	std::vector<Viewport>	viewports;
	std::vector<Rect2D>		scissors;
};

struct PipelineRasterizerState
{
	Bool			depth_clamp_enable;
	Bool			rasterizer_discard_enable;
	PolygonMode		polygon_mode;
	CullModeFlags	cull_mode;
	FrontFace		front_face;
	Bool			depth_bias_enable;
	float			depth_bias_constant_factor;
	float			depth_bias_clamp;
	float			depth_bias_slope_factor;
	float			line_width;
};

struct PipelineMultisampleState
{
	SampleCountFlags rasterization_samples;
	Bool			sample_shading_enable;
	float			min_sample_shading;
	// std::vector<uint32_t>	sample_mask;
	Bool			alpha_to_coverage_enable;
	Bool			alpha_to_one_enable;
};

struct PipelineDepthStencilState
{
	Bool			depth_test_enable;
	Bool			depth_write_enable;
	CompareOp		depth_compare_op;
	Bool			depth_bounds_test_enable;
	Bool			stencil_test_enable;
	StencilOpState  front;
	StencilOpState  back;
	float			min_depth_bounds;
	float			max_depth_bounds;
};

struct PipelineColorBlendAttachmentState
{
	Bool					blend_enable;
	BlendFactor				src_color_blend_factor;
	BlendFactor				dst_color_blend_factor;
	BlendOp					color_blend_op;
	BlendFactor				src_alpha_blend_factor;
	BlendFactor				dst_alpha_blend_factor;
	BlendOp					alpha_blend_op;
	ColorComponentFlags 	color_write_mask;
};

struct PipelineColorBlendState
{
	Bool												logic_op_enable;
	LogicOp 											logic_op;
	std::vector<PipelineColorBlendAttachmentState>		attachments;
	std::array<float, 4>								blend_constants;
};


ENGINE_NAMESPACE_END