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

SCENARIO("RESPONSE: Parsing up to a valid status codes.")
{
    GIVEN("An HTTP response with a status code.")
    {
        std::string response_data = "HTTP/1.1 100 ";
        liquid::Response response{};

        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 100);
            }
        }

        response_data = "HTTP/1.1 101 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 101);
            }
        }

        response_data = "HTTP/1.1 102 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 102);
            }
        }

        response_data = "HTTP/1.1 103 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 103);
            }
        }

        response_data = "HTTP/1.1 200 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 200);
            }
        }

        response_data = "HTTP/1.1 201 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 201);
            }
        }

        response_data = "HTTP/1.1 202 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 202);
            }
        }

        response_data = "HTTP/1.1 203 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 203);
            }
        }

        response_data = "HTTP/1.1 204 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 204);
            }
        }

        response_data = "HTTP/1.1 205 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 205);
            }
        }

        response_data = "HTTP/1.1 206 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 206);
            }
        }

        response_data = "HTTP/1.1 207 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 207);
            }
        }

        response_data = "HTTP/1.1 208 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 208);
            }
        }

        response_data = "HTTP/1.1 226 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 226);
            }
        }

        response_data = "HTTP/1.1 300 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 300);
            }
        }

        response_data = "HTTP/1.1 301 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 301);
            }
        }

        response_data = "HTTP/1.1 302 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 302);
            }
        }

        response_data = "HTTP/1.1 303 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 303);
            }
        }

        response_data = "HTTP/1.1 304 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 304);
            }
        }

        response_data = "HTTP/1.1 305 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 305);
            }
        }

        response_data = "HTTP/1.1 306 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 306);
            }
        }

        response_data = "HTTP/1.1 307 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 307);
            }
        }

        response_data = "HTTP/1.1 308 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 308);
            }
        }

        response_data = "HTTP/1.1 400 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 400);
            }
        }

        response_data = "HTTP/1.1 401 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 401);
            }
        }

        response_data = "HTTP/1.1 402 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 402);
            }
        }

        response_data = "HTTP/1.1 403 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 403);
            }
        }

        response_data = "HTTP/1.1 404 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 404);
            }
        }

        response_data = "HTTP/1.1 405 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 405);
            }
        }

        response_data = "HTTP/1.1 406 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 406);
            }
        }

        response_data = "HTTP/1.1 407 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 407);
            }
        }

        response_data = "HTTP/1.1 408 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 408);
            }
        }

        response_data = "HTTP/1.1 409 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 409);
            }
        }

        response_data = "HTTP/1.1 410 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 410);
            }
        }

        response_data = "HTTP/1.1 411 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 411);
            }
        }

        response_data = "HTTP/1.1 412 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 412);
            }
        }

        response_data = "HTTP/1.1 413 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 413);
            }
        }

        response_data = "HTTP/1.1 414 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 414);
            }
        }

        response_data = "HTTP/1.1 415 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 415);
            }
        }

        response_data = "HTTP/1.1 416 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 416);
            }
        }

        response_data = "HTTP/1.1 417 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 417);
            }
        }

        response_data = "HTTP/1.1 418 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 418);
            }
        }

        response_data = "HTTP/1.1 421 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 421);
            }
        }

        response_data = "HTTP/1.1 422 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 422);
            }
        }

        response_data = "HTTP/1.1 423 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 423);
            }
        }

        response_data = "HTTP/1.1 424 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 424);
            }
        }

        response_data = "HTTP/1.1 426 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 426);
            }
        }

        response_data = "HTTP/1.1 428 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 428);
            }
        }

        response_data = "HTTP/1.1 429 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 429);
            }
        }

        response_data = "HTTP/1.1 431 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 431);
            }
        }

        response_data = "HTTP/1.1 451 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 451);
            }
        }

        response_data = "HTTP/1.1 500 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 500);
            }
        }

        response_data = "HTTP/1.1 500 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 500);
            }
        }

        response_data = "HTTP/1.1 501 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 501);
            }
        }

        response_data = "HTTP/1.1 502 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 502);
            }
        }

        response_data = "HTTP/1.1 503 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 503);
            }
        }

        response_data = "HTTP/1.1 504 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 504);
            }
        }

        response_data = "HTTP/1.1 505 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 505);
            }
        }

        response_data = "HTTP/1.1 506 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 506);
            }
        }

        response_data = "HTTP/1.1 507 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 507);
            }
        }

        response_data = "HTTP/1.1 508 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 508);
            }
        }

        response_data = "HTTP/1.1 510 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 510);
            }
        }

        response_data = "HTTP/1.1 511 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_STATUS_CODE);
                REQUIRE(response.GetStatusCode() == 511);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing up to an invalid status code.")
{
    GIVEN("An HTTP response with an invalid status code.")
    {
        std::string response_data = "HTTP/1.1 10  ";
        liquid::Response response{};

        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::HTTP_STATUS_CODE_MALFORMED);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_VERSION);
            }
        }

        response_data = "HTTP/1.1  00 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::HTTP_STATUS_CODE_MALFORMED);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_VERSION);
            }
        }

        response_data = "HTTP/1.1 4 4 ";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::HTTP_STATUS_CODE_MALFORMED);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_VERSION);
            }
        }

        response_data = "HTTP/1.1 4040";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::HTTP_STATUS_CODE_MALFORMED);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_VERSION);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing up to a reason phrase.")
{
    GIVEN("An HTTP response with an invalid status code.")
    {
        std::string response_data = "HTTP/1.1 100 Continue\r\n";
        liquid::Response response{};

        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 100);
                REQUIRE(response.GetReasonPhrase() == "Continue");
            }
        }

        response_data = "HTTP/1.1 101 Switching Protocols\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 101);
                REQUIRE(response.GetReasonPhrase() == "Switching Protocols");
            }
        }

        response_data = "HTTP/1.1 102 Processing\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 102);
                REQUIRE(response.GetReasonPhrase() == "Processing");
            }
        }

        response_data = "HTTP/1.1 103 Early Hints\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 103);
                REQUIRE(response.GetReasonPhrase() == "Early Hints");
            }
        }

        response_data = "HTTP/1.1 200 OK\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 200);
                REQUIRE(response.GetReasonPhrase() == "OK");
            }
        }

        response_data = "HTTP/1.1 201 Created\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 201);
                REQUIRE(response.GetReasonPhrase() == "Created");
            }
        }

        response_data = "HTTP/1.1 202 Accepted\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 202);
                REQUIRE(response.GetReasonPhrase() == "Accepted");
            }
        }

        response_data = "HTTP/1.1 203 Non-Authoritative Information\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 203);
                REQUIRE(response.GetReasonPhrase() == "Non-Authoritative Information");
            }
        }

        response_data = "HTTP/1.1 204 No Content\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 204);
                REQUIRE(response.GetReasonPhrase() == "No Content");
            }
        }

        response_data = "HTTP/1.1 205 Reset Content\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 205);
                REQUIRE(response.GetReasonPhrase() == "Reset Content");
            }
        }

        response_data = "HTTP/1.1 206 Partial Content\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 206);
                REQUIRE(response.GetReasonPhrase() == "Partial Content");
            }
        }

        response_data = "HTTP/1.1 207 Multi-Status\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 207);
                REQUIRE(response.GetReasonPhrase() == "Multi-Status");
            }
        }

        response_data = "HTTP/1.1 208 Already Reported\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 208);
                REQUIRE(response.GetReasonPhrase() == "Already Reported");
            }
        }

        response_data = "HTTP/1.1 226 IM Used\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 226);
                REQUIRE(response.GetReasonPhrase() == "IM Used");
            }
        }

        response_data = "HTTP/1.1 300 Multiple Choices\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 300);
                REQUIRE(response.GetReasonPhrase() == "Multiple Choices");
            }
        }

        response_data = "HTTP/1.1 301 Moved Permanently\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 301);
                REQUIRE(response.GetReasonPhrase() == "Moved Permanently");
            }
        }

        response_data = "HTTP/1.1 302 Found\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 302);
                REQUIRE(response.GetReasonPhrase() == "Found");
            }
        }

        response_data = "HTTP/1.1 303 See Other\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 303);
                REQUIRE(response.GetReasonPhrase() == "See Other");
            }
        }

        response_data = "HTTP/1.1 304 Not Modified\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 304);
                REQUIRE(response.GetReasonPhrase() == "Not Modified");
            }
        }

        response_data = "HTTP/1.1 305 Use Proxy\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 305);
                REQUIRE(response.GetReasonPhrase() == "Use Proxy");
            }
        }

        response_data = "HTTP/1.1 306 Switch Proxy\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 306);
                REQUIRE(response.GetReasonPhrase() == "Switch Proxy");
            }
        }

        response_data = "HTTP/1.1 307 Temporary Redirect\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 307);
                REQUIRE(response.GetReasonPhrase() == "Temporary Redirect");
            }
        }

        response_data = "HTTP/1.1 308 Permanent Redirect\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 308);
                REQUIRE(response.GetReasonPhrase() == "Permanent Redirect");
            }
        }

        response_data = "HTTP/1.1 400 Bad Request\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 400);
                REQUIRE(response.GetReasonPhrase() == "Bad Request");
            }
        }

        response_data = "HTTP/1.1 401 Unauthorized\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 401);
                REQUIRE(response.GetReasonPhrase() == "Unauthorized");
            }
        }

        response_data = "HTTP/1.1 402 Payment Required\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 402);
                REQUIRE(response.GetReasonPhrase() == "Payment Required");
            }
        }

        response_data = "HTTP/1.1 403 Forbidden\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 403);
                REQUIRE(response.GetReasonPhrase() == "Forbidden");
            }
        }

        response_data = "HTTP/1.1 404 Not Found\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 404);
                REQUIRE(response.GetReasonPhrase() == "Not Found");
            }
        }

        response_data = "HTTP/1.1 405 Method Not Allowed\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 405);
                REQUIRE(response.GetReasonPhrase() == "Method Not Allowed");
            }
        }

        response_data = "HTTP/1.1 406 Not Acceptable\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 406);
                REQUIRE(response.GetReasonPhrase() == "Not Acceptable");
            }
        }

        response_data = "HTTP/1.1 407 Proxy Authentication Required\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 407);
                REQUIRE(response.GetReasonPhrase() == "Proxy Authentication Required");
            }
        }

        response_data = "HTTP/1.1 408 Request Timeout\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 408);
                REQUIRE(response.GetReasonPhrase() == "Request Timeout");
            }
        }

        response_data = "HTTP/1.1 409 Conflict\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 409);
                REQUIRE(response.GetReasonPhrase() == "Conflict");
            }
        }

        response_data = "HTTP/1.1 410 Gone\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 410);
                REQUIRE(response.GetReasonPhrase() == "Gone");
            }
        }

        response_data = "HTTP/1.1 411 Length Required\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 411);
                REQUIRE(response.GetReasonPhrase() == "Length Required");
            }
        }

        response_data = "HTTP/1.1 412 Precondition Failed\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 412);
                REQUIRE(response.GetReasonPhrase() == "Precondition Failed");
            }
        }

        response_data = "HTTP/1.1 413 Payload Too Large\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 413);
                REQUIRE(response.GetReasonPhrase() == "Payload Too Large");
            }
        }

        response_data = "HTTP/1.1 414 URI Too Long\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 414);
                REQUIRE(response.GetReasonPhrase() == "URI Too Long");
            }
        }

        response_data = "HTTP/1.1 415 Unsupported Media Type\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 415);
                REQUIRE(response.GetReasonPhrase() == "Unsupported Media Type");
            }
        }

        response_data = "HTTP/1.1 416 Range Not Satisfiable\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 416);
                REQUIRE(response.GetReasonPhrase() == "Range Not Satisfiable");
            }
        }

        response_data = "HTTP/1.1 417 Expectation Failed\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 417);
                REQUIRE(response.GetReasonPhrase() == "Expectation Failed");
            }
        }

        response_data = "HTTP/1.1 418 I'm a teapot\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 418);
                REQUIRE(response.GetReasonPhrase() == "I'm a teapot");
            }
        }

        response_data = "HTTP/1.1 421 Misdirected Request\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 421);
                REQUIRE(response.GetReasonPhrase() == "Misdirected Request");
            }
        }

        response_data = "HTTP/1.1 422 Unprocessable Entity\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 422);
                REQUIRE(response.GetReasonPhrase() == "Unprocessable Entity");
            }
        }

        response_data = "HTTP/1.1 423 Locked\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 423);
                REQUIRE(response.GetReasonPhrase() == "Locked");
            }
        }

        response_data = "HTTP/1.1 424 Failed Dependency\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 424);
                REQUIRE(response.GetReasonPhrase() == "Failed Dependency");
            }
        }

        response_data = "HTTP/1.1 426 Upgrade Required\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 426);
                REQUIRE(response.GetReasonPhrase() == "Upgrade Required");
            }
        }

        response_data = "HTTP/1.1 428 Precondition Failed\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 428);
                REQUIRE(response.GetReasonPhrase() == "Precondition Failed");
            }
        }

        response_data = "HTTP/1.1 429 Too Many Requests\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 429);
                REQUIRE(response.GetReasonPhrase() == "Too Many Requests");
            }
        }

        response_data = "HTTP/1.1 431 Request Header Fields Too Large\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 431);
                REQUIRE(response.GetReasonPhrase() == "Request Header Fields Too Large");
            }
        }

        response_data = "HTTP/1.1 451 Unavailable For Legal Reasons\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 451);
                REQUIRE(response.GetReasonPhrase() == "Unavailable For Legal Reasons");
            }
        }

        response_data = "HTTP/1.1 500 Internal Server Error\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 500);
                REQUIRE(response.GetReasonPhrase() == "Internal Server Error");
            }
        }

        response_data = "HTTP/1.1 501 Not Implemented\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 501);
                REQUIRE(response.GetReasonPhrase() == "Not Implemented");
            }
        }

        response_data = "HTTP/1.1 502 Bad Gateway\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 502);
                REQUIRE(response.GetReasonPhrase() == "Bad Gateway");
            }
        }

        response_data = "HTTP/1.1 503 Service Unavailable\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 503);
                REQUIRE(response.GetReasonPhrase() == "Service Unavailable");
            }
        }

        response_data = "HTTP/1.1 504 Gateway Timeout\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 504);
                REQUIRE(response.GetReasonPhrase() == "Gateway Timeout");
            }
        }

        response_data = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 505);
                REQUIRE(response.GetReasonPhrase() == "HTTP Version Not Supported");
            }
        }

        response_data = "HTTP/1.1 506 Variant Also Negotiates\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 506);
                REQUIRE(response.GetReasonPhrase() == "Variant Also Negotiates");
            }
        }

        response_data = "HTTP/1.1 507 Insufficient Storage\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 507);
                REQUIRE(response.GetReasonPhrase() == "Insufficient Storage");
            }
        }

        response_data = "HTTP/1.1 508 Loop Detected\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 508);
                REQUIRE(response.GetReasonPhrase() == "Loop Detected");
            }
        }

        response_data = "HTTP/1.1 510 Not Extended\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 510);
                REQUIRE(response.GetReasonPhrase() == "Not Extended");
            }
        }

        response_data = "HTTP/1.1 511 Network Authentication Required\r\n";
        WHEN("Parsed")
        {
            auto result = response.Parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == liquid::ResponseParseResult::INCOMPLETE);
                REQUIRE(response.GetParseState() == liquid::ResponseParseState::PARSED_REASON_PHRASE);
                REQUIRE(response.GetStatusCode() == 511);
                REQUIRE(response.GetReasonPhrase() == "Network Authentication Required");
            }
        }
    }
}