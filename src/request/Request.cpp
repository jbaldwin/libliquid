#include "liquid/request/Request.h"

namespace liquid::request
{

#define EXPECT(C, E) { if(data[++m_pos] != C) { return E; }}
#define EXPECT_WS(E) { if(!std::isspace(data[++m_pos])) { return E; }}
#define ADVANCE() { ++m_pos; }

auto Request::Parse(std::string_view data) -> ParseResult
{
    if(data.empty())
    {
        return ParseResult::INCOMPLETE;
    }

    if(m_parse_state == ParseState::START)
    {
        // left trim to start
        for(auto c : data)
        {
            if(std::isspace(c))
            {
                ++m_pos;
            }
            else
            {
                break;
            }
        }

        switch(data[m_pos])
        {
            // GET
            case 'G':
            {
                // do we have enough room to parse "G" "ET "?
                if(m_pos + 3 < data.length())
                {
                    EXPECT('E', ParseResult::UNKNOWN_METHOD);
                    EXPECT('T', ParseResult::UNKNOWN_METHOD);
                    EXPECT_WS(ParseResult::UNKNOWN_METHOD);
                    ADVANCE() // advance past whitespace character
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
                    ++m_pos;
                    if(data[m_pos] == 'O')
                    {
                        // "PO" "ST "
                        if(m_pos + 3 < data.length())
                        {
                            EXPECT('S', ParseResult::UNKNOWN_METHOD);
                            EXPECT('T', ParseResult::UNKNOWN_METHOD);
                            EXPECT_WS(ParseResult::UNKNOWN_METHOD);
                            ADVANCE(); // advance past whitespace character
                            m_method = Method::POST;
                        }
                        else
                        {
                            return ParseResult::INCOMPLETE;
                        }
                    }
                    else if(data[m_pos] == 'U')
                    {
                        // "PU" "T "
                        if(m_pos + 2 < data.length())
                        {
                            EXPECT('T', ParseResult::UNKNOWN_METHOD);
                            EXPECT_WS(ParseResult::UNKNOWN_METHOD); // advance past whitespace character
                            m_method = Method::PUT;
                        }
                        else
                        {
                            return ParseResult::INCOMPLETE;
                        }
                    }
                    else if(data[m_pos] == 'A')
                    {
                        // "PA" "TCH "
                        if(m_pos + 4 < data.length())
                        {
                            EXPECT('T', ParseResult::UNKNOWN_METHOD);
                            EXPECT('C', ParseResult::UNKNOWN_METHOD);
                            EXPECT('H', ParseResult::UNKNOWN_METHOD);
                            EXPECT_WS(ParseResult::UNKNOWN_METHOD); // advance past whitespace character
                            m_method = Method::PATCH;
                        }
                        else
                        {
                            return ParseResult::INCOMPLETE;
                        }
                    }
                    else
                    {
                        return ParseResult::UNKNOWN_METHOD;
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
                    EXPECT('E', ParseResult::UNKNOWN_METHOD);
                    EXPECT('A', ParseResult::UNKNOWN_METHOD);
                    EXPECT('D', ParseResult::UNKNOWN_METHOD);
                    EXPECT_WS(ParseResult::UNKNOWN_METHOD); // advance past whitespace character
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
                    EXPECT('E', ParseResult::UNKNOWN_METHOD);
                    EXPECT('L', ParseResult::UNKNOWN_METHOD);
                    EXPECT('E', ParseResult::UNKNOWN_METHOD);
                    EXPECT('T', ParseResult::UNKNOWN_METHOD);
                    EXPECT('E', ParseResult::UNKNOWN_METHOD);
                    EXPECT_WS(ParseResult::UNKNOWN_METHOD); // advance past whitespace character
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
                    EXPECT('O', ParseResult::UNKNOWN_METHOD);
                    EXPECT('N', ParseResult::UNKNOWN_METHOD);
                    EXPECT('N', ParseResult::UNKNOWN_METHOD);
                    EXPECT('E', ParseResult::UNKNOWN_METHOD);
                    EXPECT('C', ParseResult::UNKNOWN_METHOD);
                    EXPECT('T', ParseResult::UNKNOWN_METHOD);
                    EXPECT_WS(ParseResult::UNKNOWN_METHOD)
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
                    EXPECT('P', ParseResult::UNKNOWN_METHOD);
                    EXPECT('T', ParseResult::UNKNOWN_METHOD);
                    EXPECT('I', ParseResult::UNKNOWN_METHOD);
                    EXPECT('O', ParseResult::UNKNOWN_METHOD);
                    EXPECT('N', ParseResult::UNKNOWN_METHOD);
                    EXPECT('S', ParseResult::UNKNOWN_METHOD);
                    EXPECT_WS(ParseResult::UNKNOWN_METHOD);
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
                    EXPECT('R', ParseResult::UNKNOWN_METHOD);
                    EXPECT('A', ParseResult::UNKNOWN_METHOD);
                    EXPECT('C', ParseResult::UNKNOWN_METHOD);
                    EXPECT('E', ParseResult::UNKNOWN_METHOD);
                    EXPECT_WS(ParseResult::UNKNOWN_METHOD);
                    m_method = Method::TRACE;
                }
                else
                {
                    return ParseResult::INCOMPLETE;
                }
            }
                break;
            default:
                return ParseResult::UNKNOWN_METHOD;
        }
    }

    m_parse_state = ParseState::METHOD_PARSED;

    return ParseResult::INCOMPLETE;
}

auto Request::GetParseState() const -> ParseState
{
    return m_parse_state;
}

auto Request::GetMethod() const -> Method
{
    return m_method;
}

} // namespace liquid::request
