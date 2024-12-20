#pragma once

#include "rendering/rhi/vulkan/types.h"


ENGINE_NAMESPACE_BEGIN

// fork from vulkan
enum class SampleCountFlagBits
{
	e_1 = VK_SAMPLE_COUNT_1_BIT,
	e_2 = VK_SAMPLE_COUNT_2_BIT,
	e_4 = VK_SAMPLE_COUNT_4_BIT,
	e_8 = VK_SAMPLE_COUNT_8_BIT,
	e_16 = VK_SAMPLE_COUNT_16_BIT,
	e_32 = VK_SAMPLE_COUNT_32_BIT,
	e_64 = VK_SAMPLE_COUNT_64_BIT
};

enum class PipelineStageFlagBits
{
	e_top_of_pipe = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
	e_draw_indirect = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
	e_vertex_input = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
	e_vertex_shader = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
	e_tessellation_control_shader = VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT,
	e_tessellation_evaluation_shader = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT,
	e_geometry_shader = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
	e_fragment_shader = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
	e_early_fragment_tests = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
	e_late_fragment_tests = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
	e_color_attachment_output = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	e_compute_shader = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
	e_transfer = VK_PIPELINE_STAGE_TRANSFER_BIT,
	e_bottom_of_pipe = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
	e_host = VK_PIPELINE_STAGE_HOST_BIT,
	e_all_graphics = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
	e_all_commands = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
};

enum class AccessFlagBits
{
	e_indirect_command_read = VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
	e_index_read = VK_ACCESS_INDEX_READ_BIT,
	e_vertex_attribute_read = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
	e_uniform_read = VK_ACCESS_UNIFORM_READ_BIT,
	e_input_attachment_read = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
	e_shader_read = VK_ACCESS_SHADER_READ_BIT,
	e_shader_write = VK_ACCESS_SHADER_WRITE_BIT,
	e_color_attachment_read = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
	e_color_attachment_write = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
	e_depth_stencil_attachment_read = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
	e_depth_stencil_attachment_write = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
	e_transfer_read = VK_ACCESS_TRANSFER_READ_BIT,
	e_transfer_write = VK_ACCESS_TRANSFER_WRITE_BIT,
	e_host_read = VK_ACCESS_HOST_READ_BIT,
	e_host_write = VK_ACCESS_HOST_WRITE_BIT,
	e_memory_read = VK_ACCESS_MEMORY_READ_BIT,
	e_memory_write = VK_ACCESS_MEMORY_WRITE_BIT
};

enum class DependencyFlagBits
{
	e_by_region = VK_DEPENDENCY_BY_REGION_BIT,
	e_device_group = VK_DEPENDENCY_DEVICE_GROUP_BIT,
	e_view_local = VK_DEPENDENCY_VIEW_LOCAL_BIT
};

enum class ShaderStageFlagBits
{
	e_vertex					=	VK_SHADER_STAGE_VERTEX_BIT,
	e_tessellation_control		=	VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
	e_tessellation_evaluation	=	VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
	e_geometry	                =	VK_SHADER_STAGE_GEOMETRY_BIT,
	e_fragment	                =	VK_SHADER_STAGE_FRAGMENT_BIT,
	e_compute	                =	VK_SHADER_STAGE_COMPUTE_BIT,
	e_all_graphics				=	VK_SHADER_STAGE_ALL_GRAPHICS,
	e_all						=	VK_SHADER_STAGE_ALL,
	// ray tracing ext
	e_raygen					=	VK_SHADER_STAGE_RAYGEN_BIT_KHR,
	e_any_hit					=	VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
	e_closest_hit				=	VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
	e_miss						=	VK_SHADER_STAGE_MISS_BIT_KHR,
	e_intersection				=	VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
	e_callable					=	VK_SHADER_STAGE_CALLABLE_BIT_KHR,
	e_task						=	VK_SHADER_STAGE_TASK_BIT_EXT,
	e_mesh						=	VK_SHADER_STAGE_MESH_BIT_EXT,
	e_subpass_shading			=	VK_SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI,
	e_cluster_culling			=	VK_SHADER_STAGE_CLUSTER_CULLING_BIT_HUAWEI,
};

enum class CullModeFlagBits
{
	e_none				=	VK_CULL_MODE_NONE,
	e_front				=	VK_CULL_MODE_FRONT_BIT,
	e_back				=	VK_CULL_MODE_BACK_BIT,
	e_front_and_back	=	VK_CULL_MODE_FRONT_AND_BACK,
};

