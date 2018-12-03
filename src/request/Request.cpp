#include "liquid/request/Request.h"
#include "liquid/StringUtil.h"

#include <charconv>

namespace liquid::request
{

static constexpr char HTTP_SP = ' ';
static constexpr char HTTP_CR = '\r';
static constexpr char HTTP_LF = '\n';

#define EXPECT(C, E) { if(data[++m_pos] != C) { return E; }}
#define ADVANCE() { ++m_pos; }

auto Request::Parse(std::string_view data) -> ParseResult
{
    if(data.empty())
    {
        return ParseResult::INCOMPLETE;
    }

    if(m_parse_state == ParseState::START)
    {
        switch(data[0])
        {
            // GET
            case 'G':
            {
                // do we have enough room to parse "G" "ET "?
                if(m_pos + 3 < data.length())
                {
                    EXPECT('E', ParseResult::METHOD_UNKNOWN);
                    EXPECT('T', ParseResult::METHOD_UNKNOWN);
                    EXPECT(HTTP_SP, ParseResult::METHOD_UNKNOWN);
                    m_method = Method::GET;
                }
                else
                {
                    return ParseResult::INCOMPLETE;
                }
            }
                break;
            // POST
            // PUT
            // PATCH
            case 'P':
            {
                // we need to guarantee 1 character for which P method this is
                if(m_pos + 1 < data.length())
                {
                    // Only advance once for this check, since its multiple branches.
                    switch(data[++m_pos])
                    {
                        case 'O':
                        {
                            // "PO" "ST "
                            if(m_pos + 3 < data.length())
                            {
                                EXPECT('S', ParseResult::METHOD_UNKNOWN);
                                EXPECT('T', ParseResult::METHOD_UNKNOWN);
                                EXPECT(HTTP_SP, ParseResult::METHOD_UNKNOWN);
                                m_method = Method::POST;
                            }
                            else
                            {
                                return ParseResult::INCOMPLETE;
                            }
                        }
                            break;
                        case 'U':
                        {
                            // "PU" "T "
                            if(m_pos + 2 < data.length())
                            {
                                EXPECT('T', ParseResult::METHOD_UNKNOWN);
                                EXPECT(HTTP_SP, ParseResult::METHOD_UNKNOWN);
                                m_method = Method::PUT;
                            }
                            else
                            {
                                return ParseResult::INCOMPLETE;
                            }
                        }
                            break;
                        case 'A':
                        {
                            // "PA" "TCH "
                            if(m_pos + 4 < data.length())
                            {
                                EXPECT('T', ParseResult::METHOD_UNKNOWN);
                                EXPECT('C', ParseResult::METHOD_UNKNOWN);
                                EXPECT('H', ParseResult::METHOD_UNKNOWN);
                                EXPECT(HTTP_SP, ParseResult::METHOD_UNKNOWN);
                                m_method = Method::PATCH;
                            }
                            else
                            {
                                return ParseResult::INCOMPLETE;
                            }
                        }
                            break;
                        default:
                            return ParseResult::INCOMPLETE;
                    }
                }
                else
                {
                    return ParseResult::INCOMPLETE;
                }

            }
                break;
            // HEAD
            case 'H':
            {
                // do we have enough room to parse "H ""EAD "?
                if(m_pos + 4 < data.length())
                {
                    EXPECT('E', ParseResult::METHOD_UNKNOWN);
                    EXPECT('A', ParseResult::METHOD_UNKNOWN);
                    EXPECT('D', ParseResult::METHOD_UNKNOWN);
                    EXPECT(HTTP_SP, ParseResult::METHOD_UNKNOWN);
                    m_method = Method::HEAD;
                }
                else
                {
                    return ParseResult::INCOMPLETE;
                }
            }
                break;
            // DELETE
            case 'D':
            {
                // do we have enough room to parse "D" "ELETE "?
                if(m_pos + 6 < data.length())
                {
                    EXPECT('E', ParseResult::METHOD_UNKNOWN);
                    EXPECT('L', ParseResult::METHOD_UNKNOWN);
                    EXPECT('E', ParseResult::METHOD_UNKNOWN);
                    EXPECT('T', ParseResult::METHOD_UNKNOWN);
                    EXPECT('E', ParseResult::METHOD_UNKNOWN);
                    EXPECT(HTTP_SP, ParseResult::METHOD_UNKNOWN);
                    m_method = Method::DELETE;
                }
                else
                {
                    return ParseResult::INCOMPLETE;
                }
            }
                break;
            // CONNECT
            case 'C':
            {
                // do we have enough room to parse "C" "ONNECT "?
                if(m_pos + 7 < data.length())
                {
                    EXPECT('O', ParseResult::METHOD_UNKNOWN);
                    EXPECT('N', ParseResult::METHOD_UNKNOWN);
                    EXPECT('N', ParseResult::METHOD_UNKNOWN);
                    EXPECT('E', ParseResult::METHOD_UNKNOWN);
                    EXPECT('C', ParseResult::METHOD_UNKNOWN);
                    EXPECT('T', ParseResult::METHOD_UNKNOWN);
                    EXPECT(HTTP_SP, ParseResult::METHOD_UNKNOWN);
                    m_method = Method::CONNECT;
                }
                else
                {
                    return ParseResult::INCOMPLETE;
                }
            }
                break;
            // OPTIONS
            case 'O':
            {
                // do we have enough room to parse "O" "PTIONS "?
                if(m_pos + 7 < data.length())
                {
                    EXPECT('P', ParseResult::METHOD_UNKNOWN);
                    EXPECT('T', ParseResult::METHOD_UNKNOWN);
                    EXPECT('I', ParseResult::METHOD_UNKNOWN);
                    EXPECT('O', ParseResult::METHOD_UNKNOWN);
                    EXPECT('N', ParseResult::METHOD_UNKNOWN);
                    EXPECT('S', ParseResult::METHOD_UNKNOWN);
                    EXPECT(HTTP_SP, ParseResult::METHOD_UNKNOWN);
                    m_method = Method::OPTIONS;
                }
                else
                {
                    return ParseResult::INCOMPLETE;
                }
            }
                break;
            // TRACE
            case 'T':
            {
                // do we have enough room to parse "T" "RACE "?

                if(m_pos + 5 < data.length())
                {
                    EXPECT('R', ParseResult::METHOD_UNKNOWN);
                    EXPECT('A', ParseResult::METHOD_UNKNOWN);
                    EXPECT('C', ParseResult::METHOD_UNKNOWN);
                    EXPECT('E', ParseResult::METHOD_UNKNOWN);
                    EXPECT(HTTP_SP, ParseResult::METHOD_UNKNOWN);
                    m_method = Method::TRACE;
                }
                else
                {
                    return ParseResult::INCOMPLETE;
                }
            }
                break;
            default:
                return ParseResult::METHOD_UNKNOWN;
        }

        // If the parser gets this far then its successfully parsed the HTTP Method.
        m_parse_state = ParseState::PARSED_METHOD;
    }

    // After parsing the Method move on to parsing the URI.
    if(m_parse_state == ParseState::PARSED_METHOD)
    {
        ADVANCE(); // Previous parse leaves us on the HTTP_SP token before the URI, advance past it.
        size_t total_len = data.length();
        if(m_uri_start_pos == 0)
        {
            // Set the start pos once (subsequent Parse calls could have different m_pos!)
            // saved for calculating the view of the URI.
            m_uri_start_pos = m_pos;
        }

        // Advance until the next HTTP_SP is found
        while(true)
        {
            if(m_pos < total_len)
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
                return ParseResult::INCOMPLETE;
            }
        }

        // Subtract off 1 for the final HTTP_SP that was found.
        m_uri = std::string_view{&data[m_uri_start_pos], m_pos - m_uri_start_pos};

        // If the parser gets this far then its successfully parsed the URI.
        m_parse_state = ParseState::PARSED_URI;
    }

