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