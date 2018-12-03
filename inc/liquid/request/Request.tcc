#pragma once

#include "liquid/request/Request.h"

namespace liquid::request
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

} // namespace liquid::request
