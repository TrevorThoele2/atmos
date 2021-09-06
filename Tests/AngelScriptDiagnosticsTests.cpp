#include <catch.hpp>

#include "AngelScriptDiagnosticsTests.h"

SCENARIO_METHOD(AngelScriptDiagnosticsTestsFixture, "running diagnostics AngelScript scripts", "[script][angelscript]")
{
    GIVEN("DiagnosticsStatistics")
    {
        GIVEN("script that returns RelicCount")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "uint32 main()\n" \
                "{\n" \
                "    Atmos::Diagnostics::Statistics statistics;\n" \
                "    return statistics.RelicCount();\n" \
                "}",
                {},
                *fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary->Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto result = std::get<size_t>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == 0);
                }
            }
        }

        GIVEN("script that returns ShardCount")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "uint32 main()\n" \
                "{\n" \
                "    Atmos::Diagnostics::Statistics statistics;\n" \
                "    return statistics.ShardCount();\n" \
                "}",
                {},
                *fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary->Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto result = std::get<size_t>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == 0);
                }
            }
        }
    }
}