enum class ColorComponentFlagBits
{
	e_r				=	VK_COLOR_COMPONENT_R_BIT,
	e_g				=	VK_COLOR_COMPONENT_G_BIT,
	e_b				=	VK_COLOR_COMPONENT_B_BIT,
	e_a				=	VK_COLOR_COMPONENT_A_BIT,
};


enum class FormatEnum
{
	e_undefined						=	VK_FORMAT_UNDEFINED,
	e_r4g4_unorm_pack8				=	VK_FORMAT_R4G4_UNORM_PACK8,
	e_r4g4b4a4_unorm_pack16			=	VK_FORMAT_R4G4B4A4_UNORM_PACK16,
	e_b4g4r4a4_unorm_pack16			=	VK_FORMAT_B4G4R4A4_UNORM_PACK16,
	e_r5g6b5_unorm_pack16			=	VK_FORMAT_R5G6B5_UNORM_PACK16,
	e_b5g6r5_unorm_pack16			=	VK_FORMAT_B5G6R5_UNORM_PACK16,
	e_r5g5b5a1_unorm_pack16			=	VK_FORMAT_R5G5B5A1_UNORM_PACK16,
	e_b5g5r5a1_unorm_pack16			=	VK_FORMAT_B5G5R5A1_UNORM_PACK16,
	e_a1r5g5b5_unorm_pack16			=	VK_FORMAT_A1R5G5B5_UNORM_PACK16,
	e_r8_unorm						=	VK_FORMAT_R8_UNORM,
	e_r8_snorm						=	VK_FORMAT_R8_SNORM,
	e_r8_uscaled					=	VK_FORMAT_R8_USCALED,
	e_r8_sscaled					=	VK_FORMAT_R8_SSCALED,
	e_r8_uint						=	VK_FORMAT_R8_UINT,
	e_r8_sint						=	VK_FORMAT_R8_SINT,
	e_r8_srgb						=	VK_FORMAT_R8_SRGB,
	e_r8g8_unorm					=	VK_FORMAT_R8G8_UNORM,
	e_r8g8_snorm					=	VK_FORMAT_R8G8_SNORM,
	e_r8g8_uscaled					=	VK_FORMAT_R8G8_USCALED,
	e_r8g8_sscaled					=	VK_FORMAT_R8G8_SSCALED,
	e_r8g8_uint						=	VK_FORMAT_R8G8_UINT,
	e_r8g8_sint						=	VK_FORMAT_R8G8_SINT,
	e_r8g8_srgb						=	VK_FORMAT_R8G8_SRGB,
	e_r8g8b8_unorm					=	VK_FORMAT_R8G8B8_UNORM,
	e_r8g8b8_snorm					=	VK_FORMAT_R8G8B8_SNORM,
	e_r8g8b8_uscaled				=	VK_FORMAT_R8G8B8_USCALED,
	e_r8g8b8_sscaled				=	VK_FORMAT_R8G8B8_SSCALED,
	e_r8g8b8_uint					=	VK_FORMAT_R8G8B8_UINT,
	e_r8g8b8_sint					=	VK_FORMAT_R8G8B8_SINT,
	e_r8g8b8_srgb					=	VK_FORMAT_R8G8B8_SRGB,
	e_b8g8r8_unorm					=	VK_FORMAT_B8G8R8_UNORM,
	e_b8g8r8_snorm					=	VK_FORMAT_B8G8R8_SNORM,
	e_b8g8r8_uscaled				=	VK_FORMAT_B8G8R8_USCALED,
	e_b8g8r8_sscaled				=	VK_FORMAT_B8G8R8_SSCALED,
	e_b8g8r8_uint					=	VK_FORMAT_B8G8R8_UINT,
	e_b8g8r8_sint					=	VK_FORMAT_B8G8R8_SINT,
	e_b8g8r8_srgb					=	VK_FORMAT_B8G8R8_SRGB,
	e_r8g8b8a8_unorm				=	VK_FORMAT_R8G8B8A8_UNORM,
	e_r8g8b8a8_snorm				=	VK_FORMAT_R8G8B8A8_SNORM,
	e_r8g8b8a8_uscaled				=	VK_FORMAT_R8G8B8A8_USCALED,
	e_r8g8b8a8_sscaled				=	VK_FORMAT_R8G8B8A8_SSCALED,
	e_r8g8b8a8_uint					=	VK_FORMAT_R8G8B8A8_UINT,
	e_r8g8b8a8_sint					=	VK_FORMAT_R8G8B8A8_SINT,
	e_r8g8b8a8_srgb					=	VK_FORMAT_R8G8B8A8_SRGB,
	e_b8g8r8a8_unorm				=	VK_FORMAT_B8G8R8A8_UNORM,
	e_b8g8r8a8_snorm				=	VK_FORMAT_B8G8R8A8_SNORM,
	e_b8g8r8a8_uscaled				=	VK_FORMAT_B8G8R8A8_USCALED,
	e_b8g8r8a8_sscaled				=	VK_FORMAT_B8G8R8A8_SSCALED,
	e_b8g8r8a8_uint					=	VK_FORMAT_B8G8R8A8_UINT,
	e_b8g8r8a8_sint					=	VK_FORMAT_B8G8R8A8_SINT,
	e_b8g8r8a8_srgb					=	VK_FORMAT_B8G8R8A8_SRGB,
	e_a8b8g8r8_unorm_pack32			=	VK_FORMAT_A8B8G8R8_UNORM_PACK32,
	e_a8b8g8r8_snorm_pack32			=	VK_FORMAT_A8B8G8R8_SNORM_PACK32,
	e_a8b8g8r8_uscaled_pack32		=	VK_FORMAT_A8B8G8R8_USCALED_PACK32,
	e_a8b8g8r8_sscaled_pack32		=	VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
	e_a8b8g8r8_uint_pack32			=	VK_FORMAT_A8B8G8R8_UINT_PACK32,
	e_a8b8g8r8_sint_pack32			=	VK_FORMAT_A8B8G8R8_SINT_PACK32,
	e_a8b8g8r8_srgb_pack32			=	VK_FORMAT_A8B8G8R8_SRGB_PACK32,
	e_a2r10g10b10_unorm_pack32		=	VK_FORMAT_A2R10G10B10_UNORM_PACK32,
	e_a2r10g10b10_snorm_pack32		=	VK_FORMAT_A2R10G10B10_SNORM_PACK32,
	e_a2r10g10b10_uscaled_pack32	=	VK_FORMAT_A2R10G10B10_USCALED_PACK32,
	e_a2r10g10b10_sscaled_pack32	=	VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
	e_a2r10g10b10_uint_pack32		=	VK_FORMAT_A2R10G10B10_UINT_PACK32,
	e_a2r10g10b10_sint_pack32		=	VK_FORMAT_A2R10G10B10_SINT_PACK32,
	e_a2b10g10r10_unorm_pack32		=	VK_FORMAT_A2B10G10R10_UNORM_PACK32,
	e_a2b10g10r10_snorm_pack32		=	VK_FORMAT_A2B10G10R10_SNORM_PACK32,
	e_a2b10g10r10_uscaled_pack32	=	VK_FORMAT_A2B10G10R10_USCALED_PACK32,
	e_a2b10g10r10_sscaled_pack32	=	VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
	e_a2b10g10r10_uint_pack32		=	VK_FORMAT_A2B10G10R10_UINT_PACK32,
	e_a2b10g10r10_sint_pack32		=	VK_FORMAT_A2B10G10R10_SINT_PACK32,
	e_r16_unorm						=	VK_FORMAT_R16_UNORM,
	e_r16_snorm						=	VK_FORMAT_R16_SNORM,
	e_r16_uscaled					=	VK_FORMAT_R16_USCALED,
	e_r16_sscaled					=	VK_FORMAT_R16_SSCALED,
	e_r16_uint						=	VK_FORMAT_R16_UINT,
	e_r16_sint						=	VK_FORMAT_R16_SINT,
	e_r16_sfloat					=	VK_FORMAT_R16_SFLOAT,
	e_r16g16_unorm					=	VK_FORMAT_R16G16_UNORM,
	e_r16g16_snorm					=	VK_FORMAT_R16G16_SNORM,
	e_r16g16_uscaled				=	VK_FORMAT_R16G16_USCALED,
	e_r16g16_sscaled				=	VK_FORMAT_R16G16_SSCALED,
	e_r16g16_uint					=	VK_FORMAT_R16G16_UINT,
	e_r16g16_sint					=	VK_FORMAT_R16G16_SINT,
	e_r16g16_sfloat					=	VK_FORMAT_R16G16_SFLOAT,
	e_r16g16b16_unorm				=	VK_FORMAT_R16G16B16_UNORM,
	e_r16g16b16_snorm				=	VK_FORMAT_R16G16B16_SNORM,
	e_r16g16b16_uscaled				=	VK_FORMAT_R16G16B16_USCALED,
	e_r16g16b16_sscaled				=	VK_FORMAT_R16G16B16_SSCALED,
	e_r16g16b16_uint				=	VK_FORMAT_R16G16B16_UINT,
	e_r16g16b16_sint				=	VK_FORMAT_R16G16B16_SINT,
	e_r16g16b16_sfloat				=	VK_FORMAT_R16G16B16_SFLOAT,
	e_r16g16b16a16_unorm			=	VK_FORMAT_R16G16B16A16_UNORM,
	e_r16g16b16a16_snorm			=	VK_FORMAT_R16G16B16A16_SNORM,
	e_r16g16b16a16_uscaled			=	VK_FORMAT_R16G16B16A16_USCALED,
	e_r16g16b16a16_sscaled			=	VK_FORMAT_R16G16B16A16_SSCALED,
	e_r16g16b16a16_uint				=	VK_FORMAT_R16G16B16A16_UINT,
	e_r16g16b16a16_sint				=	VK_FORMAT_R16G16B16A16_SINT,
	e_r16g16b16a16_sfloat			=	VK_FORMAT_R16G16B16A16_SFLOAT,
	e_r32_uint						=	VK_FORMAT_R32_UINT,
	e_r32_sint						=	VK_FORMAT_R32_SINT,
	e_r32_sfloat					=	VK_FORMAT_R32_SFLOAT,
	e_r32g32_uint					=	VK_FORMAT_R32G32_UINT,
	e_r32g32_sint					=	VK_FORMAT_R32G32_SINT,
	e_r32g32_sfloat					=	VK_FORMAT_R32G32_SFLOAT,
	e_r32g32b32_uint				=	VK_FORMAT_R32G32B32_UINT,
	e_r32g32b32_sint				=	VK_FORMAT_R32G32B32_SINT,
	e_r32g32b32_sfloat				=	VK_FORMAT_R32G32B32_SFLOAT,
	e_r32g32b32a32_uint				=	VK_FORMAT_R32G32B32A32_UINT,
	e_r32g32b32a32_sint				=	VK_FORMAT_R32G32B32A32_SINT,
	e_r32g32b32a32_sfloat			=	VK_FORMAT_R32G32B32A32_SFLOAT,
	e_r64_uint						=	VK_FORMAT_R64_UINT,
	e_r64_sint						=	VK_FORMAT_R64_SINT,
	e_r64_sfloat					=	VK_FORMAT_R64_SFLOAT,
	e_r64g64_uint					=	VK_FORMAT_R64G64_UINT,
	e_r64g64_sint					=	VK_FORMAT_R64G64_SINT,
	e_r64g64_sfloat					=	VK_FORMAT_R64G64_SFLOAT,
	e_r64g64b64_uint				=	VK_FORMAT_R64G64B64_UINT,
	e_r64g64b64_sint				=	VK_FORMAT_R64G64B64_SINT,
	e_r64g64b64_sfloat				=	VK_FORMAT_R64G64B64_SFLOAT,
	e_r64g64b64a64_uint				=	VK_FORMAT_R64G64B64A64_UINT,
	e_r64g64b64a64_sint				=	VK_FORMAT_R64G64B64A64_SINT,
	e_r64g64b64a64_sfloat			=	VK_FORMAT_R64G64B64A64_SFLOAT,
	e_b10g11r11_ufloat_pack32		=	VK_FORMAT_B10G11R11_UFLOAT_PACK32,
	e_e5b9g9r9_ufloat_pack32		=	VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
	e_d16_unorm						=	VK_FORMAT_D16_UNORM,
	e_x8_d24_unorm_pack32			=	VK_FORMAT_X8_D24_UNORM_PACK32,
	e_d32_sfloat					=	VK_FORMAT_D32_SFLOAT,
	e_s8_uint						=	VK_FORMAT_S8_UINT,
	e_d16_unorm_s8_uint				=	VK_FORMAT_D16_UNORM_S8_UINT,
	e_d24_unorm_s8_uint				=	VK_FORMAT_D24_UNORM_S8_UINT,
	e_d32_sfloat_s8_uint			=	VK_FORMAT_D32_SFLOAT_S8_UINT,
};

