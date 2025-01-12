//
// Created by AmazingBuff on 2024/12/13.
//

#pragma once

#include "base/macro.h"
#include "base/logger.h"
#include "enum_trans.h"

ENGINE_NAMESPACE_BEGIN

#define STRING_ENUM(string, enum) {#string, enum##::##e_##string}
#define STRING_ENUM_MAP(enum) enum##_String_To_Enum_Map

#define ENUM_STRING(enum, string) {enum##::##e_##string, #string}
#define ENUM_STRING_MAP(enum) enum##_Enum_To_String_Map

template <typename Enum>
Enum string_to_enum(const std::string& str);

template <typename Enum>
std::string enum_to_string(const Enum& e);

#define STRING_ENUM_FUNC(enum)                              \
template <>                                                 \
inline enum string_to_enum<enum>(const std::string& str)    \
{                                                           \
    auto it = STRING_ENUM_MAP(enum).find(str);              \
    if (it != STRING_ENUM_MAP(enum).end())                  \
        return it->second;                                  \
    if (!str.empty() && str != "0")                         \
        LOG_ERROR("String To Enum", "no match enum!");      \
    return enum();                                          \
}                                                           \
template <>                                                 \
inline std::string enum_to_string(const enum& e)            \
{                                                           \
    auto it = ENUM_STRING_MAP(enum).find(e);                \
    if (it != ENUM_STRING_MAP(enum).end())                  \
        return it->second;                                  \
    LOG_ERROR("Enum To String", "no match string!");        \
    return "";                                              \
}


#define PARENS ()

#define FOR_EACH_STRING(enum, ...)                                      \
__VA_OPT__(EXPAND(FOR_EACH_STRING_HELPER(enum, __VA_ARGS__)))
#define FOR_EACH_STRING_HELPER(enum, str, ...)                          \
STRING_ENUM(str, enum),                                                 \
__VA_OPT__(FOR_EACH_STRING_AGAIN PARENS (enum, __VA_ARGS__))

#define FOR_EACH_STRING_AGAIN() FOR_EACH_STRING_HELPER

#define FOR_EACH_ENUM(enum, ...)                                        \
__VA_OPT__(EXPAND(FOR_EACH_ENUM_HELPER(enum, __VA_ARGS__)))
#define FOR_EACH_ENUM_HELPER(enum, str, ...)                            \
ENUM_STRING(enum, str),                                                 \
__VA_OPT__(FOR_EACH_ENUM_AGAIN PARENS (enum, __VA_ARGS__))

#define FOR_EACH_ENUM_AGAIN() FOR_EACH_ENUM_HELPER

#define GENERATE_ENUM_MAP(enum, ...)    \
const static std::unordered_map<std::string, enum> STRING_ENUM_MAP(enum) = { FOR_EACH_STRING(enum, __VA_ARGS__) };\
const static std::unordered_map<enum, std::string> ENUM_STRING_MAP(enum) = { FOR_EACH_ENUM(enum, __VA_ARGS__) };

GENERATE_ENUM_MAP(SampleCountFlagBits, 1, 2, 4, 8, 16, 32, 64)
GENERATE_ENUM_MAP(PipelineStageFlagBits,
                  top_of_pipe,
                  draw_indirect,
                  vertex_input,
                  vertex_shader,
                  tessellation_control_shader,
                  tessellation_evaluation_shader,
                  geometry_shader,
                  fragment_shader,
                  early_fragment_tests,
                  late_fragment_tests,
                  color_attachment_output,
                  compute_shader,
                  transfer,
                  bottom_of_pipe,
                  host,
                  all_graphics,
                  all_commands)
GENERATE_ENUM_MAP(AccessFlagBits,
                  indirect_command_read,
                  index_read,
                  vertex_attribute_read,
                  uniform_read,
                  input_attachment_read,
                  shader_read,
                  shader_write,
                  color_attachment_read,
                  color_attachment_write,
                  depth_stencil_attachment_read,
                  depth_stencil_attachment_write,
                  transfer_read,
                  transfer_write,
                  host_read,
                  host_write,
                  memory_read,
                  memory_write,
                  none)
GENERATE_ENUM_MAP(DependencyFlagBits,
                  by_region,
                  device_group,
                  view_local)
GENERATE_ENUM_MAP(ShaderStageFlagBits,
                  vertex,
                  tessellation_control,
                  tessellation_evaluation,
                  geometry,
                  fragment,
                  compute,
                  all_graphics,
                  all,
                  raygen,
                  any_hit,
                  closest_hit,
                  miss,
                  intersection,
                  callable,
                  task,
                  mesh,
                  subpass_shading,
                  cluster_culling)
