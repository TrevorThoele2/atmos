#include <catch.hpp>

#include "AngelScriptEntityTests.h"

#include "ScriptEngine.h"

#include <Atmos/AngelScriptEntity.h>

#include <Atmos/SpatialAlgorithms.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>
#include <Arca/LocalRelic.h>

SCENARIO_METHOD(AngelScriptEntityTestsFixture, "running entity AngelScript scripts", "[script][angelscript]")
{
    ScriptEngine engine;
    engine.Setup();

    auto fieldOrigin = Arca::ReliquaryOrigin();
    fieldOrigin.Register<Arca::OpenRelic>();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockImageAssetManager,
        *engine.nullAudioManager,
        *engine.nullInputManager,
        *engine.mockGraphicsManager,
        *engine.scriptManager,
        Spatial::ScreenSize{
            std::numeric_limits<Spatial::ScreenSize::Dimension>::max(),
            std::numeric_limits<Spatial::ScreenSize::Dimension>::max() },
            *engine.mockWindow,
            engine.Logger());
    fieldOrigin.CuratorCommandPipeline<Work>(Arca::Pipeline{ Scripting::Stage() });
    World::Field field(0, fieldOrigin.Actualize());

    auto& fieldReliquary = field.Reliquary();

    engine.mockGraphicsManager->Initialize();

    std::vector<Scripting::Finished> finishes;
    fieldReliquary.On<Scripting::Finished>([&finishes](const Scripting::Finished& signal)
        {
            finishes.push_back(signal);
        });

    const auto name = dataGeneration.Random<std::string>();
    const auto displayName = dataGeneration.Random<std::string>();
    const auto position = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
    const auto direction = dataGeneration.Random<Spatial::Angle2D>();
    const auto isSolid = dataGeneration.Random<bool>();

    auto entity = fieldReliquary.Do(Arca::Create<Entity::Entity>{ name, displayName, position, direction, isSolid });

    GIVEN("script that returns name")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "string main(Arca::RelicID entityID)\n" \
            "{\n" \
            "    auto entity = Atmos::Entity::Entity(entityID);\n" \
            "    return entity.Name();\n" \
            "}",
            { entity.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == name);
            }
        }
    }

    GIVEN("script that returns display name")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "string main(Arca::RelicID entityID)\n" \
            "{\n" \
            "    auto entity = Atmos::Entity::Entity(entityID);\n" \
            "    return entity.DisplayName();\n" \
            "}",
            { entity.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == displayName);
            }
        }
    }

    GIVEN("script that returns position")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "string main(Arca::RelicID entityID)\n" \
            "{\n" \
            "    auto entity = Atmos::Entity::Entity(entityID);\n" \
            "    return Atmos::ToString(entity.Position().x) + \" \" + Atmos::ToString(entity.Position().y);\n" \
            "}",
            { entity.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = ToString(position.x) + " " + ToString(position.y);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == expectedResult);
            }
        }
    }

    GIVEN("script that returns direction")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "Atmos::Spatial::Angle2D main(Arca::RelicID entityID)\n" \
            "{\n" \
            "    auto entity = Atmos::Entity::Entity(entityID);\n" \
            "    return entity.Direction();\n" \
            "}",
            { entity.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                REQUIRE(std::get<Spatial::Angle2D>(std::get<Variant>(finishes[0].result)) == direction);
            }
        }
    }

    GIVEN("script that returns is solid")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "bool main(Arca::RelicID entityID)\n" \
            "{\n" \
            "    auto entity = Atmos::Entity::Entity(entityID);\n" \
            "    return entity.IsSolid();\n" \
            "}",
            { entity.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == isSolid);
            }
        }
    }

    GIVEN("script that finds entity by name")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main(string name)\n" \
            "{\n" \
            "    auto entity = Arca::Reliquary::Do(Atmos::Entity::FindByName(name));\n" \
            "    return entity.ID();\n" \
            "}",
            { name },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == entity.ID());
            }
        }
    }

    GIVEN("script that finds entity by position")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main(int x, int y)\n" \
            "{\n" \
            "    auto entities = Arca::Reliquary::Do(Atmos::Entity::FindByPosition(Atmos::Spatial::Grid::Point(x, y)));\n" \
            "    return entities[0].ID();\n" \
            "}",
            { position.x, position.y },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == entity.ID());
            }
        }
    }

    GIVEN("moveTo")
    {
        const auto moveTo = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();

        GIVEN("script that moves entity to and returns position")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(Arca::RelicID entityID, int moveToX, int moveToY)\n" \
                "{\n" \
                "    auto entity = Atmos::Entity::Entity(entityID);\n" \
                "    Arca::Reliquary::Do(Atmos::Entity::MoveTo(entity, Atmos::Spatial::Grid::Point(moveToX, moveToY)));\n" \
                "    return Atmos::ToString(entity.Position().x) + \" \" + Atmos::ToString(entity.Position().y);\n" \
                "}",
                { entity.ID(), moveTo.x, moveTo.y },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult = ToString(moveTo.x) + " " + ToString(moveTo.y);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                }
            }
        }
    }

    GIVEN("modifyData")
    {
        GIVEN("add")
        {
            const auto names = dataGeneration.RandomGroup<String>(3);
            const auto values = dataGeneration.RandomGroup<std::int64_t>(3);

            GIVEN("script that modifies data by add")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID entityID, string name1, int64 value1, string name2, int64 value2, string name3, int64 value3)\n" \
                    "{\n" \
                    "    auto datum1 = Atmos::Datum(name1, value1);\n" \
                    "    auto datum2 = Atmos::Datum(name2, value2);\n" \
                    "    auto datum3 = Atmos::Datum(name3, value3);\n" \
                    "    Atmos::Datum[] add = { datum1, datum2, datum3 };\n" \
                    "    auto entity = Atmos::Entity::Entity(entityID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Entity::ModifyData(entity, add, string[](), Atmos::Datum[]()));\n" \
                    "\n" \
                    "    auto data = entity.Data();\n" \
                    "    string[] dataAsStrings;\n" \
                    "    for (uint i = 0; i < data.length(); ++i)\n" \
                    "    {\n" \
                    "        auto element = data[i];\n" \
                    "        dataAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                    "    }\n" \
                    "    return Atmos::Join(\", \", dataAsStrings);\n" \
                    "}",
                    { entity.ID(), names[0], values[0], names[1], values[1], names[2], values[2] },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            names[0] + " " + ToString(values[0]) + ", " +
                            names[1] + " " + ToString(values[1]) + ", " +
                            names[2] + " " + ToString(values[2]);
                        const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                        REQUIRE(result == expectedResult);
                    }
                }
            }
        }

        GIVEN("remove")
        {
            const auto names = dataGeneration.RandomGroup<String>(6);
            const auto values = dataGeneration.RandomGroup<std::int64_t>(6);

            std::vector<Datum> data;
            data.reserve(names.size());
            for(size_t i = 0; i < names.size(); ++i)
                data.push_back(Datum{ names[i], { values[i] } });

            fieldReliquary.Do(Entity::ModifyData{ entity, data, {}, {} });

            GIVEN("script that modifies data by remove")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID entityID, string name1, string name2, string name3)\n" \
                    "{\n" \
                    "    string[] remove = { name1, name2, name3 };\n" \
                    "    auto entity = Atmos::Entity::Entity(entityID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Entity::ModifyData(entity, Atmos::Datum[](), remove, Atmos::Datum[]()));\n" \
                    "\n" \
                    "    auto data = entity.Data();\n" \
                    "    string[] dataAsStrings;\n" \
                    "    for (uint i = 0; i < data.length(); ++i)\n" \
                    "    {\n" \
                    "        auto element = data[i];\n" \
                    "        dataAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                    "    }\n" \
                    "    return Atmos::Join(\", \", dataAsStrings);\n" \
                    "}",
                    { entity.ID(), names[0], names[2], names[4] },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            names[1] + " " + ToString(values[1]) + ", " +
                            names[3] + " " + ToString(values[3]) + ", " +
                            names[5] + " " + ToString(values[5]);
                        const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                        REQUIRE(result == expectedResult);
                    }
                }
            }
        }

        GIVEN("replace")
        {
            const auto names = dataGeneration.RandomGroup<String>(6);
            const auto values = dataGeneration.RandomGroup<std::int64_t>(9);

            std::vector<Datum> data;
            data.reserve(names.size());
            for (size_t i = 0; i < names.size(); ++i)
                data.push_back(Datum{ names[i], { values[i] } });

            fieldReliquary.Do(Entity::ModifyData{ entity, data, {}, {} });

            GIVEN("script that modifies data by replace")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID entityID, string name1, int64 value1, string name2, int64 value2, string name3, int64 value3)\n" \
                    "{\n" \
                    "    auto datum1 = Atmos::Datum(name1, value1);\n" \
                    "    auto datum2 = Atmos::Datum(name2, value2);\n" \
                    "    auto datum3 = Atmos::Datum(name3, value3);\n" \
                    "    Atmos::Datum[] replace = { datum1, datum2, datum3 };\n" \
                    "    auto entity = Atmos::Entity::Entity(entityID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Entity::ModifyData(entity, Atmos::Datum[](), string[](), replace));\n" \
                    "\n" \
                    "    auto data = entity.Data();\n" \
                    "    string[] dataAsStrings;\n" \
                    "    for (uint i = 0; i < data.length(); ++i)\n" \
                    "    {\n" \
                    "        auto element = data[i];\n" \
                    "        dataAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                    "    }\n" \
                    "    return Atmos::Join(\", \", dataAsStrings);\n" \
                    "}",
                    { entity.ID(), names[0], values[6], names[2], values[7], names[4], values[8] },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            names[0] + " " + ToString(values[6]) + ", " +
                            names[1] + " " + ToString(values[1]) + ", " +
                            names[2] + " " + ToString(values[7]) + ", " +
                            names[3] + " " + ToString(values[3]) + ", " +
                            names[4] + " " + ToString(values[8]) + ", " +
                            names[5] + " " + ToString(values[5]);
                        const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                        REQUIRE(result == expectedResult);
                    }
                }
            }
        }
    }

    GIVEN("modifyTags")
    {
        GIVEN("add")
        {
            const auto values = dataGeneration.RandomGroup<String>(3);

            GIVEN("script that modifies tags by add")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID entityID, string value1, string value2, string value3)\n" \
                    "{\n" \
                    "    string[] add = { value1, value2, value3 };\n" \
                    "    auto entity = Atmos::Entity::Entity(entityID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Entity::ModifyTags(entity, add, string[]()));\n" \
                    "\n" \
                    "    auto tags = entity.Tags();\n" \
                    "    return Atmos::Join(\", \", tags);\n" \
                    "}",
                    { entity.ID(), values[0], values[1], values[2] },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            ToString(values[0]) + ", " +
                            ToString(values[1]) + ", " +
                            ToString(values[2]);
                        const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                        REQUIRE(result == expectedResult);
                    }
                }
            }
        }

        GIVEN("remove")
        {
            const auto values = dataGeneration.RandomGroup<String>(6);

            fieldReliquary.Do(Entity::ModifyTags{ entity, values, {} });

            GIVEN("script that modifies tags by remove")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID entityID, string value1, string value2, string value3)\n" \
                    "{\n" \
                    "    string[] remove = { value1, value2, value3 };\n" \
                    "    auto entity = Atmos::Entity::Entity(entityID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Entity::ModifyTags(entity, string[](), remove));\n" \
                    "\n" \
                    "    auto tags = entity.Tags();\n" \
                    "    return Atmos::Join(\", \", tags);\n" \
                    "}",
                    { entity.ID(), values[0], values[2], values[4] },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            ToString(values[1]) + ", " +
                            ToString(values[3]) + ", " +
                            ToString(values[5]);
                        const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                        REQUIRE(result == expectedResult);
                    }
                }
            }
        }
    }
}