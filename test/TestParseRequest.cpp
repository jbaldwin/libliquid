#include "Catch2.h"
#include "liquid/request/Request.h"

SCENARIO("Parsing an empty string.")
{
    GIVEN("An empty HTTP Request data.")
    {
        std::string request_data{};
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::START);
            }
        }
    }
}

SCENARIO("Parsing GET Method")
{
    GIVEN("A GET HTTP Request")
    {
        std::string request_data = "GET ";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_METHOD);
            }
        }
    }

    GIVEN("An incomplete GET HTTP Request")
    {
        std::string request_data = "G";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::START);
            }
        }

        request_data = "GE";
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::START);
            }
        }

        request_data = "GET";
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::START);
            }
        }

        request_data = "GET ";
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the parser to return INCOMPLETE with method GET")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_METHOD);
            }
        }
    }
}

SCENARIO("Parsing POST Method")
{
    GIVEN("A POST HTTP Request")
    {
        std::string request_data = "POST ";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be POST")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::POST);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_METHOD);
            }
        }
    }
    GIVEN("An incomplete POST HTTP Request")
    {
        std::string request_data = "P";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::START);
            }
        }

        request_data = "PO";
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::START);
            }
        }

        request_data = "POS";
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::START);
            }
        }

        request_data = "POST";
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::START);
            }
        }

        request_data = "POST ";
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::POST);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_METHOD);
            }
        }
    }
}

SCENARIO("Parsing PUT Method")
{
    GIVEN("A PUT HTTP Request")
    {
        std::string request_data = "PUT ";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be PUT")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::PUT);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_METHOD);
            }
        }
    }
}

SCENARIO("Parsing PATCH Method")
{
    GIVEN("A PATCH HTTP Request")
    {
        std::string request_data = "PATCH ";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be PATCH")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::PATCH);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_METHOD);
            }
        }
    }
}

SCENARIO("Parsing HEAD Method")
{
    GIVEN("A HEAD HTTP Request")
    {
        std::string request_data = "HEAD ";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be HEAD")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::HEAD);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_METHOD);
            }
        }
    }
}

SCENARIO("Parsing DELETE Method")
{
    GIVEN("A DELETE HTTP Request")
    {
        std::string request_data = "DELETE ";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be DELETE")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::DELETE);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_METHOD);
            }
        }
    }
}

SCENARIO("Parsing a complete URI")
{
    GIVEN("A complete URI")
    {
        std::string request_data = "GET /derp.html ";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_URI);
                REQUIRE(request.GetUri() == "/derp.html");
            }
        }
    }
}

SCENARIO("Parsing an incomplete URI")
{
    GIVEN("A complete URI")
    {
        std::string request_data = "GET /derp.html?flerpity=";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and not have parsed the URI yet")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_METHOD);
            }
        }

        request_data.append("merpity");
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and not have parsed the URI yet")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_METHOD);
            }
        }

        request_data.append(" ");
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and parsed the URI")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_URI);
                REQUIRE(request.GetUri() == "/derp.html?flerpity=merpity");
            }
        }
    }
}

SCENARIO("Parsing a complete HTTP Version")
{
    GIVEN("A complete Request-Line")
    {
        std::string request_data = "GET /derp.html HTTP/1.0\r\n";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_VERSION);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_0);
            }
        }
    }
}

SCENARIO("Parsing an incomplete HTTP Version")
{
    GIVEN("An complete Request-Line")
    {
        std::string request_data = "GET /derp.html HTTP/";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_URI);
                REQUIRE(request.GetUri() == "/derp.html");
            }
        }

        request_data.append("1");
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_URI);
                REQUIRE(request.GetUri() == "/derp.html");
            }
        }

        request_data.append(".");
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_URI);
                REQUIRE(request.GetUri() == "/derp.html");
            }
        }

        request_data.append("1");
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_URI);
                REQUIRE(request.GetUri() == "/derp.html");
            }
        }

        request_data.append("\r");
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_URI);
                REQUIRE(request.GetUri() == "/derp.html");
            }
        }

        request_data.append("\n");
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI and have a version")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_VERSION);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
            }
        }

        request_data.append("\r\n");
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI and have a version and a complete request.")
            {
                REQUIRE(result == liquid::request::ParseResult::COMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_VERSION);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
            }
        }
    }
}