GENERATE_ENUM_MAP(CullModeFlagBits,
                  none,
                  front,
                  back,
                  front_and_back)
GENERATE_ENUM_MAP(ColorComponentFlagBits,
                  r,
                  g,
                  b,
                  a)
GENERATE_ENUM_MAP(FormatEnum,
                  undefined,
                  r4g4_unorm_pack8,
                  r4g4b4a4_unorm_pack16,
                  b4g4r4a4_unorm_pack16,
                  r5g6b5_unorm_pack16,
                  b5g6r5_unorm_pack16,
                  r5g5b5a1_unorm_pack16,
                  b5g5r5a1_unorm_pack16,
                  a1r5g5b5_unorm_pack16,
                  r8_unorm,
                  r8_snorm,
                  r8_uscaled,
                  r8_sscaled,
                  r8_uint,
                  r8_sint,
                  r8_srgb,
                  r8g8_unorm,
                  r8g8_snorm,
                  r8g8_uscaled,
                  r8g8_sscaled,
                  r8g8_uint,
                  r8g8_sint,
                  r8g8_srgb,
                  r8g8b8_unorm,
                  r8g8b8_snorm,
                  r8g8b8_uscaled,
                  r8g8b8_sscaled,
                  r8g8b8_uint,
                  r8g8b8_sint,
                  r8g8b8_srgb,
                  b8g8r8_unorm,
                  b8g8r8_snorm,
                  b8g8r8_uscaled,
                  b8g8r8_sscaled,
                  b8g8r8_uint,
                  b8g8r8_sint,
                  b8g8r8_srgb,
                  r8g8b8a8_unorm,
                  r8g8b8a8_snorm,
                  r8g8b8a8_uscaled,
                  r8g8b8a8_sscaled,
                  r8g8b8a8_uint,
                  r8g8b8a8_sint,
                  r8g8b8a8_srgb,
                  b8g8r8a8_unorm,
                  b8g8r8a8_snorm,
                  b8g8r8a8_uscaled,
                  b8g8r8a8_sscaled,
                  b8g8r8a8_uint,
                  b8g8r8a8_sint,
                  b8g8r8a8_srgb,
                  a8b8g8r8_unorm_pack32,
                  a8b8g8r8_snorm_pack32,
                  a8b8g8r8_uscaled_pack32,
                  a8b8g8r8_sscaled_pack32,
                  a8b8g8r8_uint_pack32,
                  a8b8g8r8_sint_pack32,
                  a8b8g8r8_srgb_pack32,
                  a2r10g10b10_unorm_pack32,
                  a2r10g10b10_snorm_pack32,
                  a2r10g10b10_uscaled_pack32,
                  a2r10g10b10_sscaled_pack32,
                  a2r10g10b10_uint_pack32,
                  a2r10g10b10_sint_pack32,
                  a2b10g10r10_unorm_pack32,
                  a2b10g10r10_snorm_pack32,
                  a2b10g10r10_uscaled_pack32,
                  a2b10g10r10_sscaled_pack32,
                  a2b10g10r10_uint_pack32,
                  a2b10g10r10_sint_pack32,
                  r16_unorm,
                  r16_snorm,
                  r16_uscaled,
                  r16_sscaled,
                  r16_uint,
                  r16_sint,
                  r16_sfloat,
                  r16g16_unorm,
                  r16g16_snorm,
                  r16g16_uscaled,
                  r16g16_sscaled,
                  r16g16_uint,
                  r16g16_sint,
                  r16g16_sfloat,
                  r16g16b16_unorm,
                  r16g16b16_snorm,
                  r16g16b16_uscaled,
                  r16g16b16_sscaled,
                  r16g16b16_uint,
                  r16g16b16_sint,
                  r16g16b16_sfloat,
                  r16g16b16a16_unorm,
                  r16g16b16a16_snorm,
                  r16g16b16a16_uscaled,
                  r16g16b16a16_sscaled,
                  r16g16b16a16_uint,
                  r16g16b16a16_sint,
                  r16g16b16a16_sfloat,
                  r32_uint,
                  r32_sint,
                  r32_sfloat,
                  r32g32_uint,
                  r32g32_sint,
                  r32g32_sfloat,
                  r32g32b32_uint,
                  r32g32b32_sint,
                  r32g32b32_sfloat,
                  r32g32b32a32_uint,
                  r32g32b32a32_sint,
                  r32g32b32a32_sfloat,
                  r64_uint,
                  r64_sint,
                  r64_sfloat,
                  r64g64_uint,
                  r64g64_sint,
                  r64g64_sfloat,
                  r64g64b64_uint,
                  r64g64b64_sint,
                  r64g64b64_sfloat,
                  r64g64b64a64_uint,
                  r64g64b64a64_sint,
                  r64g64b64a64_sfloat,
                  b10g11r11_ufloat_pack32,
                  e5b9g9r9_ufloat_pack32,
                  d16_unorm,
                  x8_d24_unorm_pack32,
                  d32_sfloat,
                  s8_uint,
                  d16_unorm_s8_uint,
                  d24_unorm_s8_uint,
                  d32_sfloat_s8_uint)
