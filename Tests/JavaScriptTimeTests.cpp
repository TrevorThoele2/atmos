#include <catch.hpp>

#include "JavaScriptTimeTests.h"

SCENARIO_METHOD(JavaScriptTimeTestsFixture, "running time JavaScript scripts", "[script][javascript][logging]")
{
    GIVEN("script that returns current time")
    {
        auto id = dataGeneration.Random<World::FieldID>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    return {
        done: true,
        value: Atmos.Time.currentTime().nanoseconds
    };
};)V0G0N",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<long long>(std::get<Variant>(finishes[0].result));
                REQUIRE(result > 0);
            }
        }
    }

    GIVEN("script that returns current frame time")
    {
        auto id = dataGeneration.Random<World::FieldID>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    return {
        done: true,
        value: Atmos.Time.currentFrameTime().nanoseconds
    };
};)V0G0N",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<long long>(std::get<Variant>(finishes[0].result));
                REQUIRE(result > 0);
            }
        }
    }
}