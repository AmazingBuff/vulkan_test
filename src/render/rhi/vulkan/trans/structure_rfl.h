#pragma once

#include "enum_map.h"
#include "structure_trans.h"

#include <rfl.hpp>
#include <func.h>
#include <regex>


template<typename Enum>
Enum string_match_enum(const std::string& str)
{
    Enum ret;

    std::regex pattern("\\s*\\|\\s*");
    std::sregex_token_iterator it(str.begin(), str.end(), pattern, -1);

    while (it != std::sregex_token_iterator())
    {
        Enum v(ENGINE_NAMESPACE_CONCAT(string_to_enum<Amazing::Trait::head_type_t<typename Amazing::Trait::template_traits<Enum>::type>>(it->str())));
        ret |= v;
        it++;
    }

    return ret;
}

template<typename Enum>
std::string enum_match_string(const Enum& value)
{
    uint32_t v = static_cast<uint32_t>(static_cast<int>(value));

    std::string ret;
    for (uint32_t i = 0; i < 32; i++)
    {
        if (v & (1 << i))
        {
            if (!ret.empty())
                ret += " | ";
            ret += ENGINE_NAMESPACE_CONCAT(enum_to_string(static_cast<Amazing::Trait::head_type_t<typename Amazing::Trait::template_traits<Enum>::type>>(1 << i)));
        }
    }

    return ret;
}


#define REFLECTOR_TO_ENUM(ty, value, name)      \
            ENGINE_NAMESPACE_CONCAT(ty) name(ENGINE_NAMESPACE_CONCAT(string_to_enum<Amazing::Trait::head_type_t<typename Amazing::Trait::template_traits<ENGINE_NAMESPACE_CONCAT(ty)>::type>>(value.name)))
#define REFLECTOR_FROM_ENUM(ty, value, name)    \
            std::string name = ENGINE_NAMESPACE_CONCAT(enum_to_string(static_cast<Amazing::Trait::head_type_t<typename Amazing::Trait::template_traits<ENGINE_NAMESPACE_CONCAT(ty)>::type>>(value.name)))

#define MATCH_TO_ENUM(type, value, name)        \
            ENGINE_NAMESPACE_CONCAT(type) name = string_match_enum<ENGINE_NAMESPACE_CONCAT(type)>(value.name)
#define MATCH_FROM_ENUM(type, value, name)      \
            std::string name = enum_match_string(value.name)


