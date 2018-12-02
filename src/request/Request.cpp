#include "liquid/request/Request.h"

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
        }
        else
        {
            return ParseResult::INCOMPLETE;
        }

        m_parse_state = ParseState::PARSED_VERSION;
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

} // namespace liquid::request
