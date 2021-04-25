#pragma once

#include "turbohttp/method.hpp"
#include "turbohttp/version.hpp"

#include <string>
#include <optional>
#include <array>
#include <span>

// The cmake build system will define this and allows overriding.
#ifndef TURBOHTTP_HEADER_COUNT
#define TURBOHTTP_HEADER_COUNT 16
#endif

namespace turbo::http
{

enum class body_type
{
    no_body,
    chunked,
    content_length
};

enum class request_parse_result
{
    /// Go to the next stage of parsing.
    advance,
    /// Parsing this request is completed, all fields are appropriately set.
    complete,
    /// More data is required to complete parsing this request.
    incomplete,
    /// The method in the request is unknown, error parse result.
    method_unknown,
    /// The http version in the request is malformed, error parse result.
    http_version_malformed,
    /// The http version is unknown or unsupported, error parse result.
    http_version_unknown,
    /// The maximum number of headers has been exceeded, error parse result.
    maximum_headers_exceeded,
    /// A malformed chunk was encountered, error parse result.
    chunk_malformed
};

enum class request_parse_state
{
    /// Parsing will start from the beginning of the request.
    start,
    /// The http method has been parsed.
    parsed_method,
    /// The http uri has been parsed.
    parsed_uri,
    /// The http version has been parsed.
    parsed_version,
    /// The http headers have been parsed.
    parsed_headers,
    /// The http body (if present) has been parsed.
    parsed_body
};

template<std::size_t header_count = TURBOHTTP_HEADER_COUNT>
class request
{
public:
    request() = default;
    ~request() = default;

    request(const request&) = default;
    request(request&&) = default;
    auto operator=(const request&) -> request& = default;
    auto operator=(request&&) -> request& = default;

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
    auto parse(std::string& data) -> request_parse_result;

    auto parse(std::span<char>& data) -> request_parse_result;

private:
    auto parse_method(std::span<char>& data) -> request_parse_result;
    auto parse_uri(std::span<char>& data) -> request_parse_result;
    auto parse_version(std::span<char>& data) -> request_parse_result;
    auto parse_headers(std::span<char>& data) -> request_parse_result;
    auto parse_body(std::span<char>& data) -> request_parse_result;
public:

    /**
     * Resets the parser to default constructed state to parse another request.
     */
    auto reset() -> void;

    /**
     * @return The current internal parse state (how far its gotten) for the current set of data.
     */
    auto state() const -> request_parse_state { return m_parse_state; }

    /**
     * @return The parsed HTTP Method.  This value is only valid if the parser has successfully
     *          passed the 'PARSED_METHOD' parse state.
     */
    auto http_method() const -> method { return m_method; }

    /**
     * @return The parsed URI.  This value is only valid if the parser has successfully
     *          passed the 'PARSED_URI' parse state.
     */
    auto http_uri() const -> std::string_view { return m_uri; };

    /**
     * @return The parsed HTTP Version.  This value is only valid if the parser has succesfully
     *          passed the 'PARSED_VERSION' parse state.
     */
    auto http_version() const -> version { return m_version; }

    /**
     * @return Gets the number of parsed headers.
     */
    auto http_header_count() const -> size_t { return m_header_count; }

    /**
     * Finds the first header given by name (case insensitive).
     * @param name Find this header's value.
     * @return The value if it was in the request, otherwise an empty optional.
     */
    auto http_header(std::string_view name) const -> std::optional<std::string_view>;

    /**
     * Iterates over each request header with the (name, value) pair as std::string_view arguments.
     * @tparam Functor [](std::string_view name, std::string_view value) -> void;
     * @param functor Callback functor to be called on each header name, value pair.
     */
    template<typename Functor>
    auto http_header_for_each(Functor&& functor) -> void
    {
        for(size_t i = 0; i < m_header_count; ++i)
        {
            auto& [name, value] = m_headers[i];
            functor(name, value);
        }
    }

    /**
     * @return Gets the request body if the request had one.
     */
    auto http_body() const -> const std::optional<std::string_view>& { return m_body; }

private:
    /// How far in the parse state machine has this data gotten?
    request_parse_state m_parse_state{request_parse_state::start};
    /// The exact index of where the previous Parse() call was left off at.
    std::size_t m_pos{0};

    /// The parsed HTTP Method.
    method m_method{method::get};

    /// The starting position of the URI, saved during subsequent parses to calculate the full view.
    std::size_t m_uri_start_pos{0};
    /// The parsed URI.
    std::string_view m_uri{};