namespace rfl
{
    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(Offset2D)>
    {
        struct ReflType
        {
            int32_t x;
            int32_t y;
        };

        static ENGINE_NAMESPACE_CONCAT(Offset2D) to(const ReflType& v) noexcept
        {
            return { v.x, v.y };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(Offset2D)& v)
        {
            return { v.x, v.y };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(Extent2D)>
    {
        struct ReflType
        {
            uint32_t width;
            uint32_t height;
        };

        static ENGINE_NAMESPACE_CONCAT(Extent2D) to(const ReflType& v) noexcept
        {
            return { v.width, v.height };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(Extent2D)& v)
        {
            return { v.width, v.height };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(Rect2D)>
    {
        struct ReflType
        {
            ENGINE_NAMESPACE_CONCAT(Offset2D)                     offset;
            std::optional<ENGINE_NAMESPACE_CONCAT(Extent2D)>      extent;
        };

        static ENGINE_NAMESPACE_CONCAT(Rect2D) to(const ReflType& v) noexcept
        {
            return { v.offset, v.extent };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(Rect2D)& v)
        {
            return { v.offset, v.extent };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(Viewport)>
    {
        struct ReflType
        {
            float                   x;
            float                   y;
            std::optional<float>    width;
            std::optional<float>    height;
            float                   min_depth;
            float                   max_depth;
        };

        static ENGINE_NAMESPACE_CONCAT(Viewport) to(const ReflType& v) noexcept
        {
            return { v.x, v.y, v.width, v.height, v.min_depth, v.max_depth };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(Viewport)& v)
        {
            return { v.x, v.y, v.width, v.height, v.min_depth, v.max_depth };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(StencilOpState)>
    {
        struct ReflType
        {
            std::string	    fail_op;
            std::string	    pass_op;
            std::string	    depth_fail_op;
            std::string	    compare_op;
            uint32_t	    compare_mask;
            uint32_t	    write_mask;
            uint32_t	    reference;
        };

        static ENGINE_NAMESPACE_CONCAT(StencilOpState) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(StencilOp, v, fail_op);
            REFLECTOR_TO_ENUM(StencilOp, v, pass_op);
            REFLECTOR_TO_ENUM(StencilOp, v, depth_fail_op);
            REFLECTOR_TO_ENUM(CompareOp, v, compare_op);
            return { fail_op, pass_op, depth_fail_op, compare_op, v.compare_mask, v.write_mask, v.reference };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(StencilOpState)& v)
        {
            REFLECTOR_FROM_ENUM(StencilOp, v, fail_op);
            REFLECTOR_FROM_ENUM(StencilOp, v, pass_op);
            REFLECTOR_FROM_ENUM(StencilOp, v, depth_fail_op);
            REFLECTOR_FROM_ENUM(CompareOp, v, compare_op);
            return { fail_op, pass_op, depth_fail_op, compare_op, v.compare_mask, v.write_mask, v.reference };
        }
    };


    // pipeline layout
    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineLayoutInfo)>
    {
        struct ReflType
        {
            std::string	                    vertex_shader;
            std::string	                    fragment_shader;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineLayoutInfo) to(const ReflType& v) noexcept
        {
            return { v.vertex_shader, v.fragment_shader };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineLayoutInfo)& v)
        {
            return { v.vertex_shader, v.fragment_shader };
        }
    };


    // render pass
    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(AttachmentDescription)>
    {
        struct ReflType
        {
            std::string	                    type;
            std::optional<std::string>	    format;
            std::string	                    samples;
            std::string	                    load_op;
            std::string	                    store_op;
            std::string	                    stencil_load_op;
            std::string	                    stencil_store_op;
            std::string	                    initial_layout;
            std::string	                    final_layout;
        };

        static ENGINE_NAMESPACE_CONCAT(AttachmentDescription) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(AttachmentType, v, type);
            std::optional <ENGINE_NAMESPACE_CONCAT(Format)> format = std::nullopt;
            if (v.format)
                format = ENGINE_NAMESPACE_CONCAT(string_to_enum<ENGINE_NAMESPACE_CONCAT(FormatEnum)>(v.format.value()));
            REFLECTOR_TO_ENUM(SampleCountFlags, v, samples);
            REFLECTOR_TO_ENUM(AttachmentLoadOp, v, load_op);
            REFLECTOR_TO_ENUM(AttachmentStoreOp, v, store_op);
            REFLECTOR_TO_ENUM(AttachmentLoadOp, v, stencil_load_op);
            REFLECTOR_TO_ENUM(AttachmentStoreOp, v, stencil_store_op);
            REFLECTOR_TO_ENUM(ImageLayout, v, initial_layout);
            REFLECTOR_TO_ENUM(ImageLayout, v, final_layout);
            return { type, format, samples, load_op, store_op, stencil_load_op, stencil_store_op, initial_layout, final_layout };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(AttachmentDescription)& v)
        {
            REFLECTOR_FROM_ENUM(AttachmentType, v, type);
            std::optional<std::string> format = std::nullopt;
            if (v.format)
                format = ENGINE_NAMESPACE_CONCAT(enum_to_string(static_cast<ENGINE_NAMESPACE_CONCAT(FormatEnum)>(v.format.value())));
            REFLECTOR_FROM_ENUM(SampleCountFlags, v, samples);
            REFLECTOR_FROM_ENUM(AttachmentLoadOp, v, load_op);
            REFLECTOR_FROM_ENUM(AttachmentStoreOp, v, store_op);
            REFLECTOR_FROM_ENUM(AttachmentLoadOp, v, stencil_load_op);
            REFLECTOR_FROM_ENUM(AttachmentStoreOp, v, stencil_store_op);
            REFLECTOR_FROM_ENUM(ImageLayout, v, initial_layout);
            REFLECTOR_FROM_ENUM(ImageLayout, v, final_layout);
            return { type, format, samples, load_op, store_op, stencil_load_op, stencil_store_op, initial_layout, final_layout };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(AttachmentReference)>
    {
        struct ReflType
        {
            std::string	    type;
            uint32_t        attachment;
            std::string	    layout;
        };

        static ENGINE_NAMESPACE_CONCAT(AttachmentReference) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(AttachmentType, v, type);
            REFLECTOR_TO_ENUM(ImageLayout, v, layout);
            return { type, v.attachment, layout };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(AttachmentReference)& v)
        {
            REFLECTOR_FROM_ENUM(AttachmentType, v, type);
            REFLECTOR_FROM_ENUM(ImageLayout, v, layout);
            return { type, v.attachment, layout };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(SubpassDescription)>
    {
        struct ReflType
        {
            std::string	                                                    name;
            std::string	                                                    pipeline_bind_point;
            std::vector<ENGINE_NAMESPACE_CONCAT(AttachmentReference)>     input_attachments;
            std::vector<ENGINE_NAMESPACE_CONCAT(AttachmentReference)>     color_attachments;
            std::vector<ENGINE_NAMESPACE_CONCAT(AttachmentReference)>     resolve_attachments;
            std::optional<ENGINE_NAMESPACE_CONCAT(AttachmentReference)>   depth_stencil_attachment;
            std::vector<uint32_t>				                            preserve_attachments;
        };

        static ENGINE_NAMESPACE_CONCAT(SubpassDescription) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(PipelineBindPoint, v, pipeline_bind_point);
            return { v.name, pipeline_bind_point, v.input_attachments, v.color_attachments, v.resolve_attachments, v.depth_stencil_attachment, v.preserve_attachments };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(SubpassDescription)& v)
        {
            REFLECTOR_FROM_ENUM(PipelineBindPoint, v, pipeline_bind_point);
            return { v.name, pipeline_bind_point, v.input_attachments, v.color_attachments, v.resolve_attachments, v.depth_stencil_attachment, v.preserve_attachments };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(SubpassDependency)>
    {
        struct ReflType
        {
            std::string	    name;
            std::string		src_subpass;
            std::string		dst_subpass;
            std::string	    src_stage_mask;
            std::string	    dst_stage_mask;
            std::string	    src_access_mask;
            std::string	    dst_access_mask;
            std::string	    dependency_flags;
        };

        static ENGINE_NAMESPACE_CONCAT(SubpassDependency) to(const ReflType& v) noexcept
        {
            MATCH_TO_ENUM(PipelineStageFlags, v, src_stage_mask);
            MATCH_TO_ENUM(PipelineStageFlags, v, dst_stage_mask);
            MATCH_TO_ENUM(AccessFlags, v, src_access_mask);
            MATCH_TO_ENUM(AccessFlags, v, dst_access_mask);
            MATCH_TO_ENUM(DependencyFlags, v, dependency_flags);
            uint32_t src_subpass = v.src_subpass == "external" ? VK_SUBPASS_EXTERNAL : std::stoul(v.src_subpass);
            uint32_t dst_subpass = v.dst_subpass == "external" ? VK_SUBPASS_EXTERNAL : std::stoul(v.dst_subpass);
            return { v.name, src_subpass, dst_subpass, src_stage_mask, dst_stage_mask, src_access_mask, dst_access_mask, dependency_flags };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(SubpassDependency)& v)
        {
            MATCH_FROM_ENUM(PipelineStageFlags, v, src_stage_mask);
            MATCH_FROM_ENUM(PipelineStageFlags, v, dst_stage_mask);
            MATCH_FROM_ENUM(AccessFlags, v, src_access_mask);
            MATCH_FROM_ENUM(AccessFlags, v, dst_access_mask);
            MATCH_FROM_ENUM(DependencyFlags, v, dependency_flags);
            std::string src_subpass = v.src_subpass == VK_SUBPASS_EXTERNAL ? "external" : std::to_string(v.src_subpass);
            std::string dst_subpass = v.dst_subpass == VK_SUBPASS_EXTERNAL ? "external" : std::to_string(v.dst_subpass);
            return { v.name, src_subpass, dst_subpass, src_stage_mask, dst_stage_mask, src_access_mask, dst_access_mask, dependency_flags };
        }
    };

    // self
    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(Attachments)>
    {
        struct ReflType
        {
            std::vector<ENGINE_NAMESPACE_CONCAT(AttachmentDescription)>   descriptions;
            std::vector<ENGINE_NAMESPACE_CONCAT(AttachmentReference)>     references;
        };
        static ENGINE_NAMESPACE_CONCAT(Attachments) to(const ReflType& v) noexcept
        {
            return { v.descriptions, v.references };
        }
        static ReflType from(const ENGINE_NAMESPACE_CONCAT(Attachments)& v)
        {
            return { v.descriptions, v.references };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(Subpasses)>
    {
        struct ReflType
        {
            std::vector<ENGINE_NAMESPACE_CONCAT(SubpassDescription)> descriptions;
            std::vector<ENGINE_NAMESPACE_CONCAT(SubpassDependency)> dependencies;
        };
        static ENGINE_NAMESPACE_CONCAT(Subpasses) to(const ReflType& v) noexcept
        {
            return { v.descriptions, v.dependencies };
        }
        static ReflType from(const ENGINE_NAMESPACE_CONCAT(Subpasses)& v)
        {
            return { v.descriptions, v.dependencies };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(RenderPassInfo)>
    {
        struct ReflType
        {
            ENGINE_NAMESPACE_CONCAT(Attachments)		    attachments;
            ENGINE_NAMESPACE_CONCAT(Subpasses)		    subpasses;
        };

        static ENGINE_NAMESPACE_CONCAT(RenderPassInfo) to(const ReflType& v) noexcept
        {
            return { v.attachments, v.subpasses };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(RenderPassInfo)& v)
        {
            return { v.attachments, v.subpasses };
        }
    };


    // pipeline
    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineShaderState)>
    {
        struct ReflType
        {
            std::string	    stage;
            std::string		module;
            std::string		name;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineShaderState) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(ShaderStageFlags, v, stage);
            return { stage, v.module, v.name };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineShaderState)& v)
        {
            REFLECTOR_FROM_ENUM(ShaderStageFlags, v, stage);
            return { stage, v.module, v.name };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineInputAssemblyState)>
    {
        struct ReflType
        {
            std::string	    topology;
            uint32_t	    primitive_restart_enable;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineInputAssemblyState) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(PrimitiveTopology, v, topology);
            return { topology, v.primitive_restart_enable };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineInputAssemblyState)& v)
        {
            REFLECTOR_FROM_ENUM(PrimitiveTopology, v, topology);
            return { topology, v.primitive_restart_enable };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineViewportState)>
    {
        struct ReflType
        {
            std::vector<ENGINE_NAMESPACE_CONCAT(Viewport)>    viewports;
            std::vector<ENGINE_NAMESPACE_CONCAT(Rect2D)>      scissors;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineViewportState) to(const ReflType& v) noexcept
        {
            return { v.viewports, v.scissors };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineViewportState)& v)
        {
            return { v.viewports, v.scissors };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineRasterizationState)>
    {
        struct ReflType
        {
            uint32_t		        depth_clamp_enable;
            uint32_t		        rasterizer_discard_enable;
            std::string             polygon_mode;
            std::string             cull_mode;
            std::string             front_face;
            uint32_t		        depth_bias_enable;
            float			        depth_bias_constant_factor;
            float			        depth_bias_clamp;
            float			        depth_bias_slope_factor;
            float			        line_width;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineRasterizationState) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(PolygonMode, v, polygon_mode);
            REFLECTOR_TO_ENUM(CullModeFlags, v, cull_mode);
            REFLECTOR_TO_ENUM(FrontFace, v, front_face);
            return { v.depth_clamp_enable, v.rasterizer_discard_enable, polygon_mode, cull_mode, front_face, v.depth_bias_enable, v.depth_bias_constant_factor, v.depth_bias_clamp, v.depth_bias_slope_factor, v.line_width };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineRasterizationState)& v)
        {
            REFLECTOR_FROM_ENUM(PolygonMode, v, polygon_mode);
            REFLECTOR_FROM_ENUM(CullModeFlags, v, cull_mode);
            REFLECTOR_FROM_ENUM(FrontFace, v, front_face);
            return { v.depth_clamp_enable, v.rasterizer_discard_enable, polygon_mode, cull_mode, front_face, v.depth_bias_enable, v.depth_bias_constant_factor, v.depth_bias_clamp, v.depth_bias_slope_factor, v.line_width };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineMultisampleState)>
    {
        struct ReflType
        {
            std::string	        rasterization_samples;
            uint32_t			sample_shading_enable;
            float				min_sample_shading;
            // std::vector<uint32_t>	sample_mask;
            uint32_t			alpha_to_coverage_enable;
            uint32_t			alpha_to_one_enable;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineMultisampleState) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(SampleCountFlags, v, rasterization_samples);
            return { rasterization_samples, v.sample_shading_enable, v.min_sample_shading, v.alpha_to_coverage_enable, v.alpha_to_one_enable };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineMultisampleState)& v)
        {
            REFLECTOR_FROM_ENUM(SampleCountFlags, v, rasterization_samples);
            return { rasterization_samples, v.sample_shading_enable, v.min_sample_shading, v.alpha_to_coverage_enable, v.alpha_to_one_enable };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineDepthStencilState)>
    {
        struct ReflType
        {
            uint32_t		                                depth_test_enable;
            uint32_t		                                depth_write_enable;
            std::string		                                depth_compare_op;
            uint32_t		                                depth_bounds_test_enable;
            uint32_t		                                stencil_test_enable;
            ENGINE_NAMESPACE_CONCAT(StencilOpState)         front;
            ENGINE_NAMESPACE_CONCAT(StencilOpState)         back;
            float			                                min_depth_bounds;
            float			                                max_depth_bounds;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineDepthStencilState) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(CompareOp, v, depth_compare_op);
            return { v.depth_test_enable, v.depth_write_enable, depth_compare_op, v.depth_bounds_test_enable, v.stencil_test_enable, v.front, v.back, v.min_depth_bounds, v.max_depth_bounds };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineDepthStencilState)& v)
        {
            REFLECTOR_FROM_ENUM(CompareOp, v, depth_compare_op);
            return { v.depth_test_enable, v.depth_write_enable, depth_compare_op, v.depth_bounds_test_enable, v.stencil_test_enable, v.front, v.back, v.min_depth_bounds, v.max_depth_bounds };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineColorBlendAttachmentState)>
    {
        struct ReflType
        {
            uint32_t		blend_enable;
            std::string	    src_color_blend_factor;
            std::string	    dst_color_blend_factor;
            std::string	    color_blend_op;
            std::string	    src_alpha_blend_factor;
            std::string	    dst_alpha_blend_factor;
            std::string	    alpha_blend_op;
            std::string 	color_write_mask;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineColorBlendAttachmentState) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(BlendFactor, v, src_color_blend_factor);
            REFLECTOR_TO_ENUM(BlendFactor, v, dst_color_blend_factor);
            REFLECTOR_TO_ENUM(BlendOp, v, color_blend_op);
            REFLECTOR_TO_ENUM(BlendFactor, v, src_alpha_blend_factor);
            REFLECTOR_TO_ENUM(BlendFactor, v, dst_alpha_blend_factor);
            REFLECTOR_TO_ENUM(BlendOp, v, alpha_blend_op);
            MATCH_TO_ENUM(ColorComponentFlags, v, color_write_mask);
            return { v.blend_enable, src_color_blend_factor, dst_color_blend_factor, color_blend_op, src_alpha_blend_factor, dst_alpha_blend_factor, alpha_blend_op, color_write_mask };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineColorBlendAttachmentState)& v)
        {
            REFLECTOR_FROM_ENUM(BlendFactor, v, src_color_blend_factor);
            REFLECTOR_FROM_ENUM(BlendFactor, v, dst_color_blend_factor);
            REFLECTOR_FROM_ENUM(BlendOp, v, color_blend_op);
            REFLECTOR_FROM_ENUM(BlendFactor, v, src_alpha_blend_factor);
            REFLECTOR_FROM_ENUM(BlendFactor, v, dst_alpha_blend_factor);
            REFLECTOR_FROM_ENUM(BlendOp, v, alpha_blend_op);
            MATCH_FROM_ENUM(ColorComponentFlags, v, color_write_mask);
            return { v.blend_enable, src_color_blend_factor, dst_color_blend_factor, color_blend_op, src_alpha_blend_factor, dst_alpha_blend_factor, alpha_blend_op, color_write_mask };
        }
    };

    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineColorBlendState)>
    {
        struct ReflType
        {
            uint32_t												                    logic_op_enable;
            std::string 											                    logic_op;
            std::vector<ENGINE_NAMESPACE_CONCAT(PipelineColorBlendAttachmentState)>   color_blend_attachments;
            std::array<float, 4>								                        blend_constants;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineColorBlendState) to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(LogicOp, v, logic_op);
            return { v.logic_op_enable, logic_op, v.color_blend_attachments, v.blend_constants };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineColorBlendState)& v)
        {
            REFLECTOR_FROM_ENUM(LogicOp, v, logic_op);
            return { v.logic_op_enable, logic_op, v.color_blend_attachments, v.blend_constants };
        }
    };

    // self
    template <>
    struct Reflector<ENGINE_NAMESPACE_CONCAT(PipelineInfo)>
    {
        struct ReflType
        {
            std::vector<ENGINE_NAMESPACE_CONCAT(PipelineShaderState)>               shader_state;
            std::optional<ENGINE_NAMESPACE_CONCAT(PipelineInputAssemblyState)>      input_assembly_state;
            std::optional<ENGINE_NAMESPACE_CONCAT(PipelineViewportState)>           viewport_state;
            std::optional<ENGINE_NAMESPACE_CONCAT(PipelineRasterizationState)>      rasterization_state;
            std::optional<ENGINE_NAMESPACE_CONCAT(PipelineMultisampleState)>        multisample_state;
            std::optional<ENGINE_NAMESPACE_CONCAT(PipelineDepthStencilState)>       depth_stencil_state;
            std::optional<ENGINE_NAMESPACE_CONCAT(PipelineColorBlendState)>         color_blend_state;
            std::vector<std::string>			                                    dynamic_state;
            std::string                                                             layout;
            std::string                                                             render_pass;
        };

        static ENGINE_NAMESPACE_CONCAT(PipelineInfo) to(const ReflType& v) noexcept
        {
            std::vector<ENGINE_NAMESPACE_CONCAT(DynamicState)> dynamic_states;
            for (auto& dynamic_state : v.dynamic_state)
                dynamic_states.emplace_back(ENGINE_NAMESPACE_CONCAT(string_to_enum<ENGINE_NAMESPACE_CONCAT(DynamicStateEnum)>(dynamic_state)));
            return { v.shader_state, v.input_assembly_state, v.viewport_state, v.rasterization_state, v.multisample_state, v.depth_stencil_state, v.color_blend_state, dynamic_states, v.layout, v.render_pass };
        }

        static ReflType from(const ENGINE_NAMESPACE_CONCAT(PipelineInfo)& v)
        {
            std::vector<std::string> dynamic_states;
            for (auto& dynamic_state : v.dynamic_state)
                dynamic_states.emplace_back(ENGINE_NAMESPACE_CONCAT(enum_to_string(static_cast<ENGINE_NAMESPACE_CONCAT(DynamicStateEnum)>(dynamic_state))));
            return { v.shader_state, v.input_assembly_state, v.viewport_state, v.rasterization_state, v.multisample_state, v.depth_stencil_state, v.color_blend_state, dynamic_states, v.layout, v.render_pass };
        }
    };

}

#undef REFLECTOR_TO_ENUM
#undef REFLECTOR_FROM_ENUM
#undef MATCH_TO_ENUM
#undef MATCH_FROM_ENUM