enum class AttachmentLoadOpEnum
{
	e_load			=	VK_ATTACHMENT_LOAD_OP_LOAD,
	e_clear			=	VK_ATTACHMENT_LOAD_OP_CLEAR,
	e_dont_care		=	VK_ATTACHMENT_LOAD_OP_DONT_CARE
};

enum class AttachmentStoreOpEnum
{
	e_store			=	VK_ATTACHMENT_STORE_OP_STORE,
	e_dont_care		=	VK_ATTACHMENT_STORE_OP_DONT_CARE
};

enum class ImageLayoutEnum
{
	e_undefined										=	VK_IMAGE_LAYOUT_UNDEFINED,
	e_general										=	VK_IMAGE_LAYOUT_GENERAL,
	e_color_attachment_optimal						=	VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	e_depth_stencil_attachment_optimal				=	VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	e_depth_stencil_read_only_optimal				=	VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
	e_shader_read_only_optimal						=	VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
	e_transfer_src_optimal							=	VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
	e_transfer_dst_optimal							=	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	e_preinitialized								=	VK_IMAGE_LAYOUT_PREINITIALIZED,
	e_depth_read_only_stencil_attachment_optimal	=	VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
	e_depth_attachment_stencil_read_only_optimal	=	VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
	e_depth_attachment_optimal						=	VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
	e_depth_read_only_optimal						=	VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL,
	e_stencil_attachment_optimal					=	VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL,
	e_stencil_read_only_optimal						=	VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL,
	e_read_only_optimal								=	VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
	e_attachment_optimal							=	VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
	e_present_src_khr								=	VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
};