    /// The parsed HTTP/X.Y version.
    version m_version{version::v1_1};

    /// The number of headers in the request.
    std::size_t m_header_count{0};
    /// The actual contents of the header values.
    std::array<std::pair<std::string_view, std::string_view>, header_count> m_headers{};

    /// The type of body, if there is one.
    body_type m_body_type{body_type::no_body};
    /// The Content-Length value if present.
    std::size_t m_content_length{0};
    /// The start of the body (used for Transfer-Encoding: chunked)
    std::size_t m_body_start{0};
    /// The request body contents if any.
    std::optional<std::string_view> m_body{};
};

enum class response_parse_result
{
    advance,
    complete,
    incomplete,
    http_version_malformed,
    http_version_unknown,
    http_status_code_malformed,
    maximum_headers_exceeded,
    chunk_malformed
};

enum class response_parse_state
{
    start,
    parsed_version,
    parsed_status_code,
    parsed_reason_phrase,
    parsed_headers,
    parsed_body
};

template<std::size_t header_count = TURBOHTTP_HEADER_COUNT>
class response
{
public:
    response() = default;
    ~response() = default;

    response(const response&) = default;
    response(response&&) = default;
    auto operator=(const response&) -> response& = default;
    auto operator=(response&&) -> response& = default;

    auto parse(std::string& data) -> response_parse_result;
    auto parse(std::span<char>& data) -> response_parse_result;
private:
    auto parse_version(std::span<char>& data) -> response_parse_result;
    auto parse_status_code(std::span<char>& data) -> response_parse_result;
    auto parse_reason_phrase(std::span<char>& data) -> response_parse_result;
    auto parse_headers(std::span<char>& data) -> response_parse_result;
    auto parse_body(std::span<char>& data) -> response_parse_result;
public:

    /**
     * Resets the parser to default constructed state to parse another response.
     */
    auto reset() -> void;

    /**
     * @return The current internal parse state (how far its gotten) for the current set of data.
     */
    auto state() const -> response_parse_state { return m_parse_state; }

    /**
     * @return Gets the HTTP Version of the response.
     */
    auto http_version() const -> version { return m_version; }

    /**
     * @return Gets the HTTP Status Code of the response.
     */
    auto http_status_code() const -> uint64_t { return m_status_code; }

    /**
     * @return Gets the HTTP Reason Phrase of the response.
     */
    auto http_reason_phrase() const -> std::string_view { return m_reason_phrase; }

    /**
     * @return Gets the number of parsed headers.
     */
    auto http_header_count() const -> std::size_t { return m_header_count; }

    /**
     * Finds the first header given by name (case insensitive).
     * @param name Find this header's value.
     * @return The value if it was in the response, otherwise an empty optional.
     */
    auto http_header(std::string_view name) const -> std::optional<std::string_view>;

    /**
     * Iterates over each response header with the (name, value) pair as std::string_view arguments.
     * @tparam Functor [](std::string_view name, std::string_view value) -> void;
     * @param functor Callback functor to be called on each header name, value pair.
     */
    template<typename Functor>
    auto http_header_for_each(Functor&& functor) -> void
    {
        for(size_t i = 0; i < m_header_count; ++i)
        {
            auto& [name, value] = m_headers[i];
            functor(name, value);
        }
    }

    /**
     * @return Gets the response body if the response had one.
     */
    auto http_body() const -> const std::optional<std::string_view>& { return m_body; }


private:
    /// How far in the parse state machine has this data gotten?
    response_parse_state m_parse_state{response_parse_state::start};
    /// The exact index of where the previous Parse() call was left off at.
    std::size_t m_pos{0};

    /// The parsed HTTP/X.Y version.
    version m_version{version::v1_1};
    /// The HTTP response status code.
    uint64_t m_status_code{0};
    /// The HTTP Reason Phrase.
    std::string_view m_reason_phrase{};

    /// The number of headers in the response.
    std::size_t m_header_count{0};
    /// The actual contents of the header values.
    std::array<std::pair<std::string_view, std::string_view>, header_count> m_headers;

    /// The type of body, if there is one.
    body_type m_body_type{body_type::no_body};
    /// The Content-Length value if present.
    std::size_t m_content_length{0};
    /// The start of the body (used for Transfer-Encoding: chunked)
    std::size_t m_body_start{0};
    /// The response body contents if any.
    std::optional<std::string_view> m_body{};
};

} // namespace turbo::http

#include "turbohttp/parser.tcc"

#undef TURBOHTTP_HEADER_COUNT
