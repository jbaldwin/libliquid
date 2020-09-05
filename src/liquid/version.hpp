#pragma once

#include <string>

namespace liquid
{

enum class version
{
    /// HTTP/1.0
    v1_0,
    /// HTTP/1.1
    v1_1
};

inline static const std::string version_unknown = "UNKNOWN";
inline static const std::string version_v1_0    = "1.0";
inline static const std::string version_v1_1    = "1.1";

inline auto to_string(version v) -> const std::string&
{
    switch(v)
    {
        case version::v1_0:
            return version_v1_0;
        case version::v1_1:
            return version_v1_1;
        default:
            return version_unknown;
    }
}

} // namespace liquid