GENERATE_ENUM_MAP(AttachmentLoadOpEnum,
                  load,
                  clear,
                  dont_care)
GENERATE_ENUM_MAP(AttachmentStoreOpEnum,
                  store,
                  dont_care)
GENERATE_ENUM_MAP(ImageLayoutEnum,
                  undefined,
                  general,
                  color_attachment_optimal,
                  depth_stencil_attachment_optimal,
                  depth_stencil_read_only_optimal,
                  shader_read_only_optimal,
                  transfer_src_optimal,
                  transfer_dst_optimal,
                  preinitialized,
                  depth_read_only_stencil_attachment_optimal,
                  depth_attachment_stencil_read_only_optimal,
                  depth_attachment_optimal,
                  depth_read_only_optimal,
                  stencil_attachment_optimal,
                  stencil_read_only_optimal,
                  read_only_optimal,
                  attachment_optimal,
                  present_src)
GENERATE_ENUM_MAP(PipelineBindPointEnum,
                  graphics,
                  compute)
GENERATE_ENUM_MAP(DynamicStateEnum,
                  viewport,
                  scissor,
                  line_width,
                  depth_bias,
                  blend_constants,
                  depth_bounds,
                  stencil_compare_mask,
                  stencil_write_mask,
                  stencil_reference,
                  cull_mode,
                  front_face,
                  primitive_topology,
                  viewport_with_count,
                  scissor_with_count,
                  vertex_input_binding_stride,
                  depth_test_enable,
                  depth_write_enable,
                  depth_compare_op,
                  depth_bounds_test_enable,
                  stencil_test_enable,
                  stencil_op,
                  rasterizer_discard_enable,
                  depth_bias_enable,
                  primitive_restart_enable,
                  viewport_w_scaling,
                  discard_rectangle,
                  discard_rectangle_enable,
                  discard_rectangle_mode,
                  sample_locations,
                  ray_tracing_pipeline_stack_size,
                  viewport_shading_rate_palette,
                  viewport_coarse_sample_order,
                  exclusive_scissor_enable,
                  exclusive_scissor,
                  fragment_shading_rate,
                  vertex_input,
                  patch_control_points,
                  logic_op,
                  color_write_enable,
                  depth_clamp_enable,
                  polygon_mode,
                  rasterization_samples,
                  sample_mask,
                  alpha_to_coverage_enable,
                  alpha_to_one_enable,
                  logic_op_enable,
                  color_blend_enable,
                  color_blend_equation,
                  color_write_mask,
                  tessellation_domain_origin,
                  rasterization_stream,
                  conservative_rasterization_mode,
                  extra_primitive_overestimation_size,
                  depth_clip_enable,
                  sample_locations_enable,
                  color_blend_advanced,
                  provoking_vertex_mode,
                  line_rasterization_mode,
                  line_stipple_enable,
                  depth_clip_negative_one_to_one,
                  viewport_w_scaling_enable,
                  viewport_swizzle,
                  coverage_to_color_enable,
                  coverage_to_color_location,
                  coverage_modulation_mode,
                  coverage_modulation_table_enable,
                  coverage_modulation_table,
                  representative_fragment_test_enable,
                  coverage_reduction_mode,
                  attachment_feedback_loop_enable,
                  line_stipple,
                  depth_clamp_range)
GENERATE_ENUM_MAP(PrimitiveTopologyEnum,
                  point_list,
                  line_list,
                  line_strip,
                  triangle_list,
                  triangle_strip,
                  line_list_with_adjacency,
                  line_strip_with_adjacency,
                  triangle_list_with_adjacency,
                  triangle_strip_with_adjacency,
                  patch_list)
GENERATE_ENUM_MAP(PolygonModeEnum,
                  fill,
                  line,
                  point,
                  fill_rectangle)
GENERATE_ENUM_MAP(FrontFaceEnum,
                  counter_clockwise,
                  clockwise)
