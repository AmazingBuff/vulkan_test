#pragma once

#include <rfl.hpp>
#define YAML_CPP_STATIC_DEFINE
#include <rfl/yaml.hpp>

#include "enum_map.h"
#include "structure_trans.h"
#include "func.h"

namespace rfl 
{

#define REFLECTOR_TO_ENUM(ty, value, name)   \
            Amazing::Engine::ty name(Amazing::Engine::string_to_enum<Amazing::Trait::head_type_t<typename Amazing::Trait::template_traits<Amazing::Engine::ty>::type>>(value.name))
#define REFLECTOR_FROM_ENUM(type, value, name)  \
            std::string name = Amazing::Engine::enum_to_string(Amazing::Engine::type(value.name))

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
            REFLECTOR_FROM_ENUM(AttachmentTypeEnum, v, type);
            REFLECTOR_FROM_ENUM(FormatEnum, v, format);
            REFLECTOR_FROM_ENUM(SampleCountFlagBits, v, samples);
            REFLECTOR_FROM_ENUM(AttachmentLoadOpEnum, v, load_op);
            REFLECTOR_FROM_ENUM(AttachmentStoreOpEnum, v, store_op);
            REFLECTOR_FROM_ENUM(AttachmentLoadOpEnum, v, stencil_load_op);
            REFLECTOR_FROM_ENUM(AttachmentStoreOpEnum, v, stencil_store_op);
            REFLECTOR_FROM_ENUM(ImageLayoutEnum, v, initial_layout);
            REFLECTOR_FROM_ENUM(ImageLayoutEnum, v, final_layout);
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
            REFLECTOR_FROM_ENUM(AttachmentTypeEnum, v, type);
            REFLECTOR_FROM_ENUM(ImageLayoutEnum, v, layout);
            return { type, v.attachment, layout };
        }
    };


    template <>
    struct Reflector<Amazing::Engine::SubpassDescription>
    {
        struct ReflType
        {
            std::string	                                        name;
            std::string	                                        pipeline_bind_point;
            std::vector<Amazing::Engine::AttachmentReference>	input_attachments;
            std::vector<Amazing::Engine::AttachmentReference>	color_attachments;
            std::vector<Amazing::Engine::AttachmentReference>	resolve_attachments;
            Amazing::Engine::AttachmentReference                depth_stencil_attachment;
            std::vector<uint32_t>				                preserve_attachments;
        };

        static Amazing::Engine::SubpassDescription to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(PipelineBindPoint, v, pipeline_bind_point);
            return { v.name, pipeline_bind_point, v.input_attachments, v.color_attachments, v.resolve_attachments, v.depth_stencil_attachment, v.preserve_attachments };
        }

        static ReflType from(const Amazing::Engine::SubpassDescription& v)
        {
            REFLECTOR_FROM_ENUM(PipelineBindPointEnum, v, pipeline_bind_point);
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
            REFLECTOR_FROM_ENUM(PipelineStageFlagBits, v, src_stage_mask);
            REFLECTOR_FROM_ENUM(PipelineStageFlagBits, v, dst_stage_mask);
            REFLECTOR_FROM_ENUM(AccessFlagBits, v, src_access_mask);
            REFLECTOR_FROM_ENUM(AccessFlagBits, v, dst_access_mask);
            REFLECTOR_FROM_ENUM(DependencyFlagBits, v, dependency_flags);
            std::string src_subpass = v.src_subpass == VK_SUBPASS_EXTERNAL ? "external" : std::to_string(v.src_subpass);
            std::string dst_subpass = v.dst_subpass == VK_SUBPASS_EXTERNAL ? "external" : std::to_string(v.dst_subpass);
            return { v.name, src_subpass, dst_subpass, src_stage_mask, dst_stage_mask, src_access_mask, dst_access_mask, dependency_flags };
        }
    };


#undef REFLECTOR_TO_ENUM
#undef REFLECTOR_FROM_ENUM
}