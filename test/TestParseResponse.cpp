#include "Catch2.h"
#include "liquid/Parser.h"

SCENARIO("RESPONSE: Parsing an empty string.")
{
    GIVEN("An empty HTTP Parser data.")
    {
        std::string response_data{};
        liquid::Response response{};

        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::START);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing a valid HTTP/1.0 response.")
{
    GIVEN("An HTTP response with only the version.")
    {
        std::string response_data = "HTTP/1.0 ";
        liquid::Response response{};

        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_VERSION);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing an unknown HTTP/1.0 response.")
{
    GIVEN("An HTTP response with an unknown version.")
    {
        std::string response_data = "HTTP/1.x ";
        liquid::Response response{};

        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::HTTP_VERSION_UNKNOWN);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::START);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing a truncated valid HTTP/1.0 response.")
{
    GIVEN("An HTTP response with truncated version data")
    {
        std::string response_data = "H";
        liquid::Response response{};

        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::START);
            }
        }

        response_data += "T";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::START);
            }
        }
        response_data += "T";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::START);
            }
        }
        response_data += "P";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::START);
            }
        }

        response_data += "/";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::START);
            }
        }
        response_data += "1";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::START);
            }
        }
        response_data += ".";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::START);
            }
        }
        response_data += "1";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::START);
            }
        }
        response_data += " ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_VERSION);
            }
        }
    }
}