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


// self
enum class AttachmentTypeEnum
{
    e_color,
    e_depth,
    e_stencil,
    e_depth_stencil
};

ENGINE_NAMESPACE_END