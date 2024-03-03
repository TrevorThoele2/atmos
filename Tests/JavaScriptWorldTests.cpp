#include <catch.hpp>

#include "JavaScriptWorldTests.h"

SCENARIO_METHOD(JavaScriptWorldTestsFixture, "running world JavaScript scripts", "[script][javascript][logging]")
{
    GIVEN("script that requests field")
    {
        auto id = dataGeneration.Random<World::FieldID>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.World.FieldId): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.World.RequestField.typeName, { id });
    return {
        done: true
    };
};)V0G0N",
            { id },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);
            }
        }
    }
}