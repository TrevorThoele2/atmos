#include <catch.hpp>

#include "JavaScriptShardTests.h"

#include <Arca/Create.h>

#include <Chroma/StringUtility.h>

TEMPLATE_TEST_CASE_METHOD(
    JavaScriptShardTestsFixture,
    "running shard JavaScript scripts",
    "[script][javascript][shard]",
    Atmos::Spatial::Bounds,
    Atmos::Render::ImageCore,
    Atmos::Render::RenderCore,
    Atmos::Render::ViewSlice)
{
    GIVEN("script that creates, finds and destroys shard")
    {
        auto relic = this->fieldReliquary->Do(Arca::Create<Arca::OpenRelic>{});

        auto [createCommand, createTraits, traits, destroyTraits, expectations] =
            this->template Scenario<TestType>(relic.ID(), *this->fieldReliquary);

        const auto createCommandJson = Inscription::Json::ToString(createCommand);

        this->CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    const created = Atmos.Reliquary.send()V0G0N" + createTraits + R"V0G0N(.typeName, )V0G0N" + createCommandJson + R"V0G0N()
    const found = Atmos.Reliquary.find()V0G0N" + traits + R"V0G0N(.typeName, created.id);
    Atmos.Reliquary.send()V0G0N" + destroyTraits + R"V0G0N(.typeName, { id: created.id });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            *this->fieldReliquary);

        WHEN("working reliquary")
        {
            this->fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));
                expectations(result, *this->fieldReliquary);
            }
        }
    }
}