#include <catch.hpp>

#include "JavaScriptLocalRelicTests.h"

#include <Arca/Create.h>

#include <Chroma/StringUtility.h>

TEMPLATE_TEST_CASE_METHOD(
    JavaScriptLocalRelicTestsFixture,
    "running local relic JavaScript scripts",
    "[script][javascript][local][relic]",
    Atmos::Asset::Action,
    Atmos::Asset::Audio,
    Atmos::Asset::Font,
    Atmos::Asset::Image,
    Atmos::Asset::Material,
    Atmos::Asset::Script,
    Atmos::Asset::Shader)
{
    GIVEN("script that finds relic")
    {
        auto [index, traits, expectations] = this->template Scenario<TestType>(*this->fieldReliquary);

        this->CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    const found = Atmos.Reliquary.find()V0G0N" + traits + R"V0G0N(.typeName, id);
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { index.ID() },
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

TEMPLATE_TEST_CASE_METHOD(
    JavaScriptCreatableLocalRelicTestsFixture,
    "running creatable local relic JavaScript scripts",
    "[script][javascript][local][relic]",
    Atmos::Audio::PositionedSound,
    Atmos::Audio::UniversalSound,
    Atmos::Entity::Entity,
    Atmos::UI::Image,
    Atmos::UI::Text,
    Atmos::Render::DynamicImage,
    Atmos::Render::DynamicText,
    Atmos::Render::GridRegion,
    Atmos::Render::Line,
    Atmos::Scripting::Script)
{
    GIVEN("script that creates, finds and destroys relic")
    {
        auto [createCommand, createTraits, traits, destroyTraits, expectations] =
            this->template Scenario<TestType>(*this->fieldReliquary);

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
                REQUIRE(this->finishes.size() >= 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[this->finishes.size() - 1].result));
                expectations(result, *this->fieldReliquary);
            }
        }
    }
}