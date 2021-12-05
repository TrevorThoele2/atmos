#include <catch.hpp>

#include "JavaScriptTests.h"

#include <Atmos/Script.h>
#include <Atmos/Work.h>
#include <Atmos/ScriptFinished.h>

SCENARIO_METHOD(JavaScriptTestsFixture, "running JavaScript scripts", "[script][javascript]")
{
    GIVEN("empty script")
    {
        CompileAndCreateScript(
            "basic_script.ts",
            "export const main = () => {};",
            *fieldReliquary);

        WHEN("working reliquary")
        {
            THEN("no error is thrown")
            {
                REQUIRE_NOTHROW(fieldReliquary->Do(Work{}));
            }

            THEN("script is destroyed")
            {
                fieldReliquary->Do(Work{});

                auto scriptCount = fieldReliquary->Batch<Scripting::Script>().Size();

                REQUIRE(scriptCount == 0);
            }
        }
    }

    GIVEN("script that runs function from shared module")
    {
        CompileAndCreateScriptWithSharedModules(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";
import { doThing } from "library";

export const main = (): Atmos.Result => {
    return {
        done: true,
        value: doThing()
    };
};)V0G0N",
            std::vector<Scripting::Module>
            {
                {
                    "library.ts",
                    "export const doThing = (): number => {\n" \
                    "    return 123;\n" \
                    "};"
                }
            },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<int>(std::get<Variant>(finishes[0].result)) == 123);
            }
        }
    }

    GIVEN("script that runs multiple times")
    {
        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

let hasRun = false;

export const main = (): Atmos.Result => {
    const result: Atmos.Result = {
        done: hasRun,
        value: !hasRun ? null : "abc"
    };
    hasRun = true;
    return result;
};)V0G0N",
            *fieldReliquary);

        WHEN("working reliquary once")
        {
            fieldReliquary->Do(Work{});

            THEN("has no finish")
            {
                REQUIRE(finishes.empty());
            }
        }

        WHEN("working reliquary twice")
        {
            fieldReliquary->Do(Work{});
            fieldReliquary->Do(Work{});

            THEN("has correct finish")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == "abc");
            }
        }
    }
}