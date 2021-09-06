#include <catch.hpp>

#include "AngelScriptDurationTests.h"

SCENARIO_METHOD(AngelScriptDurationTestsFixture, "running duration AngelScript scripts", "[script][angelscript][time]")
{
    GIVEN("script that returns equals for same duration")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "bool main()\n" \
            "{\n" \
            "    auto duration = Atmos::Time::Nanoseconds(500);\n" \
            "    return duration == duration;\n" \
            "}",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == true);
            }
        }
    }

    GIVEN("script that returns not equals for same duration")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "bool main()\n" \
            "{\n" \
            "    auto duration = Atmos::Time::Nanoseconds(500);\n" \
            "    return duration != duration;\n" \
            "}",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == false);
            }
        }
    }

    GIVEN("script that returns less than for same duration")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "bool main()\n" \
            "{\n" \
            "    auto duration = Atmos::Time::Nanoseconds(500);\n" \
            "    return duration < duration;\n" \
            "}",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == false);
            }
        }
    }

    GIVEN("script that returns less than or equals for same duration")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "bool main()\n" \
            "{\n" \
            "    auto duration = Atmos::Time::Nanoseconds(500);\n" \
            "    return duration <= duration;\n" \
            "}",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == true);
            }
        }
    }
}