    if(m_parse_state == ParseState::PARSED_URI)
    {
        if(m_pos + 10 < data.length())
        {
            // This state expects to still be on the previous HTTP_SP so we can easily EXPECT()
            EXPECT('H', ParseResult::HTTP_VERSION_MALFORMED);
            EXPECT('T', ParseResult::HTTP_VERSION_MALFORMED);
            EXPECT('T', ParseResult::HTTP_VERSION_MALFORMED);
            EXPECT('P', ParseResult::HTTP_VERSION_MALFORMED);
            EXPECT('/', ParseResult::HTTP_VERSION_MALFORMED);
            EXPECT('1', ParseResult::HTTP_VERSION_UNKNOWN);
            EXPECT('.', ParseResult::HTTP_VERSION_MALFORMED);
            switch(data[++m_pos])
            {
                case '0':
                    m_version = Version::V1_0;
                    break;
                case '1':
                    m_version = Version::V1_1;
                    break;
                default:
                    return ParseResult::HTTP_VERSION_UNKNOWN;
            }
            // We include the \r\n here Request-line to Header parse to reduce bounds checking
            EXPECT(HTTP_CR, ParseResult::HTTP_VERSION_MALFORMED);
            EXPECT(HTTP_LF, ParseResult::HTTP_VERSION_MALFORMED);
            ADVANCE(); // Next section expects to be on its starting position
        }
        else
        {
            return ParseResult::INCOMPLETE;
        }

        m_parse_state = ParseState::PARSED_VERSION;
    }