enum class PipelineBindPointEnum
{
	e_graphics	=	VK_PIPELINE_BIND_POINT_GRAPHICS,
	e_compute	=	VK_PIPELINE_BIND_POINT_COMPUTE
};

enum class DynamicStateEnum
{
	e_viewport				                    =	VK_DYNAMIC_STATE_VIEWPORT,
	e_scissor				                    =	VK_DYNAMIC_STATE_SCISSOR,
	e_line_width			                    =	VK_DYNAMIC_STATE_LINE_WIDTH,
	e_depth_bias				                =	VK_DYNAMIC_STATE_DEPTH_BIAS,
	e_blend_constants			                =	VK_DYNAMIC_STATE_BLEND_CONSTANTS,
	e_depth_bounds					            =	VK_DYNAMIC_STATE_DEPTH_BOUNDS,
	e_stencil_compare_mask		                =	VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
	e_stencil_write_mask		                =	VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
	e_stencil_reference		                    =	VK_DYNAMIC_STATE_STENCIL_REFERENCE,
	e_cull_mode				                    =	VK_DYNAMIC_STATE_CULL_MODE,
	e_front_face				                =	VK_DYNAMIC_STATE_FRONT_FACE,
	e_primitive_topology		                =	VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY,
	e_viewport_with_count		                =	VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT,
	e_scissor_with_count		                =	VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT,
	e_vertex_input_binding_stride	            =	VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE,
	e_depth_test_enable			                =	VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE,
	e_depth_write_enable						=	VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE,
	e_depth_compare_op							=	VK_DYNAMIC_STATE_DEPTH_COMPARE_OP,
	e_depth_bounds_test_enable		            =	VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE,
	e_stencil_test_enable			            =	VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE,
	e_stencil_op				                =	VK_DYNAMIC_STATE_STENCIL_OP,
	e_rasterizer_discard_enable	                =	VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE,
	e_depth_bias_enable			                =	VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE,
	e_primitive_restart_enable		            =	VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE,

