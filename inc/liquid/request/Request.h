#pragma once

#include "liquid/Method.h"
#include "liquid/Version.h"

#include <string>
#include <optional>
#include <array>

namespace liquid::request
{

enum class ParseResult
{
    COMPLETE,
    INCOMPLETE,
    METHOD_UNKNOWN,
    HTTP_VERSION_MALFORMED,
    HTTP_VERSION_UNKNOWN
};

enum class ParseState
{
    START,
    PARSED_METHOD,
    PARSED_URI,
    PARSED_VERSION,
    PARSED_HEADERS,
    PARSED_BODY
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

    /**
     * A stateful parse function which will return the status of how far the parse
     * was able to complete.  If 'INCOMPLETE" is returned then additional data
     * is expected, and should be appended to the data view passed in in subsequent calls.
     *
     * This parser currently expects the chunks of data parsed to be contiguous.
     * This parser will not 're-parse' previously parsed sections of data, e.g. once
     * headers are parsed they are never parsed a second time.
     *
     * @param data The full HTTP request data.
     * @return The current state of parsing the HTTP request data.
     */
    auto Parse(std::string_view data) -> ParseResult;

    /**
     * @return The current internal parse state (how far its gotten) for the current set of data.
     */
    auto GetParseState() const -> ParseState;

    /**
     * @return The parsed HTTP Method.  This value is only valid if the parser has successfully
     *          passed the 'PARSED_METHOD' parse state.
     */
    auto GetMethod() const -> Method;

    /**
     * @return The parsed URI.  This value is only valid if the parser has successfully
     *          passed the 'PARSED_URI' parse state.
     */
    auto GetUri() const -> std::string_view;

    /**
     * @return The parsed HTTP Version.  This value is only valid if the parser has succesfully
     *          passed the 'PARSED_VERSION' parse state.
     */
    auto GetVersion() const -> Version;

private:
    /// How far in the parse state machine has this data gotten?
    ParseState m_parse_state{ParseState::START};
    /// The exact index of where the previous Parse() call was left off at.
    size_t m_pos{0};

    /// The parsed HTTP Method.
    Method m_method;

    /// The starting position of the URI, saved during subsequent parses to calculate the full view.
    size_t m_uri_start_pos{0};
    /// The parsed URI.
    std::string_view m_uri;

    /// The parsed HTTP/X.Y version.
    Version m_version;

};

} // namespace liquid::request