    if(m_parse_state == ParseState::PARSED_VERSION)
    {
        size_t total_length = data.length();

        // missing empty line or headers
        if(total_length == m_pos)
        {
            return ParseResult::INCOMPLETE;
        }

        // If there are exactly 2 characters left and its the newline,
        // this request is complete as there are no headers.
        if(
                m_pos + 2 == data.length()
            &&  data[m_pos] == HTTP_CR
            &&  data[m_pos + 1] == HTTP_LF
        )
        {
            m_pos += 2; // advance twice for the consumed values.
            return ParseResult::COMPLETE;
        }

        // There must be some headers here, parse them!
        while(true)
        {
            size_t name_start = m_pos;
            size_t value_start;
            // walk name_start forwards to left trim
            while(name_start < total_length && std::isspace(data[name_start]))
            {
                ++name_start;
            }

            size_t name_end = name_start + 1; // first character of the name cannot be ':'
            bool found_colon = false;
            while(name_end < total_length)
            {
                if(data[name_end] == ':')
                {
                    value_start = name_end + 1; // grab value_start before walking name_end backwards for WS
                    --name_end; // don't include ':' in the name
                    found_colon = true;
                    break; // while(name_end < total_length)
                }
                ++name_end;
            }

            // Never found the ':' token, we need more data.
            if(!found_colon)
            {
                return ParseResult::INCOMPLETE;
            }

            // Walk name_end backwards to right trim.
            while(std::isspace(data[name_end]))
            {
                --name_end;
            }

            // Walk value forwards to left trim
            while(value_start < total_length && std::isspace(data[value_start]))
            {
                ++value_start;
            }

            // The parser has found the name of the header, now parse for the value.
            size_t value_end = value_start;
            bool found_crlf = false;
            // This loop must check two characters at a time (CRLF!)
            while(value_end + 1 < total_length)
            {
                if(data[value_end] == HTTP_CR && data[value_end + 1] == HTTP_LF)
                {
                    --value_end; // Found the end, do not include CRLF in the value.
                    found_crlf = true;
                    break; // while(value_end + 1 < total_length)
                }
                ++value_end; // we must only increment by 1 otherwise we could skip over a CR
            }

            if(!found_crlf)
            {
                return ParseResult::INCOMPLETE;
            }

            // Update the current position after finding the end of the header,
            // since this loop expects to be on the first char its checking ADVANCE 3 times
            m_pos = value_end + 3;

            // Walk value end backwards to right trim
            while(std::isspace(data[value_end]))
            {
                --value_end;
            }

            m_headers[m_header_count] =
            {
                {&data[name_start], (name_end - name_start + 1)},
                {&data[value_start], (value_end - value_start + 1)}
            };
            // Before continuing, check to see if any of these headers give an indication if
            // there is any body content.
            if(m_body_type == BodyType::END_OF_STREAM)
            {
                auto& [name, value] = m_headers[m_header_count];
                if(
                        string_view_icompare(name, "Transfer-Encoding")
                    &&  string_view_icompare(value, "chunked")
                )
                {
                    m_body_type = BodyType::CHUNKED;
                }
                else if(
                        string_view_icompare(name, "Content-Length")
                    &&  value.length() > 0
                )
                {
                    std::from_chars(value.data(), value.data() + value.length(), m_content_length, 10);
                    m_body_type = BodyType::CONTENT_LENGTH;
                }
            }
            ++m_header_count;

            // If this header line end with CRLF then this request has no more headers.
            if(
                m_pos + 1 < data.length()
                &&  data[m_pos] == HTTP_CR
                &&  data[m_pos + 1] == HTTP_LF
                )
            {
                m_pos += 2; // ADVANCE two times for the consumed values and setup for next parse stage
                m_parse_state = ParseState::PARSED_HEADERS;
                break; // while(true)
            }
        }
    }

    // There may or may not be a body
    if(m_parse_state == ParseState::PARSED_HEADERS)
    {
        switch(m_body_type)
        {
            case BodyType::CHUNKED:
            {

            }
                break;
            case BodyType::CONTENT_LENGTH:
            {
                if(m_pos + m_content_length >= data.length())
                {
                    m_body.emplace(&data[m_pos], m_content_length);
                    m_parse_state = ParseState::PARSED_BODY;
                }
                else
                {
                    return ParseResult::INCOMPLETE;
                }
            }
                break;
            case BodyType::END_OF_STREAM:
            {
                if (m_pos < data.length())
                {
                    // just update to as much data is possible, this isn't a super reliable method
                    m_body.emplace(&data[m_pos], data.length() - m_pos);
                    // Do not update m_parse_state, the user might
                    // call again with more data to add into the body...
                }
            }
                break;
        }
    }

    return ParseResult::COMPLETE;
}

auto Request::GetParseState() const -> ParseState
{
    return m_parse_state;
}

auto Request::GetMethod() const -> Method
{
    return m_method;
}

auto Request::GetUri() const -> std::string_view
{
    return m_uri;
}

auto Request::GetVersion() const -> Version
{
    return m_version;
}

auto Request::GetHeaderCount() const -> size_t
{
    return m_header_count;
}

auto Request::GetHeader(std::string_view name) const -> std::optional<std::string_view>
{
    for(size_t i = 0; i < m_header_count; ++i)
    {
        auto& [req_name, req_value] = m_headers[i];
        if(liquid::string_view_icompare(name, req_name))
        {
            return std::optional<std::string_view>{req_value};
        }
    }

    return std::optional<std::string_view>{};
}

auto Request::GetBody() const -> const std::optional<std::string_view>&
{
    return m_body;
}

} // namespace liquid::request
