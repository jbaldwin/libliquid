#include "catch.hpp"
#include "turbohttp/turbohttp.hpp"

using namespace turbo::http;

SCENARIO("REQUEST:Parsing an empty string.")
{
    GIVEN("An empty HTTP Parser data.")
    {
        std::string request_data{};
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.state() == request_parse_state::start);
            }
        }
    }
}

SCENARIO("REQUEST:Parsing GET Method")
{
    GIVEN("A GET HTTP Parser")
    {
        std::string request_data = "GET ";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_method);
            }
        }
    }

    GIVEN("An incomplete GET HTTP Parser")
    {
        std::string request_data = "G";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.state() == request_parse_state::start);
            }
        }

        request_data = "GE";
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.state() == request_parse_state::start);
            }
        }

        request_data = "GET";
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.state() == request_parse_state::start);
            }
        }

        request_data = "GET ";
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the parser to return INCOMPLETE with method GET")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_method);
            }
        }
    }
}

SCENARIO("REQUEST:Parsing POST Method")
{
    GIVEN("A POST HTTP Parser")
    {
        std::string request_data = "POST ";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be POST")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::post);
                REQUIRE(request.state() == request_parse_state::parsed_method);
            }
        }
    }
    GIVEN("An incomplete POST HTTP Parser")
    {
        std::string request_data = "P";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.state() == request_parse_state::start);
            }
        }

        request_data = "PO";
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.state() == request_parse_state::start);
            }
        }

        request_data = "POS";
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.state() == request_parse_state::start);
            }
        }

        request_data = "POST";
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.state() == request_parse_state::start);
            }
        }

        request_data = "POST ";
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the parser to return INCOMPLETE")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::post);
                REQUIRE(request.state() == request_parse_state::parsed_method);
            }
        }
    }
}

SCENARIO("REQUEST:Parsing PUT Method")
{
    GIVEN("A PUT HTTP Parser")
    {
        std::string request_data = "PUT ";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be PUT")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::put);
                REQUIRE(request.state() == request_parse_state::parsed_method);
            }
        }
    }
}

SCENARIO("REQUEST:Parsing PATCH Method")
{
    GIVEN("A PATCH HTTP Parser")
    {
        std::string request_data = "PATCH ";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be PATCH")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::patch);
                REQUIRE(request.state() == request_parse_state::parsed_method);
            }
        }
    }
}

SCENARIO("REQUEST:Parsing HEAD Method")
{
    GIVEN("A HEAD HTTP Parser")
    {
        std::string request_data = "HEAD ";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be HEAD")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::head);
                REQUIRE(request.state() == request_parse_state::parsed_method);
            }
        }
    }
}

SCENARIO("REQUEST:Parsing DELETE Method")
{
    GIVEN("A DELETE HTTP Parser")
    {
        std::string request_data = "DELETE ";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be DELETE")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::http_delete);
                REQUIRE(request.state() == request_parse_state::parsed_method);
            }
        }
    }
}

SCENARIO("REQUEST:Parsing a complete URI")
{
    GIVEN("A complete URI")
    {
        std::string request_data = "GET /derp.html ";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_uri);
                REQUIRE(request.http_uri() == "/derp.html");
            }
        }
    }
}

SCENARIO("REQUEST:Parsing an incomplete URI")
{
    GIVEN("A complete URI")
    {
        std::string request_data = "GET /derp.html?flerpity=";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and not have parsed the URI yet")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_method);
            }
        }

        request_data.append("merpity");
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and not have parsed the URI yet")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_method);
            }
        }

        request_data.append(" ");
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and parsed the URI")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_uri);
                REQUIRE(request.http_uri() == "/derp.html?flerpity=merpity");
            }
        }
    }
}

SCENARIO("REQUEST:Parsing a complete HTTP Version")
{
    GIVEN("A complete Parser-Line")
    {
        std::string request_data = "GET /derp.html HTTP/1.0\r\n";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_version);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_0);
            }
        }
    }
}

SCENARIO("REQUEST:Parsing an incomplete HTTP Version")
{
    GIVEN("An complete Parser-Line")
    {
        std::string request_data = "GET /derp.html HTTP/";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_uri);
                REQUIRE(request.http_uri() == "/derp.html");
            }
        }

        request_data.append("1");
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_uri);
                REQUIRE(request.http_uri() == "/derp.html");
            }
        }

        request_data.append(".");
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_uri);
                REQUIRE(request.http_uri() == "/derp.html");
            }
        }

        request_data.append("1");
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_uri);
                REQUIRE(request.http_uri() == "/derp.html");
            }
        }

        request_data.append("\r");
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_uri);
                REQUIRE(request.http_uri() == "/derp.html");
            }
        }

        request_data.append("\n");
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI and have a version")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_version);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_1);
            }
        }

        request_data.append("\r\n");
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI and have a version and a complete request.")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_version);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_1);
            }
        }
    }
}

