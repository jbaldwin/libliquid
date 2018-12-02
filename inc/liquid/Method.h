#pragma once

#include <string>

namespace liquid
{

enum class Method
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

auto to_string(Method method) -> const std::string&;

} // namespace liquid
