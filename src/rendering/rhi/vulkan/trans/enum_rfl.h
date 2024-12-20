//
// Created by AmazingBuff on 2024/12/20.
//

#pragma once

#define YAML_CPP_STATIC_DEFINE
#include <rfl.hpp>
#include <rfl/yaml.hpp>
#include <func.h>

#include "enum_map.h"

namespace rfl
{
#define REFLECTOR_TO_ENUM(ty, value, name)   \
Amazing::Engine::ty name(Amazing::Engine::string_to_enum<Amazing::Trait::head_type_t<typename Amazing::Trait::template_traits<Amazing::Engine::ty>::type>>(value.name))
#define REFLECTOR_FROM_ENUM(ty, value, name)  \
std::string name = Amazing::Engine::enum_to_string(static_cast<Amazing::Trait::head_type_t<typename Amazing::Trait::template_traits<Amazing::Engine::ty>::type>>(value.name))

    template <>
    struct Reflector<Amazing::Engine::SampleCountFlagBits>
    {
        struct ReflType
        {
            std::string   value;
        };

        static Amazing::Engine::SampleCountFlagBits to(const ReflType& v) noexcept
        {
            REFLECTOR_TO_ENUM(SampleCountFlagBits, v, fail_op);
            return { v.x, v.y };
        }

        static ReflType from(const Amazing::Engine::SampleCountFlagBits& v)
        {
            REFLECTOR_FROM_ENUM(StencilOp, v, fail_op);

            return { v.x, v.y };
        }
    };




}