GENERATE_ENUM_MAP(CompareOpEnum,
                  never,
                  less,
                  equal,
                  less_or_equal,
                  greater,
                  not_equal,
                  greater_or_equal,
                  always)
GENERATE_ENUM_MAP(StencilOpEnum,
                  keep,
                  zero,
                  replace,
                  increment_and_clamp,
                  decrement_and_clamp,
                  invert,
                  increment_and_wrap,
                  decrement_and_wrap)
GENERATE_ENUM_MAP(BlendFactorEnum,
                  zero,
                  one,
                  src_color,
                  one_minus_src_color,
                  dst_color,
                  one_minus_dst_color,
                  src_alpha,
                  one_minus_src_alpha,
                  dst_alpha,
                  one_minus_dst_alpha,
                  constant_color,
                  one_minus_constant_color,
                  constant_alpha,
                  one_minus_constant_alpha,
                  src_alpha_saturate,
                  src1_color,
                  one_minus_src1_color,
                  src1_alpha,
                  one_minus_src1_alpha)
GENERATE_ENUM_MAP(BlendOpEnum,
                  add,
                  subtract,
                  reverse_subtract,
                  min,
                  max,
                  zero,
                  src,
                  dst,
                  src_over,
                  dst_over,
                  src_in,
                  dst_in,
                  src_out,
                  dst_out,
                  src_atop,
                  dst_atop,
                  xor,
                  multiply,
                  screen,
                  overlay,
                  darken,
                  lighten,
                  colordodge,
                  colorburn,
                  hardlight,
                  softlight,
                  difference,
                  exclusion,
                  invert,
                  lineardodge,
                  linearburn,
                  vividlight,
                  linearlight,
                  pinlight,
                  hardmix,
                  hsl_hue,
                  hsl_saturation,
                  hsl_color,
                  hsl_luminosity,
                  plus,
                  plus_clamped,
                  plus_clamped_alpha,
                  plus_darker,
                  minus,
                  minus_clamped,
                  contrast,
                  invert_ovg,
                  red,
                  green,
                  blue)
GENERATE_ENUM_MAP(LogicOpEnum,
                  clear,
                  and,
                  and_reverse,
                  copy,
                  and_inverted,
                  no_op,
                  xor,
                  or,
                  nor,
                  equivalent,
                  invert,
                  or_reverse,
                  copy_inverted,
                  or_inverted,
                  nand,
                  set)



GENERATE_ENUM_MAP(AttachmentTypeEnum,
                  color,
                  depth,
                  stencil,
                  depth_stencil)


STRING_ENUM_FUNC(SampleCountFlagBits)
STRING_ENUM_FUNC(PipelineStageFlagBits)
STRING_ENUM_FUNC(AccessFlagBits)
STRING_ENUM_FUNC(DependencyFlagBits)
STRING_ENUM_FUNC(ShaderStageFlagBits)
STRING_ENUM_FUNC(CullModeFlagBits)
STRING_ENUM_FUNC(ColorComponentFlagBits)


STRING_ENUM_FUNC(FormatEnum)
STRING_ENUM_FUNC(AttachmentLoadOpEnum)
STRING_ENUM_FUNC(AttachmentStoreOpEnum)
STRING_ENUM_FUNC(ImageLayoutEnum)
STRING_ENUM_FUNC(PipelineBindPointEnum)
STRING_ENUM_FUNC(DynamicStateEnum)
STRING_ENUM_FUNC(PrimitiveTopologyEnum)
STRING_ENUM_FUNC(PolygonModeEnum)
STRING_ENUM_FUNC(FrontFaceEnum)
STRING_ENUM_FUNC(CompareOpEnum)
STRING_ENUM_FUNC(StencilOpEnum)
STRING_ENUM_FUNC(BlendFactorEnum)
STRING_ENUM_FUNC(BlendOpEnum)
STRING_ENUM_FUNC(LogicOpEnum)


STRING_ENUM_FUNC(AttachmentTypeEnum)


#undef GENERATE_ENUM_MAP
#undef FOR_EACH_ENUM_AGAIN
#undef FOR_EACH_ENUM_HELPER
#undef FOR_EACH_ENUM
#undef FOR_EACH_STRING_AGAIN
#undef FOR_EACH_STRING_HELPER
#undef FOR_EACH_STRING
#undef PARENS

#undef ENUM_STRING_MAP
#undef ENUM_STRING
#undef STRING_ENUM_FUNC
#undef STRING_ENUM_MAP
#undef STRING_ENUM

ENGINE_NAMESPACE_END