SCENARIO("Parsing a header line.")
{
    GIVEN("A complete Request-Line")
    {
        std::string request_data = "GET /derp.html HTTP/1.1\r\nConnection: keep-alive\r\n\r\n";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI")
            {
                REQUIRE(result == liquid::request::ParseResult::COMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_HEADERS);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 1);
                REQUIRE(request.GetHeader("cONNECTION").value() == "keep-alive");
            }
        }
    }
}

SCENARIO("Parsing multiple header lines.")
{
    GIVEN("A complete Request-Line")
    {
        std::string request_data =
            "GET /derp.html HTTP/1.1\r\n"
            "Connection: keep-alive\r\n"
            "Accept: */*\r\n"
            "X-Content-Length: 0\r\n"
            "\r\n";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI")
            {
                REQUIRE(result == liquid::request::ParseResult::COMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_HEADERS);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 3);
                REQUIRE(request.GetHeader("cONNECTION").value() == "keep-alive");
                REQUIRE(request.GetHeader("Accept").value() == "*/*");
                REQUIRE(request.GetHeader("X-CONTENT-LENGTH").value() == "0");
            }
        }
    }
}

SCENARIO("Parsing multiple incomplete header lines.")
{
    GIVEN("A complete Request-Line")
    {
        std::string request_data =
            "GET /derp.html HTTP/1.1\r\n"
            "Connection: keep-";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect to have parsed through the version.")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_VERSION);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 0);
            }
        }

        request_data +=
            "alive\r\n"
            "Accept: */*\r";

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect to have parsed a single header.")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_VERSION);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 1);
                REQUIRE(request.GetHeader("Connection").value() == "keep-alive");
            }
        }

        request_data +=
            "\n"
            "Content-Length: 0\r\n"
            "\r\n";

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect to complete parsing the headers.")
            {
                REQUIRE(result == liquid::request::ParseResult::COMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                // we've got a content length so a body will show up even if its empty
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_BODY);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 3);
                REQUIRE(request.GetHeader("Connection").value() == "keep-alive");
                REQUIRE(request.GetHeader("Accept").value() == "*/*");
                REQUIRE(request.GetHeader("CONTENT-LENGTH").value() == "0");
            }
        }
    }
}

SCENARIO("Parsing multiple header lines with arbitrary whitespace in the headers.")
{
    GIVEN("A complete Request-Line")
    {
        std::string request_data =
            "GET /derp.html HTTP/1.1\r\n"
            "        Connection  :  keep-alive\r\n"
            "  Accept:  */*\t  \r\n"
            "    \t     X-Content-Length      :        0   \r\n"
            "\r\n";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI")
            {
                REQUIRE(result == liquid::request::ParseResult::COMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_HEADERS);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 3);
                REQUIRE(request.GetHeader("cONNECTION").value() == "keep-alive");
                REQUIRE(request.GetHeader("Accept").value() == "*/*");
                REQUIRE(request.GetHeader("X-CONTENT-LENGTH").value() == "0");
            }
        }
    }
}

SCENARIO("Parsing a request with an END OF STREAM body.")
{
    GIVEN("A complete POST request with body.")
    {
        std::string request_data =
            "POST /derp?key=flerp HTTP/1.1\r\n"
            "Connection: keep-alive\r\n"
            "Accept: */*\r\n"
            "\r\n"
            "0123456789";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the body to be parsed.")
            {
                REQUIRE(result == liquid::request::ParseResult::COMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::POST);
                // This looks incorrect, but since END OF STREAM cannot be determined by the parser
                // it never marks this 'body' method as complete so a subsequent call can pickup more data.
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_HEADERS);
                REQUIRE(request.GetUri() == "/derp?key=flerp");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 2);
                REQUIRE(request.GetHeader("Connection").value() == "keep-alive");
                REQUIRE(request.GetHeader("Accept").value() == "*/*");
                REQUIRE(request.GetBody().value() == "0123456789");
            }
        }

        request_data += "0123456789";
        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the body to be parsed.")
            {
                REQUIRE(result == liquid::request::ParseResult::COMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::POST);
                // This looks incorrect, but since END OF STREAM cannot be determined by the parser
                // it never marks this 'body' method as complete so a subsequent call can pickup more data.
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_HEADERS);
                REQUIRE(request.GetUri() == "/derp?key=flerp");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 2);
                REQUIRE(request.GetHeader("Connection").value() == "keep-alive");
                REQUIRE(request.GetHeader("Accept").value() == "*/*");
                REQUIRE(request.GetBody().value() == "01234567890123456789");
            }
        }
    }
}

