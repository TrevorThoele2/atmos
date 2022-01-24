#include <catch.hpp>

#include "JavaScriptMathTests.h"

SCENARIO_METHOD(JavaScriptMathTestsFixture, "running math JavaScript scripts", "[script][javascript][math]")
{
    GIVEN("script that returns maximum float")
    {
        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    return {
        done: true,
        value: Atmos.Math.Float.maximum
    };
};)V0G0N",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<double>(std::get<Variant>(this->finishes[0].result));
                REQUIRE(result == std::numeric_limits<float>::max());
            }
        }
    }
}