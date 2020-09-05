#include "turbohttp/parser.hpp"

#include <charconv>
#include <cstring>

#ifdef __SSE4_2__
#include <immintrin.h>
#endif

#define UNLIKELY(EXPR) __glibc_unlikely(EXPR)
#define LIKELY(EXPR) __glibc_likely(EXPR)

namespace turbo::http
{

static constexpr char HTTP_SP = ' ';
static constexpr char HTTP_CR = '\r';
static constexpr char HTTP_LF = '\n';
static constexpr char HTTP_HTAB = '\t';

#define EXPECT(C, E) { if(data[m_pos] != C) { return E; } }
#define ADVANCE_EXPECT(C, E) { if(data[++m_pos] != C) { return E; }}
#define ADVANCE() { ++m_pos; }

static constexpr int TOLOWER_TABLE_ADD[] = {
    0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,32,32,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

/**
 * Uses a lookup table to lowercase the incoming char.
 * Does not do bounds checking, e.g int > 255 could crash!
 * @param c ASCII to uppercase.
 * @return Uppercase'ed 'c'.
 */
inline auto tolower_asciitable_add(int c) -> int
{
    return c + TOLOWER_TABLE_ADD[c];
}

/**
 * Case insensitive equality comparison of two string views.
 * @param a String view A to compare.
 * @param b String view B to compare.
 * @return True if a == b with case insensitive comparison.
 */
auto string_view_iequal(std::string_view a, std::string_view b) -> bool
{
    const size_t len = a.length();
    if(len != b.length())
    {
        return false;
    }

    for(size_t i = 0; i < len; ++i)
    {
        if(tolower_asciitable_add(a[i]) != tolower_asciitable_add(b[i]))
        {
            return false;
        }
    }

    return true;
}

/**
 * Case insenstive comparison of two string views.  Assumes 'b' is already lowercased.
 * @param a String view A to compare, no assumption made on 'tolower'.
 * @param b String view B to compare, assumes all characters are already 'tolower'.
 * @return True if a == b with tolower(a) == b.
 */
auto internal_string_view_iequal(std::string_view a, std::string_view b) -> bool
{
    const size_t len = a.length();
    if(len != b.length())
    {
        return false;
    }

    for(size_t i = 0; i < len; ++i)
    {
        if(tolower_asciitable_add(a[i]) != b[i])
        {
            return false;
        }
    }

    return true;
}

auto is_http_ws(char c) -> bool
{
    switch(c)
    {
        case HTTP_SP:
        case HTTP_HTAB:
            return true;
        default:
            return false;
    }
}

/**
 * This function will quickly find the next \r\n in the data stream starting at 'index' position
 * for up to 'data_length' bytes.  Note that 'index' is updated to the byte before \r\n.
 * @param data The input data stream to find \r\n in.
 * @param data_length The maximum number of bytes to check into 'data', this is probably data.length() most of the time.
 * @param index [out] This parameter marks the starting position and then the index of (\r\n - 1). The previous byte.
 *                    Note that 'index' is mutated regardless if the \r\n is found or not.
 * @return True if \r\n was found, otherwise false.
 */
[[gnu::always_inline]]
static inline auto find_crlf(
    std::string& data,
    size_t data_length,
    size_t& index
) -> bool
{
    bool found_crlf = false;
    // The parser has found the name of the header, now parse for the value.
#ifdef __SSE4_2__
    static const int sse_cmp_mode = _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_LEAST_SIGNIFICANT;
    static const unsigned char crlf_delim[] = "\r\n";
    ssize_t remaining_bytes = data_length - index;
    while(remaining_bytes > 0)
    {
        const ssize_t len = (remaining_bytes >= 16) ? 16 : remaining_bytes;
        const __m128i simd_a = _mm_loadu_si128((__m128i*)&data[index]);
        const __m128i simd_b = _mm_loadu_si128((__m128i*)&crlf_delim[0]);
        int result = _mm_cmpestri(simd_b, 2, simd_a , len, sse_cmp_mode);
        if(result != 16) // always returns 16 on failure even if len < 16
        {
            index += static_cast<size_t>(result) - 1;
            found_crlf = true;
            break; // while(remaining_bytes > 0)
        }
        index += 16; // doesn't matter, constant is significantly better, but ssize_t is important on remaining_bytes!
        remaining_bytes = data_length - index;
    }
#else
    while(index + 8 < data_length)
    {
        if(data[index] == HTTP_CR) break;
        if(data[++index] == HTTP_CR) break;
        if(data[++index] == HTTP_CR) break;
        if(data[++index] == HTTP_CR) break;
        if(data[++index] == HTTP_CR) break;
        if(data[++index] == HTTP_CR) break;
        if(data[++index] == HTTP_CR) break;
        if(data[++index] == HTTP_CR) break;
        ++index;
    }

    // check one by one or until incomplete
    while(index < data_length)
    {
        if(data[index++] == HTTP_CR && index < data_length && data[index] == HTTP_LF)
        {
            index -= 2;
            found_crlf = true;
            break;
        }
    }
#endif
    return found_crlf;
}

enum class parse_version_result
{
    /// The HTTP Version is malformed.
    malformed,
    /// The HTTP Version is unknown.
    unknown,
    /// The data is incomplete (more needed!).
    incomplete,
    /// The HTTP versioned was parsed, advance to the next stage.
    advance
};

static auto parse_version_common(
    std::string& data,
    std::size_t& m_pos,
    version& m_version

) -> parse_version_result
{
    size_t data_length = data.length();
    if(m_pos + 7 < data_length)
    {
        EXPECT('H', parse_version_result::malformed);
        ADVANCE_EXPECT('T', parse_version_result::malformed);
        ADVANCE_EXPECT('T', parse_version_result::malformed);
        ADVANCE_EXPECT('P', parse_version_result::malformed);
        ADVANCE_EXPECT('/', parse_version_result::malformed);
        ADVANCE_EXPECT('1', parse_version_result::unknown);
        ADVANCE_EXPECT('.', parse_version_result::malformed);
        switch(data[++m_pos])
        {
            case '0':
                m_version = version::v1_0;
                break;
            case '1':
                m_version = version::v1_1;
                break;
            default:
                return parse_version_result::unknown;
        }
    }
    else
    {
        return parse_version_result::incomplete;
    }

    return parse_version_result::advance;
}

static auto parse_request_headers(
    std::string& data,
    size_t& m_pos,
    size_t& m_header_count,
    std::array<std::pair<std::string_view, std::string_view>, 64>& m_headers,
    body_type& m_body_type,
    size_t& m_content_length,
    request::parse_state& m_parse_state
) -> request::parse_result
{
    size_t data_length = data.length();
    // missing empty line or headers
    if(data_length == m_pos)
    {
        return request::parse_result::incomplete;
    }

    // If there are exactly 2 characters left and its the newline,
    // this request is complete as there are no headers.
    if(
            m_pos + 2 <= data_length
        &&  data[m_pos] == HTTP_CR
        &&  data[m_pos + 1] == HTTP_LF
        )
    {
        m_pos += 2; // advance twice for the consumed values.
        return request::parse_result::advance;
    }

    // There must be some headers here, parse them!
    while(true)
    {
        size_t name_start = m_pos;
        size_t value_start;
        size_t name_end = name_start;

#ifdef __SSE4_2__
        static const int sse_cmp_mode = _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_LEAST_SIGNIFICANT;
        static const unsigned char colon_delim[] = ":";

        ssize_t remaining_bytes = data_length - name_end;
        bool found_colon = false;
        while(remaining_bytes > 0)
        {
            const ssize_t len = (remaining_bytes > 16) ? 16 : remaining_bytes;
            const __m128i simd_a = _mm_loadu_si128((__m128i*)&data[name_end]);
            const __m128i simd_b = _mm_loadu_si128((__m128i*)&colon_delim[0]);
            int result = _mm_cmpestri(simd_b, 1, simd_a, len, sse_cmp_mode);
            if(result != 16)
            {
                name_end += result;
                value_start = name_end + 1;
                found_colon = true;
                break; // while(remaining_bytes > 0)
            }
            name_end += 16;
            remaining_bytes = data_length - name_end;
        }

        if(!found_colon)
        {
            return request::parse_result::incomplete;
        }
#else
#define CHECK_FOR_COLON() { if(data[++name_end] == ':') break; }

        // lets check 8 chars in a row!
        while(name_end + 8 < data_length)
        {
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
        }
#undef CHECK_FOR_COLON

        // go one by one...
        while(true)
        {
            if(name_end < data_length)
            {
                if(data[name_end] == ':')
                {
                    value_start = name_end + 1;
                    break; // while(true);
                }
                ++name_end;
            }
            else
            {
                return request::parse_result::incomplete;
            }
        }
#endif

        // Walk value forwards to left trim, this is unlikely to be more than 1 HTTP_SP or HTTP_HTAB
        while(value_start < data_length && is_http_ws(data[value_start]))
        {
            ++value_start;
        }

        // The parser has found the name of the header, now parse for the value.
        size_t value_end = value_start;
        if(!find_crlf(data, data_length, value_end))
        {
            return request::parse_result::incomplete;
        }

        // Update the current position after finding the end of the header,
        // since this loop expects to be on the first char its checking ADVANCE 3 times
        m_pos = value_end + 3;

        // Walk value end backwards to right trim
        while(is_http_ws(data[value_end]))
        {
            --value_end;
        }

        // We are out of space :(
        if(m_header_count == 64)
        {
            return request::parse_result::maximum_headers_exceeded;
        }

        m_headers[m_header_count] =
        {
            {&data[name_start], (name_end - name_start)},
            {&data[value_start], (value_end - value_start + 1)}
        };
        // Before continuing, check to see if any of these headers give an indication if
        // there is any body content.
        if(m_body_type == body_type::no_body)
        {
            auto& [name, value] = m_headers[m_header_count];
            if(
                    internal_string_view_iequal(name, "transfer-encoding")
                &&  internal_string_view_iequal(value, "chunked")
            )
            {
                m_body_type = body_type::chunked;
            }
            else if(
                    internal_string_view_iequal(name, "content-length")
                &&  value.length() > 0
            )
            {
                m_content_length = 0; // in the event from_chars fails, we'll get no body
                std::from_chars(value.data(), value.data() + value.length(), m_content_length, 10);
                m_body_type = body_type::content_length;
            }
        }
        ++m_header_count;

        // If this header line end with CRLF then this request has no more headers.
        if(
                m_pos + 1 < data_length
            &&  data[m_pos] == HTTP_CR
            &&  data[m_pos + 1] == HTTP_LF
        )
        {
            m_pos += 2; // ADVANCE two times for the consumed values and setup for next parse stage
            m_parse_state = request::parse_state::parsed_headers;
            break; // while(true)
        }
    }

    return request::parse_result::advance;
}

auto request::parse(std::string& data) -> request::parse_result
{
    if(data.empty())
    {
        return request::parse_result::incomplete;
    }

    if(m_parse_state == request::parse_state::start)
    {
        auto result = parse_method(data);
        if(result != request::parse_result::advance)
        {
            return result;
        }
    }

    // After parsing the Method move on to parsing the URI.
    if(m_parse_state == request::parse_state::parsed_method)
    {
        auto result = parse_uri(data);
        if(result != request::parse_result::advance)
        {
            return result;
        }
    }

    if(m_parse_state == request::parse_state::parsed_url)
    {
        auto result = parse_version(data);
        if(result != request::parse_result::advance)
        {
            return result;
        }
    }

    if(m_parse_state == request::parse_state::parsed_version)
    {
        auto result = parse_headers(data);
        if(result != request::parse_result::advance)
        {
            return result;
        }
    }

    /**
     * Its possible there is a body after the headers, but this parser only supports
     * it if it can deduce how long the body is by either having a content-length header
     * or chunked encoding.  End of stream is technically a valid http request but
     * without access to the socket the parser has no way of knowing if its the end or,
     * thus we'll just report completion upon finishing parsing of the headers.
     */
    if(    m_parse_state == request::parse_state::parsed_headers
        && m_body_type != body_type::no_body
    )
    {
        auto result = parse_body(data);
        if(result != request::parse_result::advance)
        {
            return result;
        }
    }

    // Currently does not support trailing headers.

    return request::parse_result::complete;
}

auto request::parse_method(std::string& data) -> request::parse_result
{
    size_t data_length = data.length();

    switch(data[0])
    {
        // GET
        case 'G':
        {
            // do we have enough room to parse "G" "ET "?
            if(m_pos + 3 < data_length)
            {
                ADVANCE_EXPECT('E', request::parse_result::method_unknown);
                ADVANCE_EXPECT('T', request::parse_result::method_unknown);
                ADVANCE_EXPECT(HTTP_SP, request::parse_result::method_unknown);
                m_method = method::get;
            }
            else
            {
                return request::parse_result::incomplete;
            }
        }
            break;
        // POST
        // PUT
        // PATCH
        case 'P':
        {
            // we need to guarantee 1 character for which P method this is
            if(m_pos + 1 < data_length)
            {
                // Only advance once for this check, since its multiple branches.
                switch(data[++m_pos])
                {
                    case 'O':
                    {
                        // "PO" "ST "
                        if(m_pos + 3 < data_length)
                        {
                            ADVANCE_EXPECT('S', request::parse_result::method_unknown);
                            ADVANCE_EXPECT('T', request::parse_result::method_unknown);
                            ADVANCE_EXPECT(HTTP_SP, request::parse_result::method_unknown);
                            m_method = method::post;
                        }
                        else
                        {
                            return request::parse_result::incomplete;
                        }
                    }
                        break;
                    case 'U':
                    {
                        // "PU" "T "
                        if(m_pos + 2 < data_length)
                        {
                            ADVANCE_EXPECT('T', request::parse_result::method_unknown);
                            ADVANCE_EXPECT(HTTP_SP, request::parse_result::method_unknown);
                            m_method = method::put;
                        }
                        else
                        {
                            return request::parse_result::incomplete;
                        }
                    }
                        break;
                    case 'A':
                    {
                        // "PA" "TCH "
                        if(m_pos + 4 < data_length)
                        {
                            ADVANCE_EXPECT('T', request::parse_result::method_unknown);
                            ADVANCE_EXPECT('C', request::parse_result::method_unknown);
                            ADVANCE_EXPECT('H', request::parse_result::method_unknown);
                            ADVANCE_EXPECT(HTTP_SP, request::parse_result::method_unknown);
                            m_method = method::patch;
                        }
                        else
                        {
                            return request::parse_result::incomplete;
                        }
                    }
                        break;
                    default:
                        return request::parse_result::incomplete;
                }
            }
            else
            {
                return request::parse_result::incomplete;
            }

        }
            break;
        // HEAD
        case 'H':
        {
            // do we have enough room to parse "H ""EAD "?
            if(m_pos + 4 < data_length)
            {
                ADVANCE_EXPECT('E', request::parse_result::method_unknown);
                ADVANCE_EXPECT('A', request::parse_result::method_unknown);
                ADVANCE_EXPECT('D', request::parse_result::method_unknown);
                ADVANCE_EXPECT(HTTP_SP, request::parse_result::method_unknown);
                m_method = method::head;
            }
            else
            {
                return request::parse_result::incomplete;
            }
        }
            break;
        // DELETE
        case 'D':
        {
            // do we have enough room to parse "D" "ELETE "?
            if(m_pos + 6 < data_length)
            {
                ADVANCE_EXPECT('E', request::parse_result::method_unknown);
                ADVANCE_EXPECT('L', request::parse_result::method_unknown);
                ADVANCE_EXPECT('E', request::parse_result::method_unknown);
                ADVANCE_EXPECT('T', request::parse_result::method_unknown);
                ADVANCE_EXPECT('E', request::parse_result::method_unknown);
                ADVANCE_EXPECT(HTTP_SP, request::parse_result::method_unknown);
                m_method = method::http_delete;
            }
            else
            {
                return request::parse_result::incomplete;
            }
        }
            break;
        // CONNECT
        case 'C':
        {
            // do we have enough room to parse "C" "ONNECT "?
            if(m_pos + 7 < data_length)
            {
                ADVANCE_EXPECT('O', request::parse_result::method_unknown);
                ADVANCE_EXPECT('N', request::parse_result::method_unknown);
                ADVANCE_EXPECT('N', request::parse_result::method_unknown);
                ADVANCE_EXPECT('E', request::parse_result::method_unknown);
                ADVANCE_EXPECT('C', request::parse_result::method_unknown);
                ADVANCE_EXPECT('T', request::parse_result::method_unknown);
                ADVANCE_EXPECT(HTTP_SP, request::parse_result::method_unknown);
                m_method = method::connect;
            }
            else
            {
                return request::parse_result::incomplete;
            }
        }
            break;
        // OPTIONS
        case 'O':
        {
            // do we have enough room to parse "O" "PTIONS "?
            if(m_pos + 7 < data_length)
            {
                ADVANCE_EXPECT('P', request::parse_result::method_unknown);
                ADVANCE_EXPECT('T', request::parse_result::method_unknown);
                ADVANCE_EXPECT('I', request::parse_result::method_unknown);
                ADVANCE_EXPECT('O', request::parse_result::method_unknown);
                ADVANCE_EXPECT('N', request::parse_result::method_unknown);
                ADVANCE_EXPECT('S', request::parse_result::method_unknown);
                ADVANCE_EXPECT(HTTP_SP, request::parse_result::method_unknown);
                m_method = method::options;
            }
            else
            {
                return request::parse_result::incomplete;
            }
        }
            break;
        // TRACE
        case 'T':
        {
            // do we have enough room to parse "T" "RACE "?

            if(m_pos + 5 < data_length)
            {
                ADVANCE_EXPECT('R', request::parse_result::method_unknown);
                ADVANCE_EXPECT('A', request::parse_result::method_unknown);
                ADVANCE_EXPECT('C', request::parse_result::method_unknown);
                ADVANCE_EXPECT('E', request::parse_result::method_unknown);
                ADVANCE_EXPECT(HTTP_SP, request::parse_result::method_unknown);
                m_method = method::trace;
            }
            else
            {
                return request::parse_result::incomplete;
            }
        }
            break;
        default:
            return request::parse_result::method_unknown;
    }

    // If the parser gets this far then its successfully parsed the HTTP Method.
    m_parse_state = request::parse_state::parsed_method;

    return request::parse_result::advance;
}

auto request::parse_uri(std::string& data) -> request::parse_result
{
    ADVANCE(); // Previous parse leaves us on the HTTP_SP token before the URI, advance past it.
    size_t data_length = data.length();
    if(m_uri_start_pos == 0)
    {
        // Set the start pos once (subsequent Parse calls could have different m_pos!)
        // saved for calculating the view of the URI.
        m_uri_start_pos = m_pos;
    }

    // Advance until the next HTTP_SP is found
    while(true)
    {
        if(m_pos < data_length)
        {
            if(data[m_pos] != HTTP_SP)
            {
                // only advance if we don't find the HTTP_SP
                ADVANCE();
                continue; // while(true)
            }
            else
            {
                // We've found the end of the URI.
                break; // while(true)
            }
        }
        else
        {
            // If the end of the data is found with no HTTP_SP then more data is needed.
            return request::parse_result::incomplete;
        }
    }

    // Subtract off 1 for the final HTTP_SP that was found.
    m_uri = std::string_view{&data[m_uri_start_pos], m_pos - m_uri_start_pos};

    // If the parser gets this far then its successfully parsed the URI.
    m_parse_state = request::parse_state::parsed_url;

    return request::parse_result::advance;
}

auto request::parse_version(std::string& data) -> request::parse_result
{
    ADVANCE(); // the standalone function expects to be on the first character.

    auto result = parse_version_common(data, m_pos, m_version);

    switch(result)
    {
        case parse_version_result::malformed:
            return request::parse_result::http_version_malformed;
        case parse_version_result::unknown:
            return request::parse_result::http_version_unknown;
        case parse_version_result::incomplete:
            return request::parse_result::incomplete;
        case parse_version_result::advance:
        {
            if (UNLIKELY(m_pos + 2 >= data.length()))
            {
                return request::parse_result::incomplete;
            }
            else
            {
                // Need to also check for trailing \r\n in requests
                ADVANCE_EXPECT(HTTP_CR, request::parse_result::http_version_malformed);
                ADVANCE_EXPECT(HTTP_LF, request::parse_result::http_version_malformed);
                ADVANCE(); // Next section expects to be on its starting position
            }

            m_parse_state = request::parse_state::parsed_version;
            return request::parse_result::advance;
        }
    }

    // impossible but gcc complains
    return request::parse_result::http_version_unknown;
}

auto request::parse_headers(std::string& data) -> request::parse_result
{
    return parse_request_headers(
        data,
        m_pos,
        m_header_count,
        m_headers,
        m_body_type,
        m_content_length,
        m_parse_state
    );
}

auto request::parse_body(std::string& data) -> request::parse_result
{
    size_t data_length = data.length();
    switch(m_body_type)
    {
        case body_type::chunked:
        {
            // First time through record the start of the body for the full chunked body size.
            if(!m_body.has_value())
            {
                m_body_start = m_pos;
                m_content_length = 0; // leverage this for the decoded length
                m_body.emplace(&data[m_pos], 0);
            }

            while(true)
            {
                size_t chunk_size_end = m_pos + 1;
                bool chunk_size_end_found = false;
                while(chunk_size_end + 1 < data_length)
                {
                    if(data[chunk_size_end] == HTTP_CR && data[chunk_size_end + 1] == HTTP_LF)
                    {
                        chunk_size_end_found = true;
                        break; // while(chunk_size_end + 1 < data_length)
                    }
                    ++chunk_size_end;
                }

                if(!chunk_size_end_found)
                {
                    return request::parse_result::incomplete;
                }

                size_t chunk_length{0};
                std::from_chars(&data[m_pos], &data[chunk_size_end], chunk_length, 16);

                if(chunk_length != 0)
                {
                    // There is a valid chunk with some data, parse over it!
                    // (note: chunk_size_end includes 1 byte for \r\n already
                    m_pos = chunk_size_end + 1 + chunk_length;
                    if(m_pos + 2 < data_length)
                    {
                        // move the data into the correct position. this is major YIKES!
                        char* data_start = data.data() + (m_body_start + m_content_length);
                        char* chunk_start = data.data() + (chunk_size_end + 2);

                        std::memmove(data_start, chunk_start, chunk_length);
                        m_content_length += chunk_length; // Keep track of the entire size though content length.
                        m_body.emplace(&data[m_body_start], m_content_length);

                        ADVANCE_EXPECT(HTTP_CR, request::parse_result::chunk_malformed);
                        ADVANCE_EXPECT(HTTP_LF, request::parse_result::chunk_malformed);
                        ADVANCE(); // This chunk parser expects to be on the first byte of the chunk
                    }
                }
                else
                {
                    // If the chunk length is zero, then ADVANCE_EXPECT the final \r\n and record the body.
                    if(chunk_size_end + 3 <= data_length) // need \n\r\n
                    {
                        m_pos = chunk_size_end + 1; // strip the \n trailing chunk_size_end
                        ADVANCE_EXPECT(HTTP_CR, request::parse_result::chunk_malformed);
                        ADVANCE_EXPECT(HTTP_LF, request::parse_result::chunk_malformed);
                        m_parse_state = request::parse_state::parsed_body;
                        break; // while(true)
                    }
                    else
                    {
                        // its possible to infer the last two bytes *should* be CR LF, but technically
                        // it is incomplete without them
                        return request::parse_result::incomplete;
                    }
                }
            }
        }
            break;
        case body_type::content_length:
        {
            if(m_pos + m_content_length >= data_length)
            {
                m_body.emplace(&data[m_pos], m_content_length);
                m_parse_state = request::parse_state::parsed_body;
            }
            else
            {
                return request::parse_result::incomplete;
            }
        }
            break;
        case body_type::no_body:
            // nothing to do
            break;
    }

    return request::parse_result::advance;
}

auto request::reset() -> void
{
    m_parse_state = request::parse_state::start;
    m_pos = 0;
    //m_method{method::get};
    m_uri_start_pos = 0;
    //m_uri;
    //m_version{version::v1_1};
    m_header_count = 0;
    //m_headers;
    m_body_type = body_type::no_body;
    m_content_length = 0;
    m_body_start = 0;
    m_body = std::nullopt;
}

auto request::http_header(std::string_view name) const -> std::optional<std::string_view>
{
    for(size_t i = 0; i < m_header_count; ++i)
    {
        auto& [header_name, header_value] = m_headers[i];
        if(string_view_iequal(name, header_name))
        {
            return std::optional<std::string_view>{header_value};
        }
    }

    return std::optional<std::string_view>{};
}

auto response::parse(std::string& data) -> response::parse_result
{
    if(data.empty())
    {
        return response::parse_result::incomplete;
    }

    if(m_parse_state == response::parse_state::start)
    {
        auto result = parse_version(data);
        if(result != response::parse_result::advance)
        {
            return result;
        }
    }

    if(m_parse_state == response::parse_state::parsed_version)
    {
        auto result = parse_status_code(data);
        if(result != response::parse_result::advance)
        {
            return result;
        }
    }

    if(m_parse_state == response::parse_state::parsed_status_code)
    {
        auto result = parse_reason_phrase(data);
        if(result != response::parse_result::advance)
        {
            return result;
        }
    }

    if(m_parse_state == response::parse_state::parsed_reason_phrase)
    {
        auto result = parse_headers(data);
        if(result != response::parse_result::advance)
        {
            return result;
        }
    }

    if(    m_parse_state == response::parse_state::parsed_headers
        && m_body_type != body_type::no_body
    )
    {
        auto result = parse_body(data);
        if(result != response::parse_result::advance)
        {
            return result;
        }
    }

    return response::parse_result::complete;
}

auto response::parse_version(std::string& data) -> response::parse_result
{
    auto result = parse_version_common(data, m_pos, m_version);

    switch(result)
    {

        case parse_version_result::malformed:
            return response::parse_result::http_version_malformed;
        case parse_version_result::unknown:
            return response::parse_result::http_version_unknown;
        case parse_version_result::incomplete:
            return response::parse_result::incomplete;
        case parse_version_result::advance:
        {
            if(UNLIKELY(m_pos + 1 >= data.length()))
            {
                return response::parse_result::incomplete;
            }
            else
            {
                ADVANCE_EXPECT(HTTP_SP, response::parse_result::http_version_malformed);
                ADVANCE();
                m_parse_state = response::parse_state::parsed_version;
                return response::parse_result::advance;
            }
        }
    }

    // impossible gcc error
    return response::parse_result::http_version_unknown;
}

auto response::parse_status_code(std::string& data) -> response::parse_result
{
    size_t data_length = data.length();
    size_t required_bytes = m_pos + 3;

    /**
     * All status codes are 3 digits in length, plus the trailing HTTP_SP, "XXX "
     */
    if(UNLIKELY(required_bytes > data_length))
    {
        return response::parse_result::incomplete;
    }

    if(    !std::isdigit(data[m_pos])
        || !std::isdigit(data[m_pos + 1])
        || !std::isdigit(data[m_pos + 2])
    )
    {
        return response::parse_result::http_status_code_malformed;
    }

    std::from_chars(&data[m_pos], &data[required_bytes], m_status_code, 10);
    if(LIKELY(m_status_code != 0))
    {
        m_pos += 3; // Advanced 3x past the status code.
        EXPECT(HTTP_SP, response::parse_result::http_status_code_malformed);
        ADVANCE();
        m_parse_state = response::parse_state::parsed_status_code;
        return response::parse_result::advance;
    }
    else
    {
        return response::parse_result::http_status_code_malformed;
    }
}

auto response::parse_reason_phrase(std::string& data) -> response::parse_result
{
    // Since the reason phrases are not truely standardized, the parser just looks
    // for the \r\n that ends the line and sets the m_reason_phrase to the entire section.
    // Its possible there are only certain characters allowed in the reason phrase, this is
    // currently not handled by the parser and just looks for \r\n.

    size_t value_end = m_pos;
    if(LIKELY(find_crlf(data, data.length(), value_end)))
    {
        // If found, value_end will be the byte before \r\n, so calculate the length + 1 for the string view.
        m_reason_phrase = std::string_view{&data[m_pos], value_end - m_pos + 1};
        m_pos = value_end + 3; // advance past the \r\n as well
        m_parse_state = response::parse_state::parsed_reason_phrase;
        return response::parse_result::advance;
    }
    else
    {
        return response::parse_result::incomplete;
    }
}

static auto parse_response_headers(
    std::string& data,
    size_t& m_pos,
    size_t& m_header_count,
    std::array<std::pair<std::string_view, std::string_view>, 64>& m_headers,
    body_type& m_body_type,
    size_t& m_content_length,
    response::parse_state& m_parse_state
) -> response::parse_result
{
    size_t data_length = data.length();
    // missing empty line or headers
    if(data_length == m_pos)
    {
        return response::parse_result::incomplete;
    }

    // If there are exactly 2 characters left and its the newline,
    // this request is complete as there are no headers.
    if(
            m_pos + 2 <= data_length
        &&  data[m_pos] == HTTP_CR
        &&  data[m_pos + 1] == HTTP_LF
        )
    {
        m_pos += 2; // advance twice for the consumed values.
        return response::parse_result::advance;
    }

    // There must be some headers here, parse them!
    while(true)
    {
        size_t name_start = m_pos;
        size_t value_start;
        size_t name_end = name_start;

#ifdef __SSE4_2__
        static const int sse_cmp_mode = _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_LEAST_SIGNIFICANT;
        static const unsigned char colon_delim[] = ":";

        ssize_t remaining_bytes = data_length - name_end;
        bool found_colon = false;
        while(remaining_bytes > 0)
        {
            const ssize_t len = (remaining_bytes > 16) ? 16 : remaining_bytes;
            const __m128i simd_a = _mm_loadu_si128((__m128i*)&data[name_end]);
            const __m128i simd_b = _mm_loadu_si128((__m128i*)&colon_delim[0]);
            int result = _mm_cmpestri(simd_b, 1, simd_a, len, sse_cmp_mode);
            if(result != 16)
            {
                name_end += result;
                value_start = name_end + 1;
                found_colon = true;
                break; // while(remaining_bytes > 0)
            }
            name_end += 16;
            remaining_bytes = data_length - name_end;
        }

        if(!found_colon)
        {
            return response::parse_result::incomplete;
        }
#else
        #define CHECK_FOR_COLON() { if(data[++name_end] == ':') break; }

        // lets check 8 chars in a row!
        while(name_end + 8 < data_length)
        {
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
            CHECK_FOR_COLON();
        }
#undef CHECK_FOR_COLON

        // go one by one...
        while(true)
        {
            if(name_end < data_length)
            {
                if(data[name_end] == ':')
                {
                    value_start = name_end + 1;
                    break; // while(true);
                }
                ++name_end;
            }
            else
            {
                return response::parse_result::incomplete;
            }
        }
#endif

        // Walk value forwards to left trim, this is unlikely to be more than 1 HTTP_SP or HTTP_HTAB
        while(value_start < data_length && is_http_ws(data[value_start]))
        {
            ++value_start;
        }

        // The parser has found the name of the header, now parse for the value.
        size_t value_end = value_start;
        if(!find_crlf(data, data_length, value_end))
        {
            return response::parse_result::incomplete;
        }

        // Update the current position after finding the end of the header,
        // since this loop expects to be on the first char its checking ADVANCE 3 times
        m_pos = value_end + 3;

        // Walk value end backwards to right trim
        while(is_http_ws(data[value_end]))
        {
            --value_end;
        }

        // We are out of space :(
        if(m_header_count == 64)
        {
            return response::parse_result::maximum_headers_exceeded;
        }

        m_headers[m_header_count] =
        {
            {&data[name_start], (name_end - name_start)},
            {&data[value_start], (value_end - value_start + 1)}
        };
        // Before continuing, check to see if any of these headers give an indication if
        // there is any body content.
        if(m_body_type == body_type::no_body)
        {
            auto& [name, value] = m_headers[m_header_count];
            if(
                    internal_string_view_iequal(name, "transfer-encoding")
                &&  internal_string_view_iequal(value, "chunked")
                )
            {
                m_body_type = body_type::chunked;
            }
            else if(
                    internal_string_view_iequal(name, "content-length")
                &&  value.length() > 0
                )
            {
                m_content_length = 0; // in the event from_chars fails, we'll get no body
                std::from_chars(value.data(), value.data() + value.length(), m_content_length, 10);
                m_body_type = body_type::content_length;
            }
        }
        ++m_header_count;

        // If this header line end with CRLF then this request has no more headers.
        if(
                m_pos + 1 < data_length
            &&  data[m_pos] == HTTP_CR
            &&  data[m_pos + 1] == HTTP_LF
            )
        {
            m_pos += 2; // ADVANCE two times for the consumed values and setup for next parse stage
            m_parse_state = response::parse_state::parsed_headers;
            break; // while(true)
        }
    }

    return response::parse_result::advance;
}

auto response::parse_headers(std::string& data) -> response::parse_result
{
    return parse_response_headers(
        data,
        m_pos,
        m_header_count,
        m_headers,
        m_body_type,
        m_content_length,
        m_parse_state
    );
}

auto response::parse_body(std::string& data) -> response::parse_result
{
    size_t data_length = data.length();
    switch(m_body_type)
    {
        case body_type::chunked:
        {
            // First time through record the start of the body for the full chunked body size.
            if(!m_body.has_value())
            {
                m_body_start = m_pos;
                m_content_length = 0; // leverage this for the decoded length
                m_body.emplace(&data[m_pos], 0);
            }

            while(true)
            {
                size_t chunk_size_end = m_pos + 1;
                bool chunk_size_end_found = false;
                while(chunk_size_end + 1 < data_length)
                {
                    if(data[chunk_size_end] == HTTP_CR && data[chunk_size_end + 1] == HTTP_LF)
                    {
                        chunk_size_end_found = true;
                        break; // while(chunk_size_end + 1 < data_length)
                    }
                    ++chunk_size_end;
                }

                if(!chunk_size_end_found)
                {
                    return response::parse_result::incomplete;
                }

                size_t chunk_length{0};
                std::from_chars(&data[m_pos], &data[chunk_size_end], chunk_length, 16);

                if(chunk_length != 0)
                {
                    // There is a valid chunk with some data, parse over it!
                    // (note: chunk_size_end includes 1 byte for \r\n already
                    m_pos = chunk_size_end + 1 + chunk_length;
                    if(m_pos + 2 < data_length)
                    {
                        // move the data into the correct position. this is major YIKES!
                        char* data_start = data.data() + (m_body_start + m_content_length);
                        char* chunk_start = data.data() + (chunk_size_end + 2);

                        std::memmove(data_start, chunk_start, chunk_length);
                        m_content_length += chunk_length; // Keep track of the entire size though content length.
                        m_body.emplace(&data[m_body_start], m_content_length);

                        ADVANCE_EXPECT(HTTP_CR, response::parse_result::chunk_malformed);
                        ADVANCE_EXPECT(HTTP_LF, response::parse_result::chunk_malformed);
                        ADVANCE(); // This chunk parser expects to be on the first byte of the chunk
                    }
                }
                else
                {
                    // If the chunk length is zero, then ADVANCE_EXPECT the final \r\n and record the body.
                    if(chunk_size_end + 3 <= data_length) // need \n\r\n
                    {
                        m_pos = chunk_size_end + 1; // strip the \n trailing chunk_size_end
                        ADVANCE_EXPECT(HTTP_CR, response::parse_result::chunk_malformed);
                        ADVANCE_EXPECT(HTTP_LF, response::parse_result::chunk_malformed);
                        m_parse_state = response::parse_state::parsed_body;
                        break; // while(true)
                    }
                    else
                    {
                        // its possible to infer the last two bytes *should* be CR LF, but technically
                        // it is incomplete without them
                        return response::parse_result::incomplete;
                    }
                }
            }
        }
            break;
        case body_type::content_length:
        {
            if(m_pos + m_content_length >= data_length)
            {
                m_body.emplace(&data[m_pos], m_content_length);
                m_parse_state = response::parse_state::parsed_body;
            }
            else
            {
                return response::parse_result::incomplete;
            }
        }
            break;
        case body_type::no_body:
            // nothing to do
            break;
    }

    return response::parse_result::complete;
}

auto response::reset() -> void
{
    m_parse_state = response::parse_state::start;
    m_pos = 0;

    //m_version{version::v1_1};
    m_status_code = 0;
    //m_reason_phrase;

    m_header_count = 0;
    //m_headers;

    m_body_type = body_type::no_body;
    m_content_length = 0;
    m_body_start = 0;
    m_body = std::nullopt;
}

auto response::http_header(std::string_view name) const -> std::optional<std::string_view>
{
    for(size_t i = 0; i < m_header_count; ++i)
    {
        auto& [header_name, header_value] = m_headers[i];
        if(string_view_iequal(name, header_name))
        {
            return std::optional<std::string_view>{header_value};
        }
    }

    return std::optional<std::string_view>{};
}

} // namespace turbo::http

