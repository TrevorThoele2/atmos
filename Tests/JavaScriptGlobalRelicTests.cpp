#include <catch.hpp>

#include "JavaScriptGlobalRelicTests.h"

#include <Chroma/StringUtility.h>

TEMPLATE_TEST_CASE_METHOD(
    JavaScriptGlobalRelicTestsFixture,
    "running global relic JavaScript scripts",
    "[script][javascript][local][relic]",
    Atmos::Diagnostics::Statistics,
    Atmos::Render::Camera)
{
    GIVEN("script that finds relic")
    {
        auto [traits, expectations] = this->template Scenario<TestType>(*this->fieldReliquary);

        this->CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    const found = Atmos.Reliquary.find()V0G0N" + traits + R"V0G0N(.typeName);
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            {},
            *this->fieldReliquary);

        WHEN("working reliquary")
        {
            this->fieldReliquary->Do(Work{});

            THEN("returns JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));
                expectations(result);
            }
        }
    }
}