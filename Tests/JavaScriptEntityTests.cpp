#include <catch.hpp>

#include "JavaScriptEntityTests.h"

#include <Atmos/JavaScriptEntity.h>
#include <Atmos/FindPath.h>

#include <Inscription/Json.h>

SCENARIO_METHOD(JavaScriptEntityTestsFixture, "running entity JavaScript scripts", "[script][javascript][logging]")
{
    GIVEN("script that retrieves current entity")
    {
        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (): Atmos.Result => {
    const entity = Atmos.Entity.currentEntity();
    return {
        done: true,
        value: JSON.stringify(entity, (key, value) => typeof value === 'bigint' ? value.toString() : value)
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
            }
        }
    }

    GIVEN("script that sends entity FindByName")
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto position = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        const auto isSolid = dataGeneration.Random<bool>();
        const auto tags = std::vector<Tag>();

        auto entity = fieldReliquary->Do(Arca::Create<Atmos::Entity::Entity>{name, position, isSolid, tags});

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (name: string): Atmos.Result => {
    const entity = Atmos.Reliquary.send(Atmos.Traits.Entity.FindByName.typeName, { name });
    return {
        done: true,
        value: JSON.stringify(entity, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { name },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Atmos::Scripting::JavaScript::Entity resultEntity;
                Inscription::Json::FromString(resultEntity, result);

                REQUIRE(resultEntity.id == entity.ID());
                REQUIRE(resultEntity.position.x == position.x);
                REQUIRE(resultEntity.position.y == position.y);
                REQUIRE(resultEntity.isSolid == isSolid);
                REQUIRE(resultEntity.tags == tags);
            }
        }
    }

    GIVEN("script that sends entity FindByPosition")
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto position = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        const auto isSolid = dataGeneration.Random<bool>();
        const auto tags = std::vector<Tag>();

        auto entity = fieldReliquary->Do(Arca::Create<Atmos::Entity::Entity>{name, position, isSolid, tags});

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (positionJson: string): Atmos.Result => {
    const position = JSON.parse(positionJson);
    const entity = Atmos.Reliquary.send(Atmos.Traits.Entity.FindByPosition.typeName, { position });
    return {
        done: true,
        value: JSON.stringify(entity, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { Inscription::Json::ToString(position) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                std::vector<Atmos::Scripting::JavaScript::Entity> resultEntities;
                Inscription::Json::FromString(resultEntities, result);

                REQUIRE(resultEntities.size() == 1);
                REQUIRE(resultEntities[0].id == entity.ID());
                REQUIRE(resultEntities[0].position.x == position.x);
                REQUIRE(resultEntities[0].position.y == position.y);
                REQUIRE(resultEntities[0].isSolid == isSolid);
                REQUIRE(resultEntities[0].tags == tags);
            }
        }
    }

    GIVEN("script that sends entity MoveTo")
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto position = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        const auto isSolid = dataGeneration.Random<bool>();
        const auto tags = std::vector<Tag>();

        auto entity = fieldReliquary->Do(Arca::Create<Atmos::Entity::Entity>{name, position, isSolid, tags});

        const auto moveTo = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (entity: Atmos.RelicId, positionJson: string): Atmos.Result => {
    const position = JSON.parse(positionJson);
    Atmos.Reliquary.send(Atmos.Traits.Entity.MoveTo.typeName, { entity, to: position });
    return {
        done: true
    };
};)V0G0N",
            { entity.ID(), Inscription::Json::ToString(moveTo) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto newPosition = entity->position;
                REQUIRE(newPosition == moveTo);
            }
        }
    }

    GIVEN("script that sends entity CanMoveTo")
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto position = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        const auto isSolid = dataGeneration.Random<bool>();
        const auto tags = std::vector<Tag>();

        auto entity = fieldReliquary->Do(Arca::Create<Atmos::Entity::Entity>{name, position, isSolid, tags});

        const auto moveTo = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (entity: Atmos.RelicId, positionJson: string): Atmos.Result => {
    const position = JSON.parse(positionJson);
    const canMoveTo = Atmos.Reliquary.send(Atmos.Traits.Entity.CanMoveTo.typeName, { entity, to: position });
    return {
        done: true,
        value: canMoveTo
    };
};)V0G0N",
            { entity.ID(), Inscription::Json::ToString(moveTo) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(this->finishes[0].result));

                REQUIRE(result == true);
            }
        }
    }

    GIVEN("script that sends entity ModifyTags")
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto position = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        const auto isSolid = dataGeneration.Random<bool>();
        const auto tags = std::vector<Tag>();

        auto entity = fieldReliquary->Do(Arca::Create<Atmos::Entity::Entity>{name, position, isSolid, tags});

        const auto newTags = std::vector
        {
            dataGeneration.Random<std::string>(),
            dataGeneration.Random<std::string>(),
            dataGeneration.Random<std::string>()
        };

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (entity: Atmos.RelicId, tagsJson: string): Atmos.Result => {
    const tags = JSON.parse(tagsJson);
    Atmos.Reliquary.send(Atmos.Traits.Entity.ModifyTags.typeName, { entity, add: tags, remove: [] });
    return {
        done: true
    };
};)V0G0N",
            { entity.ID(), Inscription::Json::ToString(newTags) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                REQUIRE(entity->tags.size() == newTags.size());
                REQUIRE(entity->tags[0] == newTags[0]);
                REQUIRE(entity->tags[1] == newTags[1]);
                REQUIRE(entity->tags[2] == newTags[2]);
            }
        }
    }

    GIVEN("script that sends entity FindPath")
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto position = Spatial::Grid::Point{ 0, 0 };
        const auto isSolid = dataGeneration.Random<bool>();
        const auto tags = std::vector<Tag>();

        auto entity = fieldReliquary->Do(Arca::Create<Atmos::Entity::Entity>{name, position, isSolid, tags});

        const auto otherPosition = Spatial::Grid::Point{ 2, 0 };

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (entity: Atmos.RelicId, positionJson: string): Atmos.Result => {
    const position = JSON.parse(positionJson);
    const path = Atmos.Reliquary.send(Atmos.Traits.Entity.FindPath.typeName, { entity, to: position });
    return {
        done: true,
        value: JSON.stringify(path, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { entity.ID(), Inscription::Json::ToString(otherPosition) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                std::vector<Atmos::Spatial::Grid::Point> resultPath;
                Inscription::Json::FromString(resultPath, result);

                const auto expectedPath = fieldReliquary->Do(Atmos::Entity::FindPath{ entity.ID(), otherPosition });
                REQUIRE(expectedPath == resultPath);
            }
        }
    }
}