	e_viewport_w_scaling		                =	VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_NV,
	e_discard_rectangle					        =	VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT,
	e_discard_rectangle_enable		            =	VK_DYNAMIC_STATE_DISCARD_RECTANGLE_ENABLE_EXT,
	e_discard_rectangle_mode			        = 	VK_DYNAMIC_STATE_DISCARD_RECTANGLE_MODE_EXT,
	e_sample_locations			                =	VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT,
	e_ray_tracing_pipeline_stack_size	        =	VK_DYNAMIC_STATE_RAY_TRACING_PIPELINE_STACK_SIZE_KHR,
	e_viewport_shading_rate_palette		        =	VK_DYNAMIC_STATE_VIEWPORT_SHADING_RATE_PALETTE_NV,
	e_viewport_coarse_sample_order		        =	VK_DYNAMIC_STATE_VIEWPORT_COARSE_SAMPLE_ORDER_NV,
	e_exclusive_scissor_enable			        =	VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_ENABLE_NV,
	e_exclusive_scissor					        =	VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_NV,
	e_fragment_shading_rate				        =	VK_DYNAMIC_STATE_FRAGMENT_SHADING_RATE_KHR,
	e_vertex_input						        =	VK_DYNAMIC_STATE_VERTEX_INPUT_EXT,
	e_patch_control_points				        =	VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT,
	e_logic_op							        =	VK_DYNAMIC_STATE_LOGIC_OP_EXT,
	e_color_write_enable					    =	VK_DYNAMIC_STATE_COLOR_WRITE_ENABLE_EXT,
	e_depth_clamp_enable					    =	VK_DYNAMIC_STATE_DEPTH_CLAMP_ENABLE_EXT,
	e_polygon_mode						        =	VK_DYNAMIC_STATE_POLYGON_MODE_EXT,
	e_rasterization_samples				        =	VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT,
	e_sample_mask							    =	VK_DYNAMIC_STATE_SAMPLE_MASK_EXT,
	e_alpha_to_coverage_enable			        =	VK_DYNAMIC_STATE_ALPHA_TO_COVERAGE_ENABLE_EXT,
	e_alpha_to_one_enable				        =	VK_DYNAMIC_STATE_ALPHA_TO_ONE_ENABLE_EXT,
	e_logic_op_enable						    =	VK_DYNAMIC_STATE_LOGIC_OP_ENABLE_EXT,
	e_color_blend_enable					    =	VK_DYNAMIC_STATE_COLOR_BLEND_ENABLE_EXT,
	e_color_blend_equation					    =	VK_DYNAMIC_STATE_COLOR_BLEND_EQUATION_EXT,
	e_color_write_mask						    =	VK_DYNAMIC_STATE_COLOR_WRITE_ENABLE_EXT,
	e_tessellation_domain_origin		        =	VK_DYNAMIC_STATE_TESSELLATION_DOMAIN_ORIGIN_EXT,
	e_rasterization_stream				        =	VK_DYNAMIC_STATE_RASTERIZATION_STREAM_EXT,
	e_conservative_rasterization_mode		    =	VK_DYNAMIC_STATE_CONSERVATIVE_RASTERIZATION_MODE_EXT,
	e_extra_primitive_overestimation_size	    = 	VK_DYNAMIC_STATE_EXTRA_PRIMITIVE_OVERESTIMATION_SIZE_EXT,
	e_depth_clip_enable						    =	VK_DYNAMIC_STATE_DEPTH_CLIP_ENABLE_EXT,
	e_sample_locations_enable				    =	VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_ENABLE_EXT,
	e_color_blend_advanced					    =	VK_DYNAMIC_STATE_COLOR_BLEND_ADVANCED_EXT,
	e_provoking_vertex_mode 				    =	VK_DYNAMIC_STATE_PROVOKING_VERTEX_MODE_EXT,
	e_line_rasterization_mode				    =	VK_DYNAMIC_STATE_LINE_RASTERIZATION_MODE_EXT,
	e_line_stipple_enable					    =	VK_DYNAMIC_STATE_LINE_STIPPLE_ENABLE_EXT,
	e_depth_clip_negative_one_to_one	        =	VK_DYNAMIC_STATE_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT,
	e_viewport_w_scaling_enable			        =	VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_ENABLE_NV,
	e_viewport_swizzle						    =	VK_DYNAMIC_STATE_VIEWPORT_SWIZZLE_NV,
	e_coverage_to_color_enable				    =	VK_DYNAMIC_STATE_COVERAGE_TO_COLOR_ENABLE_NV,
	e_coverage_to_color_location			    =	VK_DYNAMIC_STATE_COVERAGE_TO_COLOR_LOCATION_NV,
	e_coverage_modulation_mode			        =	VK_DYNAMIC_STATE_COVERAGE_MODULATION_MODE_NV,
	e_coverage_modulation_table_enable	        =	VK_DYNAMIC_STATE_COVERAGE_MODULATION_TABLE_ENABLE_NV,
	e_coverage_modulation_table			        =	VK_DYNAMIC_STATE_COVERAGE_MODULATION_TABLE_NV,
	e_representative_fragment_test_enable	    =	VK_DYNAMIC_STATE_REPRESENTATIVE_FRAGMENT_TEST_ENABLE_NV,
	e_coverage_reduction_mode			        =	VK_DYNAMIC_STATE_COVERAGE_REDUCTION_MODE_NV,
	e_attachment_feedback_loop_enable	        =	VK_DYNAMIC_STATE_ATTACHMENT_FEEDBACK_LOOP_ENABLE_EXT,
	e_line_stipple						        =	VK_DYNAMIC_STATE_LINE_STIPPLE_KHR,
	e_depth_clamp_range					        =	VK_DYNAMIC_STATE_DEPTH_CLAMP_RANGE_EXT,
};

