#include "liquid/Method.h"

namespace liquid
{

using namespace std::string_literals;

static const std::string METHOD_GET     = "GET"s;
static const std::string METHOD_HEAD    = "HEAD"s;
static const std::string METHOD_POST    = "POST"s;
static const std::string METHOD_PUT     = "PUT"s;
static const std::string METHOD_DELETE  = "DELETE"s;
static const std::string METHOD_CONNECT = "CONNECT"s;
static const std::string METHOD_OPTIONS = "OPTIONS"s;
static const std::string METHOD_TRACE   = "TRACE"s;
static const std::string METHOD_PATCH   = "PATCH"s;
static const std::string METHOD_INVALID = "INVALID"s;

auto to_string(Method method) -> const std::string&
{
    switch(method)
    {
        case Method::GET:
            return METHOD_GET;
        case Method::HEAD:
            return METHOD_HEAD;
        case Method::POST:
            return METHOD_POST;
        case Method::PUT:
            return METHOD_PUT;
        case Method::DELETE:
            return METHOD_DELETE;
        case Method::CONNECT:
            return METHOD_CONNECT;
        case Method::OPTIONS:
            return METHOD_OPTIONS;
        case Method::TRACE:
            return METHOD_TRACE;
        case Method::PATCH:
            return METHOD_PATCH;
        default:
            return METHOD_INVALID;
    }
}

} // namespace liquid
