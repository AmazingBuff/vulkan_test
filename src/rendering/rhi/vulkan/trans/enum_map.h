//
// Created by AmazingBuff on 2024/12/13.
//

#pragma once

#include "base/macro.h"
#include "enum_trans.h"

ENGINE_NAMESPACE_BEGIN

#define STRING_ENUM(enum, string) {#string, enum##::##e_##string}
#define STRING_ENUM_MAP(enum) enum##_Map

template <typename Enum>
Enum string_enum(const std::string& str);

#define STRING_TO_ENUM(enum)                            \
template <>                                             \
enum string_enum<enum>(const std::string& str)          \
{                                                       \
    auto it = STRING_ENUM_MAP(enum).find(str);          \
    if (it != STRING_ENUM_MAP(enum).end())              \
        return it->second;                              \
    return enum();                                      \
}

#define PARENS ()

#define FOR_EACH(enum, ...)                                     \
__VA_OPT__(EXPAND(FOR_EACH_HELPER(enum, __VA_ARGS__)))
#define FOR_EACH_HELPER(enum, str, ...)                         \
STRING_ENUM(enum, str),                                         \
__VA_OPT__(FOR_EACH_AGAIN PARENS (enum, __VA_ARGS__))

#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define GENERATE_ENUM_MAP(enum, ...)    \
const static std::unordered_map<std::string, enum> STRING_ENUM_MAP(enum) = { FOR_EACH(enum, __VA_ARGS__) };

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
                  memory_write)
GENERATE_ENUM_MAP(DependencyFlagBits,
                  by_region,
                  device_group,
                  view_local)
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
                  present_src_khr)
GENERATE_ENUM_MAP(PipelineBindPointEnum,
                  graphics,
                  compute)
GENERATE_ENUM_MAP(AttachmentTypeEnum,
                  color,
                  depth,
                  stencil,
                  depth_stencil)


STRING_TO_ENUM(SampleCountFlagBits)
STRING_TO_ENUM(PipelineStageFlagBits)
STRING_TO_ENUM(AccessFlagBits)
STRING_TO_ENUM(DependencyFlagBits)
STRING_TO_ENUM(FormatEnum)
STRING_TO_ENUM(AttachmentLoadOpEnum)
STRING_TO_ENUM(AttachmentStoreOpEnum)
STRING_TO_ENUM(ImageLayoutEnum)
STRING_TO_ENUM(PipelineBindPointEnum)
STRING_TO_ENUM(AttachmentTypeEnum)


#undef GENERATE_ENUM_MAP
#undef FOR_EACH
#undef FOR_EACH_HELPER
#undef FOR_EACH_AGAIN()
#undef PARENS
#undef STRING_ENUM_MAP
#undef STRING_ENUM

ENGINE_NAMESPACE_END