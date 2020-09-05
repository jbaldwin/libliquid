#pragma once

#include "liquid/method.hpp"
#include "liquid/version.hpp"

#include <string>
#include <optional>
#include <array>

namespace liquid
{

enum class body_type
{
    no_body,
    chunked,
    content_length
};

class request
{
public:
    enum class parse_result
    {
        advance,
        complete,
        incomplete,
        method_unknown,
        http_version_malformed,
        http_version_unknown,
        maximum_headers_exceeded,
        chunk_malformed
    };

    enum class parse_state
    {
        start,
        parsed_method,
        parsed_url,
        parsed_version,
        parsed_headers,
        parsed_body
    };

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
    auto parse(std::string& data) -> request::parse_result;

private:
    auto parse_method(std::string& data) -> request::parse_result;
    auto parse_uri(std::string& data) -> request::parse_result;
    auto parse_version(std::string& data) -> request::parse_result;
    auto parse_headers(std::string& data) -> request::parse_result;
    auto parse_body(std::string& data) -> request::parse_result;
public:

    /**
     * Resets the parser to default constructed state to parse another request.
     */
    auto reset() -> void;

    /**
     * @return The current internal parse state (how far its gotten) for the current set of data.
     */
    auto state() const -> request::parse_state { return m_parse_state; }

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
    request::parse_state m_parse_state{request::parse_state::start};
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
    std::array<std::pair<std::string_view, std::string_view>, 64> m_headers{};

    /// The type of body, if there is one.
    body_type m_body_type{body_type::no_body};
    /// The Content-Length value if present.
    std::size_t m_content_length{0};
    /// The start of the body (used for Transfer-Encoding: chunked)
    std::size_t m_body_start{0};
    /// The request body contents if any.
    std::optional<std::string_view> m_body{};
};

class response
{
public:
    enum class parse_result
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

    enum class parse_state
    {
        start,
        parsed_version,
        parsed_status_code,
        parsed_reason_phrase,
        parsed_headers,
        parsed_body
    };

    response() = default;
    ~response() = default;

    response(const response&) = default;
    response(response&&) = default;
    auto operator=(const response&) -> response& = default;
    auto operator=(response&&) -> response& = default;

    auto parse(std::string& data) -> response::parse_result;
private:
    auto parse_version(std::string& data) -> response::parse_result;
    auto parse_status_code(std::string& data) -> response::parse_result;
    auto parse_reason_phrase(std::string& data) -> response::parse_result;
    auto parse_headers(std::string& data) -> response::parse_result;
    auto parse_body(std::string& data) -> response::parse_result;
public:

    /**
     * Resets the parser to default constructed state to parse another response.
     */
    auto reset() -> void;

    /**
     * @return The current internal parse state (how far its gotten) for the current set of data.
     */
    auto state() const -> response::parse_state { return m_parse_state; }

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
    response::parse_state m_parse_state{response::parse_state::start};
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
    std::array<std::pair<std::string_view, std::string_view>, 64> m_headers;

    /// The type of body, if there is one.
    body_type m_body_type{body_type::no_body};
    /// The Content-Length value if present.
    std::size_t m_content_length{0};
    /// The start of the body (used for Transfer-Encoding: chunked)
    std::size_t m_body_start{0};
    /// The response body contents if any.
    std::optional<std::string_view> m_body{};
};

} // namespace liquid