SCENARIO("REQUEST:Parsing a body with zero headers.")
{
    GIVEN("A complete Parser-Line with no headers but a EOF body, parser will ignore the body.")
    {
        std::string request_data = "GET /derp.html HTTP/1.1\r\n\r\nHERP DERP";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI but no version yet")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::get);
                // This will never get updated to PARSED_BODY since its EOF stream,
                // which doesnt' set parsed body since the parser cannot know if its the EOF stream or not.
                REQUIRE(request.state() == request_parse_state::parsed_version);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_header_count() == 0);
                // This has a body but the parser will ignore it since it didn't get any indication to its length.
                REQUIRE(!request.http_body().has_value());
            }
        }
    }
}

SCENARIO("REQUEST:Parsing a header line.")
{
    GIVEN("A complete Parser-Line")
    {
        std::string request_data = "GET /derp.html HTTP/1.1\r\nConnection: keep-alive\r\n\r\n";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_headers);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 1);
                REQUIRE(request.http_header("cONNECTION").value() == "keep-alive");
            }
        }
    }
}

SCENARIO("REQUEST:Parsing multiple header lines.")
{
    GIVEN("A complete Parser-Line")
    {
        std::string request_data =
            "GET /derp.html HTTP/1.1\r\n"
            "Connection: keep-alive\r\n"
            "Accept: */*\r\n"
            "X-Content-Length: 0\r\n"
            "\r\n";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_headers);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 3);
                REQUIRE(request.http_header("cONNECTION").value() == "keep-alive");
                REQUIRE(request.http_header("Accept").value() == "*/*");
                REQUIRE(request.http_header("X-CONTENT-LENGTH").value() == "0");
            }
        }
    }
}

SCENARIO("REQUEST:Parsing multiple incomplete header lines.")
{
    GIVEN("A complete Parser-Line")
    {
        std::string request_data =
            "GET /derp.html HTTP/1.1\r\n"
            "Connection: keep-";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect to have parsed through the version.")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_version);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 0);
            }
        }

        request_data +=
            "alive\r\n"
            "Accept: */*\r";

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect to have parsed a single header.")
            {
                REQUIRE(result == request_parse_result::incomplete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_version);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 1);
                REQUIRE(request.http_header("Connection").value() == "keep-alive");
            }
        }

        request_data +=
            "\n"
            "Content-Length: 0\r\n"
            "\r\n";

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect to complete parsing the headers.")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::get);
                // we've got a content length so a body will show up even if its empty
                REQUIRE(request.state() == request_parse_state::parsed_body);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 3);
                REQUIRE(request.http_header("Connection").value() == "keep-alive");
                REQUIRE(request.http_header("Accept").value() == "*/*");
                REQUIRE(request.http_header("CONTENT-LENGTH").value() == "0");
            }
        }
    }
}

SCENARIO("REQUEST:Parsing multiple header lines with arbitrary whitespace in the headers.")
{
    GIVEN("A complete Parser-Line")
    {
        std::string request_data =
            "GET /derp.html HTTP/1.1\r\n"
            "Connection:  keep-alive\r\n"
            "Accept:  */*\t  \r\n"
            "X-Content-Length:        0   \r\n"
            "\r\n";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be GET and have a parsed URI")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_headers);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 3);
                REQUIRE(request.http_header("cONNECTION").value() == "keep-alive");
                REQUIRE(request.http_header("Accept").value() == "*/*");
                REQUIRE(request.http_header("X-CONTENT-LENGTH").value() == "0");
            }
        }
    }
}

SCENARIO("REQUEST:Parsing a request with an END OF STREAM body.")
{
    GIVEN("A complete POST request with body.")
    {
        std::string request_data =
            "POST /derp?key=flerp HTTP/1.1\r\n"
            "Connection: keep-alive\r\n"
            "Accept: */*\r\n"
            "\r\n"
            "0123456789";
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the body to be ignored.")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::post);
                // This looks incorrect, but since END OF STREAM cannot be determined by the parser
                // it never marks this 'body' method as complete so a subsequent call can pickup more data.
                REQUIRE(request.state() == request_parse_state::parsed_headers);
                REQUIRE(request.http_uri() == "/derp?key=flerp");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 2);
                REQUIRE(request.http_header("Connection").value() == "keep-alive");
                REQUIRE(request.http_header("Accept").value() == "*/*");
                REQUIRE(!request.http_body().has_value()); // EOS body isn't parsed
            }
        }

        request_data += "0123456789";
        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the body be ignored by the parser.")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::post);
                // This looks incorrect, but since END OF STREAM cannot be determined by the parser
                // it never marks this 'body' method as complete so a subsequent call can pickup more data.
                REQUIRE(request.state() == request_parse_state::parsed_headers);
                REQUIRE(request.http_uri() == "/derp?key=flerp");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 2);
                REQUIRE(request.http_header("Connection").value() == "keep-alive");
                REQUIRE(request.http_header("Accept").value() == "*/*");
                REQUIRE(!request.http_body().has_value());
            }
        }
    }
}

