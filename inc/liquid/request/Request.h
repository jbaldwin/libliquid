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
    CONTINUE,
    COMPLETE,
    INCOMPLETE,
    METHOD_UNKNOWN,
    HTTP_VERSION_MALFORMED,
    HTTP_VERSION_UNKNOWN,
    HEADERS_MALFORMED,
    TOO_MANY_HEADERS,
    CHUNK_MALFORMED
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

enum class BodyType
{
    CHUNKED,
    CONTENT_LENGTH,
    END_OF_STREAM
};

class Request
{
public:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
    Request() = default;
#pragma GCC diagnostic pop
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
     * If the header Transfer-Encoding: chunked is received then the parser
     * will de-chunk the data.  That means the 'data' parameter is not truely const
     * and should be modifiable.
     *
     * @param data The full HTTP request data.  If this is a chunked request, it will be mutated.
     * @return The current state of parsing the HTTP request data.
     */
    auto Parse(std::string& data) -> ParseResult;

private:
    auto parseMethod(std::string& data) -> ParseResult;
    auto parseUri(std::string& data) -> ParseResult;
    auto parseVersion(std::string& data) -> ParseResult;
    auto parseHeaders(std::string& data) -> ParseResult;
    auto parseBody(std::string& data) -> ParseResult;
public:

    /**
     * Resets the parser to default constructed state to parse another request.
     */
    auto Reset() -> void;

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

    /**
     * @return Gets the number of parsed headers.
     */
    auto GetHeaderCount() const -> size_t;

    /**
     * Finds the first header given by name (case insensitive).
     * @param name Find this header's value.
     * @return The value if it was in the request, otherwise an empty optional.
     */
    auto GetHeader(std::string_view name) const -> std::optional<std::string_view>;

    /**
     * Iterates over each request header with the (name, value) pair as std::string_view arguments.
     * @tparam Functor [](std::string_view name, std::string_view value) -> void;
     * @param functor Callback functor to be called on each header name, value pair.
     */
    template<typename Functor>
    auto ForEachHeader(Functor&& functor) -> void;

    /**
     * @return Gets the request body if the request had one.
     */
    auto GetBody() const -> const std::optional<std::string_view>&;
private:
    /// How far in the parse state machine has this data gotten?
    ParseState m_parse_state{ParseState::START};
    /// The exact index of where the previous Parse() call was left off at.
    size_t m_pos{0};

    /// The parsed HTTP Method.
    Method m_method{Method::GET};

    /// The starting position of the URI, saved during subsequent parses to calculate the full view.
    size_t m_uri_start_pos{0};
    /// The parsed URI.
    std::string_view m_uri;

    /// The parsed HTTP/X.Y version.
    Version m_version{Version::V1_1};

    /// The number of headers in the request.
    size_t m_header_count{0};
    /// The actual contents of the header values.
    std::array<std::pair<std::string_view, std::string_view>, 64> m_headers;

    /// The type of body, if there is one.
    BodyType m_body_type{BodyType::END_OF_STREAM};
    /// The Content-Length value if present.
    size_t m_content_length;
    /// The start of the body (used for Transfer-Encoding: chunked)
    size_t m_body_start;
    /// The request body contents if any.
    std::optional<std::string_view> m_body;
};

} // namespace liquid::request

#include "liquid/request/Request.tcc"
