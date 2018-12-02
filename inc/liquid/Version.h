#pragma once

#include <string>

namespace liquid
{

enum class Version
{
    /// HTTP/1.0
    V1_0,
    /// HTTP/1.1
    V1_1
};

auto to_string(Version version) -> const std::string&;

} // namespace liquid
