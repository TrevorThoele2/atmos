#include <catch.hpp>

#include "AngelScriptArcaTests.h"

SCENARIO_METHOD(AngelScriptArcaTestsFixture, "running Arca AngelScript scripts", "[script][angelscript]")
{
    GIVEN("script that returns Arca::nullRelicID")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main()\n" \
            "{\n" \
            "    return Arca::nullRelicID;\n" \
            "}",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == Arca::nullRelicID);
            }
        }
    }
}