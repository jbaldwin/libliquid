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
                REQUIRE(request.GetParseState() == liquid::request::ParseState::METHOD_PARSED);
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
                REQUIRE(request.GetParseState() == liquid::request::ParseState::METHOD_PARSED);
            }
        }
    }
}

SCENARIO("Parsing POST Method")
{
    GIVEN("A POST HTTP Request")
    {
        std::string request_data = "POST /derp.html HTTP/1.1\r\n\r\n";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be POST")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::POST);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::METHOD_PARSED);
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
                REQUIRE(request.GetParseState() == liquid::request::ParseState::METHOD_PARSED);
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
                REQUIRE(request.GetParseState() == liquid::request::ParseState::METHOD_PARSED);
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
                REQUIRE(request.GetParseState() == liquid::request::ParseState::METHOD_PARSED);
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
                REQUIRE(request.GetParseState() == liquid::request::ParseState::METHOD_PARSED);
            }
        }
    }
}

SCENARIO("Parsing DELETE Method")
{
    GIVEN("A DELETE HTTP Request")
    {
        std::string request_data = "DELETE /derp.html HTTP/1.1\r\n\r\n";
        liquid::request::Request request{};

        WHEN("Parsed")
        {
            auto result = request.Parse(request_data);
            THEN("We expect the method to be DELETE")
            {
                REQUIRE(result == liquid::request::ParseResult::INCOMPLETE);
                REQUIRE(request.GetMethod() == liquid::Method::DELETE);
                REQUIRE(request.GetParseState() == liquid::request::ParseState::METHOD_PARSED);
            }
        }
    }
}