#pragma once

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
	Offset2D					offset;
	std::optional<Extent2D>		extent;
};

struct Viewport
{
	float					x;
	float					y;
	std::optional<float>    width;
	std::optional<float>    height;
	float					min_depth;
	float					max_depth;
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


// pipeline layout
struct PipelineLayoutInfo
{
	std::string 	vertex_shader;
	std::string 	fragment_shader;
};


// render pass
struct AttachmentDescription
{
	AttachmentType				type;
	std::optional<Format>		format;
	SampleCountFlags			samples;
	AttachmentLoadOp			load_op;
	AttachmentStoreOp			store_op;
	AttachmentLoadOp			stencil_load_op;
	AttachmentStoreOp			stencil_store_op;
	ImageLayout					initial_layout;
	ImageLayout					final_layout;
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

struct RenderPassInfo
{
	Attachments		attachments;
	Subpasses		subpasses;

	NODISCARD constexpr explicit operator bool() const
	{
		return !attachments && !subpasses;
	}
};


// pipeline
struct PipelineShaderState
{
	ShaderStageFlags	stage;
	std::string			module;
	std::string			name;
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

struct PipelineRasterizationState
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
	SampleCountFlags	rasterization_samples;
	Bool				sample_shading_enable;
	float				min_sample_shading;
	// std::vector<uint32_t>	sample_mask;
	Bool				alpha_to_coverage_enable;
	Bool				alpha_to_one_enable;
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
	std::vector<PipelineColorBlendAttachmentState>		color_blend_attachments;
	std::array<float, 4>								blend_constants;
};

struct PipelineInfo
{
	std::vector<PipelineShaderState>				shader_state;
	std::optional<PipelineInputAssemblyState>		input_assembly_state;
	std::optional<PipelineViewportState>			viewport_state;
	std::optional<PipelineRasterizationState>		rasterization_state;
	std::optional<PipelineMultisampleState>			multisample_state;
	std::optional<PipelineDepthStencilState>		depth_stencil_state;
	std::optional<PipelineColorBlendState>			color_blend_state;
	std::vector<DynamicState>						dynamic_state;
	std::string                                     layout;
	std::string                                     render_pass;

	NODISCARD constexpr explicit operator bool() const
	{
		return !shader_state.empty() && render_pass.empty();
	}
};


ENGINE_NAMESPACE_END