enum class PrimitiveTopologyEnum
{
	e_point_list		                =   VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
	e_line_list			                =   VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
	e_line_strip		                =   VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
	e_triangle_list		                =   VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	e_triangle_strip	                =   VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
	e_line_list_with_adjacency	        =   VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
	e_line_strip_with_adjacency         =   VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
	e_triangle_list_with_adjacency	    =   VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
	e_triangle_strip_with_adjacency     =   VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
	e_patch_list						=   VK_PRIMITIVE_TOPOLOGY_PATCH_LIST,
};

enum class PolygonModeEnum
{
	e_fill				= 	VK_POLYGON_MODE_FILL,
	e_line				= 	VK_POLYGON_MODE_LINE,
	e_point 			= 	VK_POLYGON_MODE_POINT,
	e_fill_rectangle	= 	VK_POLYGON_MODE_FILL_RECTANGLE_NV,
};

enum class FrontFaceEnum
{
	e_counter_clockwise		=	VK_FRONT_FACE_COUNTER_CLOCKWISE,
	e_clockwise				=	VK_FRONT_FACE_CLOCKWISE,
};

enum class CompareOpEnum
{
	e_never					=	VK_COMPARE_OP_NEVER,
	e_less					=	VK_COMPARE_OP_LESS,
	e_equal					=	VK_COMPARE_OP_EQUAL,
	e_less_or_equal			=	VK_COMPARE_OP_LESS_OR_EQUAL,
	e_greater				=	VK_COMPARE_OP_GREATER,
	e_not_equal				=	VK_COMPARE_OP_NOT_EQUAL,
	e_greater_or_equal		=	VK_COMPARE_OP_GREATER_OR_EQUAL,
	e_always				=	VK_COMPARE_OP_ALWAYS,
};

