#include "catch.hpp"
#include <turbohttp/turbohttp.hpp>

using namespace turbo::http;

SCENARIO("RESPONSE: Parsing an empty string.")
{
    GIVEN("An empty HTTP Parser data.")
    {
        std::string response_data{};
        response response{};

        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::start);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing a valid HTTP/1.0 response.")
{
    GIVEN("An HTTP response with only the version.")
    {
        std::string response_data = "HTTP/1.0 ";
        response response{};

        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_version);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing an unknown HTTP/1.0 response.")
{
    GIVEN("An HTTP response with an unknown version.")
    {
        std::string response_data = "HTTP/1.x ";
        response response{};

        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::http_version_unknown);
                REQUIRE(response.state() == response_parse_state::start);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing a truncated valid HTTP/1.0 response.")
{
    GIVEN("An HTTP response with truncated version data")
    {
        std::string response_data = "H";
        response response{};

        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::start);
            }
        }

        response_data += "T";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::start);
            }
        }
        response_data += "T";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::start);
            }
        }
        response_data += "P";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::start);
            }
        }

        response_data += "/";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::start);
            }
        }
        response_data += "1";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::start);
            }
        }
        response_data += ".";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::start);
            }
        }
        response_data += "1";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::start);
            }
        }
        response_data += " ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_version);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing up to a valid status codes.")
{
    GIVEN("An HTTP response with a status code.")
    {
        std::string response_data = "HTTP/1.1 100 ";
        response response{};

        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 100);
            }
        }

        response_data = "HTTP/1.1 101 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 101);
            }
        }

        response_data = "HTTP/1.1 102 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 102);
            }
        }

        response_data = "HTTP/1.1 103 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 103);
            }
        }

        response_data = "HTTP/1.1 200 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 200);
            }
        }

        response_data = "HTTP/1.1 201 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 201);
            }
        }

        response_data = "HTTP/1.1 202 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 202);
            }
        }

        response_data = "HTTP/1.1 203 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 203);
            }
        }

        response_data = "HTTP/1.1 204 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 204);
            }
        }

        response_data = "HTTP/1.1 205 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 205);
            }
        }

        response_data = "HTTP/1.1 206 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 206);
            }
        }

        response_data = "HTTP/1.1 207 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 207);
            }
        }

        response_data = "HTTP/1.1 208 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 208);
            }
        }

        response_data = "HTTP/1.1 226 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 226);
            }
        }

        response_data = "HTTP/1.1 300 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 300);
            }
        }

        response_data = "HTTP/1.1 301 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 301);
            }
        }

        response_data = "HTTP/1.1 302 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 302);
            }
        }

        response_data = "HTTP/1.1 303 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 303);
            }
        }

        response_data = "HTTP/1.1 304 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 304);
            }
        }

        response_data = "HTTP/1.1 305 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 305);
            }
        }

        response_data = "HTTP/1.1 306 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 306);
            }
        }

        response_data = "HTTP/1.1 307 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 307);
            }
        }

        response_data = "HTTP/1.1 308 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 308);
            }
        }

        response_data = "HTTP/1.1 400 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 400);
            }
        }

        response_data = "HTTP/1.1 401 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 401);
            }
        }

        response_data = "HTTP/1.1 402 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 402);
            }
        }

        response_data = "HTTP/1.1 403 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 403);
            }
        }

        response_data = "HTTP/1.1 404 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 404);
            }
        }

        response_data = "HTTP/1.1 405 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 405);
            }
        }

        response_data = "HTTP/1.1 406 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 406);
            }
        }

        response_data = "HTTP/1.1 407 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 407);
            }
        }

        response_data = "HTTP/1.1 408 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 408);
            }
        }

        response_data = "HTTP/1.1 409 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 409);
            }
        }

        response_data = "HTTP/1.1 410 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 410);
            }
        }

        response_data = "HTTP/1.1 411 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 411);
            }
        }

        response_data = "HTTP/1.1 412 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 412);
            }
        }

        response_data = "HTTP/1.1 413 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 413);
            }
        }

        response_data = "HTTP/1.1 414 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 414);
            }
        }

        response_data = "HTTP/1.1 415 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 415);
            }
        }

        response_data = "HTTP/1.1 416 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 416);
            }
        }

        response_data = "HTTP/1.1 417 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 417);
            }
        }

        response_data = "HTTP/1.1 418 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 418);
            }
        }

        response_data = "HTTP/1.1 421 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 421);
            }
        }

        response_data = "HTTP/1.1 422 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 422);
            }
        }

        response_data = "HTTP/1.1 423 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 423);
            }
        }

        response_data = "HTTP/1.1 424 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 424);
            }
        }

        response_data = "HTTP/1.1 426 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 426);
            }
        }

        response_data = "HTTP/1.1 428 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 428);
            }
        }

        response_data = "HTTP/1.1 429 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 429);
            }
        }

        response_data = "HTTP/1.1 431 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 431);
            }
        }

        response_data = "HTTP/1.1 451 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 451);
            }
        }

        response_data = "HTTP/1.1 500 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 500);
            }
        }

        response_data = "HTTP/1.1 500 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 500);
            }
        }

        response_data = "HTTP/1.1 501 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 501);
            }
        }

        response_data = "HTTP/1.1 502 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 502);
            }
        }

        response_data = "HTTP/1.1 503 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 503);
            }
        }

        response_data = "HTTP/1.1 504 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 504);
            }
        }

        response_data = "HTTP/1.1 505 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 505);
            }
        }

        response_data = "HTTP/1.1 506 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 506);
            }
        }

        response_data = "HTTP/1.1 507 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 507);
            }
        }

        response_data = "HTTP/1.1 508 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 508);
            }
        }

        response_data = "HTTP/1.1 510 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 510);
            }
        }

        response_data = "HTTP/1.1 511 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_status_code);
                REQUIRE(response.http_status_code() == 511);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing up to an invalid status code.")
{
    GIVEN("An HTTP response with an invalid status code.")
    {
        std::string response_data = "HTTP/1.1 10  ";
        response response{};

        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::http_status_code_malformed);
                REQUIRE(response.state() == response_parse_state::parsed_version);
            }
        }

        response_data = "HTTP/1.1  00 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::http_status_code_malformed);
                REQUIRE(response.state() == response_parse_state::parsed_version);
            }
        }

        response_data = "HTTP/1.1 4 4 ";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::http_status_code_malformed);
                REQUIRE(response.state() == response_parse_state::parsed_version);
            }
        }

        response_data = "HTTP/1.1 4040";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::http_status_code_malformed);
                REQUIRE(response.state() == response_parse_state::parsed_version);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing up to a reason phrase.")
{
    GIVEN("An HTTP response with an invalid status code.")
    {
        std::string response_data = "HTTP/1.1 100 Continue\r\n";
        response response{};

        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 100);
                REQUIRE(response.http_reason_phrase() == "Continue");
            }
        }

        response_data = "HTTP/1.1 101 Switching Protocols\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 101);
                REQUIRE(response.http_reason_phrase() == "Switching Protocols");
            }
        }

        response_data = "HTTP/1.1 102 Processing\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 102);
                REQUIRE(response.http_reason_phrase() == "Processing");
            }
        }

        response_data = "HTTP/1.1 103 Early Hints\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 103);
                REQUIRE(response.http_reason_phrase() == "Early Hints");
            }
        }

        response_data = "HTTP/1.1 200 OK\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 200);
                REQUIRE(response.http_reason_phrase() == "OK");
            }
        }

        response_data = "HTTP/1.1 201 Created\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 201);
                REQUIRE(response.http_reason_phrase() == "Created");
            }
        }

        response_data = "HTTP/1.1 202 Accepted\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 202);
                REQUIRE(response.http_reason_phrase() == "Accepted");
            }
        }

        response_data = "HTTP/1.1 203 Non-Authoritative Information\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 203);
                REQUIRE(response.http_reason_phrase() == "Non-Authoritative Information");
            }
        }

        response_data = "HTTP/1.1 204 No Content\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 204);
                REQUIRE(response.http_reason_phrase() == "No Content");
            }
        }

        response_data = "HTTP/1.1 205 Reset Content\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 205);
                REQUIRE(response.http_reason_phrase() == "Reset Content");
            }
        }

        response_data = "HTTP/1.1 206 Partial Content\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 206);
                REQUIRE(response.http_reason_phrase() == "Partial Content");
            }
        }

        response_data = "HTTP/1.1 207 Multi-Status\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 207);
                REQUIRE(response.http_reason_phrase() == "Multi-Status");
            }
        }

        response_data = "HTTP/1.1 208 Already Reported\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 208);
                REQUIRE(response.http_reason_phrase() == "Already Reported");
            }
        }

        response_data = "HTTP/1.1 226 IM Used\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 226);
                REQUIRE(response.http_reason_phrase() == "IM Used");
            }
        }

        response_data = "HTTP/1.1 300 Multiple Choices\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 300);
                REQUIRE(response.http_reason_phrase() == "Multiple Choices");
            }
        }

        response_data = "HTTP/1.1 301 Moved Permanently\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 301);
                REQUIRE(response.http_reason_phrase() == "Moved Permanently");
            }
        }

        response_data = "HTTP/1.1 302 Found\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 302);
                REQUIRE(response.http_reason_phrase() == "Found");
            }
        }

        response_data = "HTTP/1.1 303 See Other\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 303);
                REQUIRE(response.http_reason_phrase() == "See Other");
            }
        }

        response_data = "HTTP/1.1 304 Not Modified\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 304);
                REQUIRE(response.http_reason_phrase() == "Not Modified");
            }
        }

        response_data = "HTTP/1.1 305 Use Proxy\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 305);
                REQUIRE(response.http_reason_phrase() == "Use Proxy");
            }
        }

        response_data = "HTTP/1.1 306 Switch Proxy\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 306);
                REQUIRE(response.http_reason_phrase() == "Switch Proxy");
            }
        }

        response_data = "HTTP/1.1 307 Temporary Redirect\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 307);
                REQUIRE(response.http_reason_phrase() == "Temporary Redirect");
            }
        }

        response_data = "HTTP/1.1 308 Permanent Redirect\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 308);
                REQUIRE(response.http_reason_phrase() == "Permanent Redirect");
            }
        }

        response_data = "HTTP/1.1 400 Bad request\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 400);
                REQUIRE(response.http_reason_phrase() == "Bad request");
            }
        }

        response_data = "HTTP/1.1 401 Unauthorized\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 401);
                REQUIRE(response.http_reason_phrase() == "Unauthorized");
            }
        }

        response_data = "HTTP/1.1 402 Payment Required\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 402);
                REQUIRE(response.http_reason_phrase() == "Payment Required");
            }
        }

        response_data = "HTTP/1.1 403 Forbidden\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 403);
                REQUIRE(response.http_reason_phrase() == "Forbidden");
            }
        }

        response_data = "HTTP/1.1 404 Not Found\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 404);
                REQUIRE(response.http_reason_phrase() == "Not Found");
            }
        }

        response_data = "HTTP/1.1 405 Method Not Allowed\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 405);
                REQUIRE(response.http_reason_phrase() == "Method Not Allowed");
            }
        }

        response_data = "HTTP/1.1 406 Not Acceptable\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 406);
                REQUIRE(response.http_reason_phrase() == "Not Acceptable");
            }
        }

        response_data = "HTTP/1.1 407 Proxy Authentication Required\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 407);
                REQUIRE(response.http_reason_phrase() == "Proxy Authentication Required");
            }
        }

        response_data = "HTTP/1.1 408 request Timeout\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 408);
                REQUIRE(response.http_reason_phrase() == "request Timeout");
            }
        }

        response_data = "HTTP/1.1 409 Conflict\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 409);
                REQUIRE(response.http_reason_phrase() == "Conflict");
            }
        }

        response_data = "HTTP/1.1 410 Gone\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 410);
                REQUIRE(response.http_reason_phrase() == "Gone");
            }
        }

        response_data = "HTTP/1.1 411 Length Required\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 411);
                REQUIRE(response.http_reason_phrase() == "Length Required");
            }
        }

        response_data = "HTTP/1.1 412 Precondition Failed\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 412);
                REQUIRE(response.http_reason_phrase() == "Precondition Failed");
            }
        }

        response_data = "HTTP/1.1 413 Payload Too Large\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 413);
                REQUIRE(response.http_reason_phrase() == "Payload Too Large");
            }
        }

        response_data = "HTTP/1.1 414 URI Too Long\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 414);
                REQUIRE(response.http_reason_phrase() == "URI Too Long");
            }
        }

        response_data = "HTTP/1.1 415 Unsupported Media Type\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 415);
                REQUIRE(response.http_reason_phrase() == "Unsupported Media Type");
            }
        }

        response_data = "HTTP/1.1 416 Range Not Satisfiable\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 416);
                REQUIRE(response.http_reason_phrase() == "Range Not Satisfiable");
            }
        }

        response_data = "HTTP/1.1 417 Expectation Failed\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 417);
                REQUIRE(response.http_reason_phrase() == "Expectation Failed");
            }
        }

        response_data = "HTTP/1.1 418 I'm a teapot\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 418);
                REQUIRE(response.http_reason_phrase() == "I'm a teapot");
            }
        }

        response_data = "HTTP/1.1 421 Misdirected request\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 421);
                REQUIRE(response.http_reason_phrase() == "Misdirected request");
            }
        }

        response_data = "HTTP/1.1 422 Unprocessable Entity\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 422);
                REQUIRE(response.http_reason_phrase() == "Unprocessable Entity");
            }
        }

        response_data = "HTTP/1.1 423 Locked\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 423);
                REQUIRE(response.http_reason_phrase() == "Locked");
            }
        }

        response_data = "HTTP/1.1 424 Failed Dependency\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 424);
                REQUIRE(response.http_reason_phrase() == "Failed Dependency");
            }
        }

        response_data = "HTTP/1.1 426 Upgrade Required\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 426);
                REQUIRE(response.http_reason_phrase() == "Upgrade Required");
            }
        }

        response_data = "HTTP/1.1 428 Precondition Failed\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 428);
                REQUIRE(response.http_reason_phrase() == "Precondition Failed");
            }
        }

        response_data = "HTTP/1.1 429 Too Many requests\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 429);
                REQUIRE(response.http_reason_phrase() == "Too Many requests");
            }
        }

        response_data = "HTTP/1.1 431 request Header Fields Too Large\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 431);
                REQUIRE(response.http_reason_phrase() == "request Header Fields Too Large");
            }
        }

        response_data = "HTTP/1.1 451 Unavailable For Legal Reasons\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 451);
                REQUIRE(response.http_reason_phrase() == "Unavailable For Legal Reasons");
            }
        }

        response_data = "HTTP/1.1 500 Internal Server Error\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 500);
                REQUIRE(response.http_reason_phrase() == "Internal Server Error");
            }
        }

        response_data = "HTTP/1.1 501 Not Implemented\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 501);
                REQUIRE(response.http_reason_phrase() == "Not Implemented");
            }
        }

        response_data = "HTTP/1.1 502 Bad Gateway\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 502);
                REQUIRE(response.http_reason_phrase() == "Bad Gateway");
            }
        }

        response_data = "HTTP/1.1 503 Service Unavailable\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 503);
                REQUIRE(response.http_reason_phrase() == "Service Unavailable");
            }
        }

        response_data = "HTTP/1.1 504 Gateway Timeout\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 504);
                REQUIRE(response.http_reason_phrase() == "Gateway Timeout");
            }
        }

        response_data = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 505);
                REQUIRE(response.http_reason_phrase() == "HTTP Version Not Supported");
            }
        }

        response_data = "HTTP/1.1 506 Variant Also Negotiates\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 506);
                REQUIRE(response.http_reason_phrase() == "Variant Also Negotiates");
            }
        }

        response_data = "HTTP/1.1 507 Insufficient Storage\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 507);
                REQUIRE(response.http_reason_phrase() == "Insufficient Storage");
            }
        }

        response_data = "HTTP/1.1 508 Loop Detected\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 508);
                REQUIRE(response.http_reason_phrase() == "Loop Detected");
            }
        }

        response_data = "HTTP/1.1 510 Not Extended\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 510);
                REQUIRE(response.http_reason_phrase() == "Not Extended");
            }
        }

        response_data = "HTTP/1.1 511 Network Authentication Required\r\n";
        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::incomplete);
                REQUIRE(response.state() == response_parse_state::parsed_reason_phrase);
                REQUIRE(response.http_status_code() == 511);
                REQUIRE(response.http_reason_phrase() == "Network Authentication Required");
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing up through headers.")
{
    GIVEN("An HTTP response with headers.")
    {
        std::string response_data =
            "HTTP/1.1 200 OK\r\n"
            "X-random-header: nullvalue\r\n"
            "\r\n";
        response response{};

        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::complete);
                REQUIRE(response.state() == response_parse_state::parsed_headers);
            }
        }
    }
}

SCENARIO("RESPONSE: Parsing a content length http_body.")
{
    GIVEN("An HTTP response with headers.")
    {
        std::string response_data =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 5\r\n"
            "\r\n"
            "12345";
        response response{};

        WHEN("Parsed")
        {
            auto result = response.parse(response_data);
            THEN("We expect the parser to not fail.")
            {
                REQUIRE(result == response_parse_result::complete);
                REQUIRE(response.state() == response_parse_state::parsed_body);
                REQUIRE(response.http_body().has_value());
                REQUIRE(response.http_body().value() == "12345");
            }
        }
    }
}