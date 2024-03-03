#include <catch.hpp>

#include "AngelScriptStopwatchTests.h"

SCENARIO_METHOD(AngelScriptStopwatchTestsFixture, "running stopwatch AngelScript scripts", "[script][angelscript][time]")
{
    GIVEN("real stopwatch")
    {
        GIVEN("script that returns resume")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "int64 main()\n" \
                "{\n" \
                "    auto stopwatch = Atmos::Time::CreateRealStopwatch();\n" \
                "    stopwatch.Pause();\n" \
                "    return stopwatch.Resume().TimeSinceEpoch().Count();\n" \
                "}",
                {},
                *fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary->Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<long long>(std::get<Variant>(finishes[0].result)) > 0);
                }
            }
        }

        GIVEN("script that returns restart")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "int64 main()\n" \
                "{\n" \
                "    auto stopwatch = Atmos::Time::CreateRealStopwatch();\n" \
                "    return stopwatch.Restart().TimeSinceEpoch().Count();\n" \
                "}",
                {},
                *fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary->Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<long long>(std::get<Variant>(finishes[0].result)) > 0);
                }
            }
        }
    }
}