SCENARIO("REQUEST:Parsing a request with a Content-Length body.")
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
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be POST and have a parsed URI and a full body.")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::post);
                REQUIRE(request.state() == request_parse_state::parsed_body);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 3);
                REQUIRE(request.http_header("Connection").value() == "keep-alive");
                REQUIRE(request.http_header("Accept").value() == "*/*");
                REQUIRE(request.http_header("Content-Length").value() == "10");
                REQUIRE(request.http_body().value() == "0123456789");
            }
        }
    }
}

SCENARIO("REQUEST:Parsing a request with a Transfer-Encoding: chunked body.")
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
        request request{};

        WHEN("Parsed")
        {
            auto result = request.parse(request_data);
            THEN("We expect the method to be POST with a chunked encoded body.")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::post);
                REQUIRE(request.state() == request_parse_state::parsed_body);
                REQUIRE(request.http_uri() == "/derp.html");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 1);
                REQUIRE(request.http_header("Transfer-Encoding").value() == "chunked");
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
                REQUIRE(request.http_body().value() == decoded_body);
            }
        }
    }
}

SCENARIO("REQUEST:Parse PicoHTTPParser performance request.")
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

        request request{};
        WHEN("Parsed")
        {
            auto result = request.parse(buffer);
            THEN("We expect it to parse correctly.")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_headers);
                REQUIRE(request.http_uri() == "/cookies");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 9);
                REQUIRE(request.http_header("Host").value() == "127.0.0.1:8090");
                REQUIRE(request.http_header("Connection").value() == "keep-alive");
                REQUIRE(request.http_header("Cache-Control").value() == "max-age=0");
                REQUIRE(request.http_header("Accept").value() == "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
                REQUIRE(request.http_header("User-Agent").value() == "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
                REQUIRE(request.http_header("Accept-Encoding").value() == "gzip,deflate,sdch");
                REQUIRE(request.http_header("Accept-Language").value() == "en-US,en;q=0.8");
                REQUIRE(request.http_header("Accept-Charset").value() == "ISO-8859-1,utf-8;q=0.7,*;q=0.3");
                REQUIRE(request.http_header("Cookie").value() == "name=wookie");
            }
        }
    }
}

SCENARIO("REQUEST:http_header_for_each")
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

        request request{};
        WHEN("Parsed")
        {
            auto result = request.parse(buffer);
            THEN("We expect it to parse correctly.")
            {
                REQUIRE(result == request_parse_result::complete);
                REQUIRE(request.http_method() == method::get);
                REQUIRE(request.state() == request_parse_state::parsed_headers);
                REQUIRE(request.http_uri() == "/cookies");
                REQUIRE(request.http_version() == version::v1_1);
                REQUIRE(request.http_header_count() == 9);
                request.http_header_for_each([](std::string_view name, std::string_view value) {
                    if(name == "Host")
                    {
                        REQUIRE(value == "127.0.0.1:8090");
                    }
                    else if(name == "Connection")
                    {
                        REQUIRE(value == "keep-alive");
                    }
                    else if(name == "Cache-Control")
                    {
                        REQUIRE(value == "max-age=0");
                    }
                    else if(name == "Accept")
                    {
                        REQUIRE(value == "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
                    }
                    else if(name == "User-Agent")
                    {
                        REQUIRE(value == "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
                    }
                    else if(name == "Accept-Encoding")
                    {
                        REQUIRE(value == "gzip,deflate,sdch");
                    }
                    else if(name == "Accept-Language")
                    {
                        REQUIRE(value == "en-US,en;q=0.8");
                    }
                    else if(name == "Accept-Charset")
                    {
                        REQUIRE(value == "ISO-8859-1,utf-8;q=0.7,*;q=0.3");
                    }
                    else if(name == "Cookie")
                    {
                        REQUIRE(value == "name=wookie");
                    }
                });
            }
        }
    }
}