#include "liquid/StringUtil.h"

#include <string>

namespace liquid
{

auto string_view_icompare(std::string_view a, std::string_view b) -> bool
{
    if(a.length() != b.length())
    {
        return false;
    }

    for(size_t i = 0; i < a.length(); ++i)
    {
        if(std::tolower(a[i]) != std::tolower(b[i]))
        {
            return false;
        }
    }

    return true;
}

} // namespace liquid