SCENARIO("Parsing a request with a Content-Length body.")
{
    GIVEN("A POST request with Content-Length + body")
    {
        std::string request_data =
            "POST /derp.html HTTP/1.1\r\n"
            "Connection: keep-alive\r\n"
            "Accept: */*\r\n"
            "Content-Length: 10\r\n"
            "\r\n"
            "0123456789";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be POST and have a parsed URI and a full body.")
            {
                REQUIRE(result == liquid::request::ParseResult::COMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::POST);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_BODY);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 3);
                REQUIRE(request.GetHeader("Connection").value() == "keep-alive");
                REQUIRE(request.GetHeader("Accept").value() == "*/*");
                REQUIRE(request.GetHeader("Content-Length").value() == "10");
                REQUIRE(request.GetBody().value() == "0123456789");
            }
        }
    }
}

SCENARIO("Parsing a request with a Transfer-Encoding: chunked body.")
{
    GIVEN("A POST request with Transfer-Encoding: chunked + body.")
    {
        std::string request_data =
            "POST /derp.html HTTP/1.1\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "4\r\n"
            "Wiki\r\n"
            "5\r\n"
            "pedia\r\n"
            "E\r\n"
            " in\r\n"
            "\r\n"
            "chunks.\r\n"
            "0\r\n"
            "\r\n";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be POST with a chunked encoded body.")
            {
                REQUIRE(result == liquid::request::ParseResult::COMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::POST);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_BODY);
                REQUIRE(request.GetUri() == "/derp.html");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 1);
                REQUIRE(request.GetHeader("Transfer-Encoding").value() == "chunked");
//                std::string encoded_body =
//                    "4\r\n"
//                    "Wiki\r\n"
//                    "5\r\n"
//                    "pedia\r\n"
//                    "E\r\n"
//                    " in\r\n"
//                    "\r\n"
//                    "chunks.\r\n"
//                    "0\r\n"
//                    "\r\n";
                std::string decoded_body =
                    "Wikipedia in\r\n"
                    "\r\n"
                    "chunks.";
                REQUIRE(request.GetBody().value() == decoded_body);
            }
        }
    }
}

SCENARIO("Parse PicoHTTPParser performance request.")
{
    GIVEN("The buffer")
    {
        std::string buffer =
            "GET /cookies HTTP/1.1\r\n"
            "Host: 127.0.0.1:8090\r\n"
            "Connection: keep-alive\r\n"
            "Cache-Control: max-age=0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17\r\n"
            "Accept-Encoding: gzip,deflate,sdch\r\n"
            "Accept-Language: en-US,en;q=0.8\r\n"
            "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\r\n"
            "Cookie: name=wookie\r\n\r\n";

        liquid::request::Request request{};
        WHEN("Parsed")
        {
            auto result = request.Parse(buffer);
            THEN("We expect it to parse correctly.")
            {
                REQUIRE(result == liquid::request::ParseResult::COMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::GET);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::PARSED_HEADERS);
                REQUIRE(request.GetUri() == "/cookies");
                REQUIRE(request.GetVersion() == liquid::Version::V1_1);
                REQUIRE(request.GetHeaderCount() == 9);
                REQUIRE(request.GetHeader("Host").value() == "127.0.0.1:8090");
                REQUIRE(request.GetHeader("Connection").value() == "keep-alive");
                REQUIRE(request.GetHeader("Cache-Control").value() == "max-age=0");
                REQUIRE(request.GetHeader("Accept").value() == "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
                REQUIRE(request.GetHeader("User-Agent").value() == "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
                REQUIRE(request.GetHeader("Accept-Encoding").value() == "gzip,deflate,sdch");
                REQUIRE(request.GetHeader("Accept-Language").value() == "en-US,en;q=0.8");
                REQUIRE(request.GetHeader("Accept-Charset").value() == "ISO-8859-1,utf-8;q=0.7,*;q=0.3");
                REQUIRE(request.GetHeader("Cookie").value() == "name=wookie");
            }
        }
    }
}