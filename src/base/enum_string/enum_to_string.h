//
// Created by AmazingBuff on 2024/12/12.
//

#pragma once

#include "base/macro.h"
#include <source_location>

AMAZING_NAMESPACE_BEGIN

namespace detail
{
    template <auto V>
    consteval std::string_view to_string()
    {
#if defined(_MSC_VER)
        return std::string_view(__FUNCSIG__);
#elif defined(__clang__) || defined(__GNUC__)
        return std::string_view(__PRETTY_FUNCTION__);
#else
        return std::string_view(std::source_location::current().function_name());
#endif
    }

    consteval std::pair<size_t, size_t> find_name_pos()
    {
        constexpr std::string_view result = to_string<nullptr>();
        constexpr size_t left_omitted = result.find("nullptr");
        constexpr size_t right_omitted = result.size() - left_omitted - 7;
        return std::pair(left_omitted, right_omitted);
    }
}

template <auto V, size_t Offset = 0>
    requires (std::is_enum_v<decltype(V)>)
consteval std::string_view enum_to_string()
{
    const std::string_view function_str = detail::to_string<V>();
    const auto [l, r] = detail::find_name_pos();
    const std::string_view value_str = function_str.substr(l, function_str.size() - l - r);
    // from back to top
    const size_t scope_idx = value_str.find_last_of("::");
    if (scope_idx != std::string_view::npos)
        return value_str.substr(scope_idx + 1 + Offset);
    else
        return value_str.substr(Offset);
}


AMAZING_NAMESPACE_END