#pragma once

#define YAML_CPP_STATIC_DEFINE
#include <rfl.hpp>
#include <rfl/yaml.hpp>
#include <func.h>

#include "enum_map.h"
#include "structure_trans.h"

namespace rfl 
{

#define REFLECTOR_TO_ENUM(ty, value, name)   \
            Amazing::Engine::ty name(Amazing::Engine::string_to_enum<Amazing::Trait::head_type_t<typename Amazing::Trait::template_traits<Amazing::Engine::ty>::type>>(value.name))
#define REFLECTOR_FROM_ENUM(ty, value, name)  \
            std::string name = Amazing::Engine::enum_to_string(static_cast<Amazing::Trait::head_type_t<typename Amazing::Trait::template_traits<Amazing::Engine::ty>::type>>(value.name))

    template <>
    struct Reflector<Amazing::Engine::Offset2D>
    {
        struct ReflType
        {
            int32_t x;
            int32_t y;
        };

        static Amazing::Engine::Offset2D to(const ReflType& v) noexcept
        {
            return { v.x, v.y };
        }

        static ReflType from(const Amazing::Engine::Offset2D& v)
        {
            return { v.x, v.y };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::Extent2D>
    {
        struct ReflType
        {
            uint32_t width;
            uint32_t height;
        };

        static Amazing::Engine::Extent2D to(const ReflType& v) noexcept
        {
            return { v.width, v.height };
        }

        static ReflType from(const Amazing::Engine::Extent2D& v)
        {
            return { v.width, v.height };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::Rect2D>
    {
        struct ReflType
        {
            Amazing::Engine::Offset2D   offset;
            Amazing::Engine::Extent2D   extent;
        };

        static Amazing::Engine::Rect2D to(const ReflType& v) noexcept
        {
            return { v.offset, v.extent };
        }

        static ReflType from(const Amazing::Engine::Rect2D& v)
        {
            return { v.offset, v.extent };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::Viewport>
    {
        struct ReflType
        {
            float    x;
            float    y;
            float    width;
            float    height;
            float    min_depth;
            float    max_depth;
        };

        static Amazing::Engine::Viewport to(const ReflType& v) noexcept
        {
            return { v.x, v.y, v.width, v.height, v.min_depth, v.max_depth };
        }

        static ReflType from(const Amazing::Engine::Viewport& v)
        {
            return { v.x, v.y, v.width, v.height, v.min_depth, v.max_depth };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::StencilOpState>
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

        static Amazing::Engine::StencilOpState to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(StencilOp, v, fail_op);
            REFLECTOR_TO_ENUM(StencilOp, v, pass_op);
            REFLECTOR_TO_ENUM(StencilOp, v, depth_fail_op);
            REFLECTOR_TO_ENUM(CompareOp, v, compare_op);
            return { fail_op, pass_op, depth_fail_op, compare_op, v.compare_mask, v.write_mask, v.reference };
        }

        static ReflType from(const Amazing::Engine::StencilOpState& v)
        {
            REFLECTOR_FROM_ENUM(StencilOp, v, fail_op);
            REFLECTOR_FROM_ENUM(StencilOp, v, pass_op);
            REFLECTOR_FROM_ENUM(StencilOp, v, depth_fail_op);
            REFLECTOR_FROM_ENUM(CompareOp, v, compare_op);
            return { fail_op, pass_op, depth_fail_op, compare_op, v.compare_mask, v.write_mask, v.reference };
        }
    };


    // render pass
    template <>
    struct Reflector<Amazing::Engine::AttachmentDescription>
    {
        struct ReflType
        {
            std::string	    type;
            std::string	    format;
            std::string	    samples;
            std::string	    load_op;
            std::string	    store_op;
            std::string	    stencil_load_op;
            std::string	    stencil_store_op;
            std::string	    initial_layout;
            std::string	    final_layout;
        };

        static Amazing::Engine::AttachmentDescription to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(AttachmentType, v, type);
            REFLECTOR_TO_ENUM(Format, v, format);
            REFLECTOR_TO_ENUM(SampleCountFlags, v, samples);
            REFLECTOR_TO_ENUM(AttachmentLoadOp, v, load_op);
            REFLECTOR_TO_ENUM(AttachmentStoreOp, v, store_op);
            REFLECTOR_TO_ENUM(AttachmentLoadOp, v, stencil_load_op);
            REFLECTOR_TO_ENUM(AttachmentStoreOp, v, stencil_store_op);
            REFLECTOR_TO_ENUM(ImageLayout, v, initial_layout);
            REFLECTOR_TO_ENUM(ImageLayout, v, final_layout);
            return { type, format, samples, load_op, store_op, stencil_load_op, stencil_store_op, initial_layout, final_layout };
        }

        static ReflType from(const Amazing::Engine::AttachmentDescription& v)
        {
            REFLECTOR_FROM_ENUM(AttachmentType, v, type);
            REFLECTOR_FROM_ENUM(Format, v, format);
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
    struct Reflector<Amazing::Engine::AttachmentReference>
    {
        struct ReflType
        {
            std::string	    type;
            uint32_t        attachment;
            std::string	    layout;
        };

        static Amazing::Engine::AttachmentReference to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(AttachmentType, v, type);
            REFLECTOR_TO_ENUM(ImageLayout, v, layout);
            return { type, v.attachment, layout };
        }

        static ReflType from(const Amazing::Engine::AttachmentReference& v)
        {
            REFLECTOR_FROM_ENUM(AttachmentType, v, type);
            REFLECTOR_FROM_ENUM(ImageLayout, v, layout);
            return { type, v.attachment, layout };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::SubpassDescription>
    {
        struct ReflType
        {
            std::string	                                            name;
            std::string	                                            pipeline_bind_point;
            std::vector<Amazing::Engine::AttachmentReference>	    input_attachments;
            std::vector<Amazing::Engine::AttachmentReference>	    color_attachments;
            std::vector<Amazing::Engine::AttachmentReference>	    resolve_attachments;
            std::optional<Amazing::Engine::AttachmentReference>     depth_stencil_attachment;
            std::vector<uint32_t>				                    preserve_attachments;
        };

        static Amazing::Engine::SubpassDescription to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(PipelineBindPoint, v, pipeline_bind_point);
            return { v.name, pipeline_bind_point, v.input_attachments, v.color_attachments, v.resolve_attachments, v.depth_stencil_attachment, v.preserve_attachments };
        }

        static ReflType from(const Amazing::Engine::SubpassDescription& v)
        {
            REFLECTOR_FROM_ENUM(PipelineBindPoint, v, pipeline_bind_point);
            return { v.name, pipeline_bind_point, v.input_attachments, v.color_attachments, v.resolve_attachments, v.depth_stencil_attachment, v.preserve_attachments };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::SubpassDependency>
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

        static Amazing::Engine::SubpassDependency to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(PipelineStageFlags, v, src_stage_mask);
            REFLECTOR_TO_ENUM(PipelineStageFlags, v, dst_stage_mask);
            REFLECTOR_TO_ENUM(AccessFlags, v, src_access_mask);
            REFLECTOR_TO_ENUM(AccessFlags, v, dst_access_mask);
            REFLECTOR_TO_ENUM(DependencyFlags, v, dependency_flags);
            uint32_t src_subpass = v.src_subpass == "external" ? VK_SUBPASS_EXTERNAL : std::stoul(v.src_subpass);
            uint32_t dst_subpass = v.dst_subpass == "external" ? VK_SUBPASS_EXTERNAL : std::stoul(v.dst_subpass);
            return { v.name, src_subpass, dst_subpass, src_stage_mask, dst_stage_mask, src_access_mask, dst_access_mask, dependency_flags };
        }

        static ReflType from(const Amazing::Engine::SubpassDependency& v)
        {
            REFLECTOR_FROM_ENUM(PipelineStageFlags, v, src_stage_mask);
            REFLECTOR_FROM_ENUM(PipelineStageFlags, v, dst_stage_mask);
            REFLECTOR_FROM_ENUM(AccessFlags, v, src_access_mask);
            REFLECTOR_FROM_ENUM(AccessFlags, v, dst_access_mask);
            REFLECTOR_FROM_ENUM(DependencyFlags, v, dependency_flags);
            std::string src_subpass = v.src_subpass == VK_SUBPASS_EXTERNAL ? "external" : std::to_string(v.src_subpass);
            std::string dst_subpass = v.dst_subpass == VK_SUBPASS_EXTERNAL ? "external" : std::to_string(v.dst_subpass);
            return { v.name, src_subpass, dst_subpass, src_stage_mask, dst_stage_mask, src_access_mask, dst_access_mask, dependency_flags };
        }
    };


    // pipeline
    template <>
    struct Reflector<Amazing::Engine::PipelineShaderState>
    {
        struct ReflType
        {
            std::string	    stage;
            std::string		module;
            std::string		name;
        };

        static Amazing::Engine::PipelineShaderState to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(ShaderStageFlags, v, stage);
            return { stage, v.module, v.name };
        }

        static ReflType from(const Amazing::Engine::PipelineShaderState& v)
        {
            REFLECTOR_FROM_ENUM(ShaderStageFlags, v, stage);
            return { stage, v.module, v.name };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::PipelineDynamicState>
    {
        struct ReflType
        {
            std::vector<std::string>	dynamic_states;
        };

        static Amazing::Engine::PipelineDynamicState to(const ReflType& v) noexcept
        {
            std::vector<Amazing::Engine::DynamicState> dynamic_states;
            for (auto& dynamic_state : v.dynamic_states)
                dynamic_states.emplace_back(Amazing::Engine::string_to_enum<Amazing::Engine::DynamicStateEnum>(dynamic_state));
            return { dynamic_states };
        }

        static ReflType from(const Amazing::Engine::PipelineDynamicState& v)
        {
            std::vector<std::string> dynamic_states;
            for (auto& dynamic_state : v.dynamic_states)
                dynamic_states.emplace_back(Amazing::Engine::enum_to_string(static_cast<Amazing::Engine::DynamicStateEnum>(dynamic_state)));
            return { dynamic_states };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::PipelineInputAssemblyState>
    {
        struct ReflType
        {
            std::string	                topology;
            Amazing::Engine::Bool		primitive_restart_enable;
        };

        static Amazing::Engine::PipelineInputAssemblyState to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(PrimitiveTopology, v, topology);
            return { topology, v.primitive_restart_enable };
        }

        static ReflType from(const Amazing::Engine::PipelineInputAssemblyState& v)
        {
            REFLECTOR_FROM_ENUM(PrimitiveTopology, v, topology);
            return { topology, v.primitive_restart_enable };
        }
    };

    template <>
    struct Reflector<Amazing::Engine::PipelineViewportState>
    {
        struct ReflType
        {
            std::vector<Amazing::Engine::Viewport>	viewports;
            std::vector<Amazing::Engine::Rect2D>	scissors;
        };

        static Amazing::Engine::PipelineViewportState to(const ReflType& v) noexcept
        {
            return { v.viewports, v.scissors };
        }

        static ReflType from(const Amazing::Engine::PipelineViewportState& v)
        {
            return { v.viewports, v.scissors };
        }
    };

    template <>
  struct Reflector<Amazing::Engine::PipelineRasterizerState>
    {
        struct ReflType
        {
            Amazing::Engine::Bool		depth_clamp_enable;
            Amazing::Engine::Bool		rasterizer_discard_enable;
            PolygonMode		polygon_mode;
            CullModeFlags	cull_mode;
            FrontFace		front_face;
            Amazing::Engine::Bool		depth_bias_enable;
            float			            depth_bias_constant_factor;
            float			            depth_bias_clamp;
            float			            depth_bias_slope_factor;
            float			            line_width;
        };

        static Amazing::Engine::PipelineRasterizerState to(const ReflType& v) noexcept
        {
            return { v.viewports, v.scissors };
        }

        static ReflType from(const Amazing::Engine::PipelineRasterizerState& v)
        {
            return { v.viewports, v.scissors };
        }
    };



#undef REFLECTOR_TO_ENUM
#undef REFLECTOR_FROM_ENUM
}