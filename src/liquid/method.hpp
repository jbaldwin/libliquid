#pragma once

#include <string>

namespace liquid
{

enum class method
{
    get,
    head,
    post,
    put,
    http_delete, // 'delete' is reserved.
    connect,
    options,
    trace,
    patch
};

inline static const std::string method_unknown = "UNKNOWN";
inline static const std::string method_get     = "GET";
inline static const std::string method_head    = "HEAD";
inline static const std::string method_post    = "POST";
inline static const std::string method_put     = "PUT";
inline static const std::string method_delete  = "DELETE";
inline static const std::string method_connect = "CONNECT";
inline static const std::string method_options = "OPTIONS";
inline static const std::string method_trace   = "TRACE";
inline static const std::string method_patch   = "PATCH";

inline auto to_string(method m) -> const std::string&
{
    switch(m)
    {
        case method::get:
            return method_get;
        case method::head:
            return method_head;
        case method::post:
            return method_post;
        case method::put:
            return method_put;
        case method::http_delete:
            return method_delete;
        case method::connect:
            return method_connect;
        case method::options:
            return method_options;
        case method::trace:
            return method_trace;
        case method::patch:
            return method_patch;
        default:
            return method_unknown;
    }
}

} // namespace liquid
