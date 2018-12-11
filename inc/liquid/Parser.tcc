#pragma once

#include "liquid/Parser.h"

namespace liquid
{

template<typename Functor>
auto Request::ForEachHeader(Functor&& functor) -> void
{
    for(size_t i = 0; i < m_header_count; ++i)
    {
        auto& [name, value] = m_headers[i];
        functor(name, value);
    }
}

} // namespace liquid
