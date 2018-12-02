#pragma once

#include "liquid/Method.h"

#include <string>
#include <optional>
#include <array>

namespace liquid::request
{

enum class ParseResult
{
    COMPLETE,
    INCOMPLETE,
    UNKNOWN_METHOD
};

enum class ParseState
{
    START,
    METHOD_PARSED,
    RESOURCE,
    VERSION,
    HEADERS,
    BODY
};

class Request
{
public:
    Request() = default;
    ~Request() = default;

    Request(const Request&) = default;
    Request(Request&&) = default;
    auto operator=(const Request&) -> Request& = default;
    auto operator=(Request&&) -> Request& = default;

    auto Parse(std::string_view data) -> ParseResult;
    auto GetParseState() const -> ParseState;

    auto GetMethod() const -> Method;

private:
    ParseState m_parse_state{ParseState::START};
    size_t m_pos{0};

    Method m_method;
};

} // namespace liquid::request
