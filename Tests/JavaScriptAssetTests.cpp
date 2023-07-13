#include <catch.hpp>

#include "JavaScriptAssetTests.h"

#include <Arca/Create.h>

#include <Chroma/StringUtility.h>

TEMPLATE_TEST_CASE_METHOD(
    JavaScriptAssetTestsFixture,
    "running asset JavaScript scripts",
    "[script][javascript][local][relic]",
    Atmos::Asset::Action,
    Atmos::Asset::Audio,
    Atmos::Asset::Font,
    Atmos::Asset::Image,
    Atmos::Asset::Material,
    Atmos::Asset::Script,
    Atmos::Asset::Shader)
{
    GIVEN("script that finds asset by name")
    {
        auto [name, traits, expectations] = this->template Scenario<TestType>(*this->fieldReliquary);

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (name: string): Atmos.Result => {
    const found = Atmos.Reliquary.send()V0G0N" + traits + R"V0G0N(.typeName, { name} );
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { name },
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