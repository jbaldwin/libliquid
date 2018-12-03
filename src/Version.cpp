#include "liquid/Version.h"

namespace liquid
{

using namespace std::string_literals;

static const std::string VERSION_V1_0 = "1.0"s;
static const std::string VERSION_V1_1 = "1.1"s;
static const std::string VERSION_INVALID = "INVALID"s;

auto to_string(Version version) -> const std::string&
{
    switch(version)
    {
        case Version::V1_0:
            return VERSION_V1_0;
        case Version::V1_1:
            return VERSION_V1_1;
        default:
            return VERSION_INVALID;
    }
}

} // namespace liquid