enum class StencilOpEnum
{
	e_keep					=	VK_STENCIL_OP_KEEP,
	e_zero					=	VK_STENCIL_OP_ZERO,
	e_replace				=	VK_STENCIL_OP_REPLACE,
	e_increment_and_clamp	=	VK_STENCIL_OP_INCREMENT_AND_CLAMP,
	e_decrement_and_clamp	=	VK_STENCIL_OP_DECREMENT_AND_CLAMP,
	e_invert				=	VK_STENCIL_OP_INVERT,
	e_increment_and_wrap	=	VK_STENCIL_OP_INCREMENT_AND_WRAP,
	e_decrement_and_wrap	=	VK_STENCIL_OP_DECREMENT_AND_WRAP,
};

enum class BlendFactorEnum
{
	e_zero						=	VK_BLEND_FACTOR_ZERO,
	e_one						=	VK_BLEND_FACTOR_ONE,
	e_src_color					=	VK_BLEND_FACTOR_SRC_COLOR,
	e_one_minus_src_color		=	VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
	e_dst_color					=	VK_BLEND_FACTOR_DST_COLOR,
	e_one_minus_dst_color		=	VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
	e_src_alpha					=	VK_BLEND_FACTOR_SRC_ALPHA,
	e_one_minus_src_alpha		=	VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	e_dst_alpha					=	VK_BLEND_FACTOR_DST_ALPHA,
	e_one_minus_dst_alpha		=	VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
	e_constant_color			=	VK_BLEND_FACTOR_CONSTANT_COLOR,
	e_one_minus_constant_color	=	VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
	e_constant_alpha			=	VK_BLEND_FACTOR_CONSTANT_ALPHA,
	e_one_minus_constant_alpha	=	VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
	e_src_alpha_saturate		=	VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
	e_src1_color				=	VK_BLEND_FACTOR_SRC1_COLOR,
	e_one_minus_src1_color		=	VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
	e_src1_alpha				=	VK_BLEND_FACTOR_SRC1_ALPHA,
	e_one_minus_src1_alpha		=	VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA,
};

enum class BlendOpEnum
{
	e_add					=	VK_BLEND_OP_ADD,
	e_subtract				=	VK_BLEND_OP_SUBTRACT,
	e_reverse_subtract		=	VK_BLEND_OP_REVERSE_SUBTRACT,
	e_min					=	VK_BLEND_OP_MIN,
	e_max					=	VK_BLEND_OP_MAX,
	e_zero					=	VK_BLEND_OP_ZERO_EXT,
	e_src					=	VK_BLEND_OP_SRC_EXT,
	e_dst					=	VK_BLEND_OP_DST_EXT,
	e_src_over				=	VK_BLEND_OP_SRC_OVER_EXT,
	e_dst_over				=	VK_BLEND_OP_DST_OVER_EXT,
	e_src_in				=	VK_BLEND_OP_SRC_IN_EXT,
	e_dst_in				=	VK_BLEND_OP_DST_IN_EXT,
	e_src_out				=	VK_BLEND_OP_SRC_OUT_EXT,
	e_dst_out				=	VK_BLEND_OP_DST_OUT_EXT,
	e_src_atop				=	VK_BLEND_OP_SRC_ATOP_EXT,
	e_dst_atop				=	VK_BLEND_OP_DST_ATOP_EXT,
	e_xor					=	VK_BLEND_OP_XOR_EXT,
	e_multiply				=	VK_BLEND_OP_MULTIPLY_EXT,
	e_screen				=	VK_BLEND_OP_SCREEN_EXT,
	e_overlay				=	VK_BLEND_OP_OVERLAY_EXT,
	e_darken				=	VK_BLEND_OP_DARKEN_EXT,
	e_lighten				=	VK_BLEND_OP_LIGHTEN_EXT,
	e_colordodge			=	VK_BLEND_OP_COLORDODGE_EXT,
	e_colorburn				=	VK_BLEND_OP_COLORBURN_EXT,
	e_hardlight				=	VK_BLEND_OP_HARDLIGHT_EXT,
	e_softlight				=	VK_BLEND_OP_SOFTLIGHT_EXT,
	e_difference			=	VK_BLEND_OP_DIFFERENCE_EXT,
	e_exclusion				=	VK_BLEND_OP_EXCLUSION_EXT,
	e_invert					=	VK_BLEND_OP_INVERT_EXT,
	e_lineardodge				=	VK_BLEND_OP_LINEARDODGE_EXT,
	e_linearburn				=	VK_BLEND_OP_LINEARBURN_EXT,
	e_vividlight				=	VK_BLEND_OP_VIVIDLIGHT_EXT,
	e_linearlight				=	VK_BLEND_OP_LINEARLIGHT_EXT,
	e_pinlight					=	VK_BLEND_OP_PINLIGHT_EXT,
	e_hardmix					=	VK_BLEND_OP_HARDMIX_EXT,
	e_hsl_hue					=	VK_BLEND_OP_HSL_HUE_EXT,
	e_hsl_saturation			=	VK_BLEND_OP_HSL_SATURATION_EXT,
	e_hsl_color					=	VK_BLEND_OP_HSL_COLOR_EXT,
	e_hsl_luminosity			=	VK_BLEND_OP_HSL_LUMINOSITY_EXT,
	e_plus						=	VK_BLEND_OP_PLUS_EXT,
	e_plus_clamped				=	VK_BLEND_OP_PLUS_CLAMPED_EXT,
	e_plus_clamped_alpha		=	VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT,
	e_plus_darker				=	VK_BLEND_OP_PLUS_DARKER_EXT,
	e_minus						=	VK_BLEND_OP_MINUS_EXT,
	e_minus_clamped				=	VK_BLEND_OP_MINUS_CLAMPED_EXT,
	e_contrast					=	VK_BLEND_OP_CONTRAST_EXT,
	e_invert_ovg				=	VK_BLEND_OP_INVERT_OVG_EXT,
	e_red						=	VK_BLEND_OP_RED_EXT,
	e_green						=	VK_BLEND_OP_GREEN_EXT,
	e_blue						=	VK_BLEND_OP_BLUE_EXT,
};

enum class LogicOpEnum
{
	e_clear 			=	VK_LOGIC_OP_CLEAR,
	e_and				= 	VK_LOGIC_OP_AND,
	e_and_reverse		= 	VK_LOGIC_OP_AND_REVERSE,
	e_copy				= 	VK_LOGIC_OP_COPY,
	e_and_inverted		= 	VK_LOGIC_OP_AND_INVERTED,
	e_no_op				= 	VK_LOGIC_OP_NO_OP,
	e_xor				= 	VK_LOGIC_OP_XOR,
	e_or				= 	VK_LOGIC_OP_OR,
	e_nor				= 	VK_LOGIC_OP_NOR,
	e_equivalent		= 	VK_LOGIC_OP_EQUIVALENT,
	e_invert			= 	VK_LOGIC_OP_INVERT,
	e_or_reverse		= 	VK_LOGIC_OP_OR_REVERSE,
	e_copy_inverted 	= 	VK_LOGIC_OP_COPY_INVERTED,
	e_or_inverted		= 	VK_LOGIC_OP_OR_INVERTED,
	e_nand				= 	VK_LOGIC_OP_NAND,
	e_set				= 	VK_LOGIC_OP_SET,
};


// self
enum class AttachmentTypeEnum
{
    e_color,
    e_depth,
    e_stencil,
    e_depth_stencil
};

ENGINE_NAMESPACE_END