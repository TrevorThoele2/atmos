#include <catch.hpp>

#include "AngelScriptEntityTests.h"

#include "ScriptEngine.h"

#include <Atmos/AngelScriptEntity.h>
#include <Atmos/AngelScriptModifyProperties.h>

#include <Atmos/SpatialAlgorithms.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>
#include <Atmos/EntityPrototype.h>
#include <Atmos/ActualizeAllEntityPrototypes.h>
#include <Arca/LocalRelic.h>
#include <Atmos/DataCore.h>

SCENARIO_METHOD(AngelScriptEntityTestsFixture, "running entity AngelScript scripts", "[script][angelscript]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    logger.Add<Logging::FileSink>();
    ScriptEngine engine(logger);

    auto fieldOrigin = Arca::ReliquaryOrigin();
    fieldOrigin.Register<Arca::OpenRelic>();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockAssetResourceManager,
        *engine.mockAudioManager,
        *engine.mockInputManager,
        *engine.mockGraphicsManager,
        *engine.scriptManager,
        *engine.mockWorldManager,
        Spatial::ScreenSize{
            std::numeric_limits<Spatial::ScreenSize::Dimension>::max(),
            std::numeric_limits<Spatial::ScreenSize::Dimension>::max() },
            *engine.mockWindow,
            engine.Logger());
    fieldOrigin.CuratorCommandPipeline<Work>(Arca::Pipeline{ Scripting::Stage() });
    World::Field field(0, fieldOrigin.Actualize());

    auto& fieldReliquary = field.Reliquary();

    std::vector<Scripting::Finished> finishes;
    fieldReliquary.On<Scripting::Finished>([&finishes](const Scripting::Finished& signal)
        {
            finishes.push_back(signal);
        });

    GIVEN("created entity")
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto position = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        const auto isSolid = dataGeneration.Random<bool>();

        auto entity = fieldReliquary.Do(Arca::Create<Entity::Entity>{ name, position, isSolid });

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

        GIVEN("ModifyProperties")
        {
            GIVEN("add")
            {
                const auto names = dataGeneration.RandomGroup<String>(3);
                const auto values = dataGeneration.RandomGroup<std::int64_t>(3);

                GIVEN("script that modifies properties by add")
                {
                    CompileAndCreateScript(
                        "basic_script.as",
                        "string main(Arca::RelicID entityID, string name1, int64 value1, string name2, int64 value2, string name3, int64 value3)\n" \
                        "{\n" \
                        "    auto property1 = Atmos::Property(name1, value1);\n" \
                        "    auto property2 = Atmos::Property(name2, value2);\n" \
                        "    auto property3 = Atmos::Property(name3, value3);\n" \
                        "    Atmos::Property[] add = { property1, property2, property3 };\n" \
                        "    auto entity = Atmos::Entity::Entity(entityID);\n" \
                        "    Arca::Reliquary::Do(Atmos::ModifyProperties(entity.ID(), add, string[](), Atmos::Property[]()));\n" \
                        "\n" \
                        "    auto properties = Atmos::DataCore(entity.ID()).Properties();\n" \
                        "    string[] propertiesAsStrings;\n" \
                        "    for (uint i = 0; i < properties.length(); ++i)\n" \
                        "    {\n" \
                        "        auto element = properties[i];\n" \
                        "        propertiesAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                        "    }\n" \
                        "    return Atmos::Join(\", \", propertiesAsStrings);\n" \
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

                std::vector<Property> properties;
                properties.reserve(names.size());
                for (size_t i = 0; i < names.size(); ++i)
                    properties.push_back(Property{ names[i], { values[i] } });

                fieldReliquary.Do(ModifyProperties{ entity.ID(), properties, {}, {} });

                GIVEN("script that modifies properties by remove")
                {
                    CompileAndCreateScript(
                        "basic_script.as",
                        "string main(Arca::RelicID entityID, string name1, string name2, string name3)\n" \
                        "{\n" \
                        "    string[] remove = { name1, name2, name3 };\n" \
                        "    auto entity = Atmos::Entity::Entity(entityID);\n" \
                        "    Arca::Reliquary::Do(Atmos::ModifyProperties(entity.ID(), Atmos::Property[](), remove, Atmos::Property[]()));\n" \
                        "\n" \
                        "    auto properties = Atmos::DataCore(entity.ID()).Properties();\n" \
                        "    string[] propertiesAsStrings;\n" \
                        "    for (uint i = 0; i < properties.length(); ++i)\n" \
                        "    {\n" \
                        "        auto element = properties[i];\n" \
                        "        propertiesAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                        "    }\n" \
                        "    return Atmos::Join(\", \", propertiesAsStrings);\n" \
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

                std::vector<Property> properties;
                properties.reserve(names.size());
                for (size_t i = 0; i < names.size(); ++i)
                    properties.push_back(Property{ names[i], { values[i] } });

                fieldReliquary.Do(ModifyProperties{ entity.ID(), properties, {}, {} });

                GIVEN("script that modifies properties by replace")
                {
                    CompileAndCreateScript(
                        "basic_script.as",
                        "string main(Arca::RelicID entityID, string name1, int64 value1, string name2, int64 value2, string name3, int64 value3)\n" \
                        "{\n" \
                        "    auto property1 = Atmos::Property(name1, value1);\n" \
                        "    auto property2 = Atmos::Property(name2, value2);\n" \
                        "    auto property3 = Atmos::Property(name3, value3);\n" \
                        "    Atmos::Property[] replace = { property1, property2, property3 };\n" \
                        "    auto entity = Atmos::Entity::Entity(entityID);\n" \
                        "    Arca::Reliquary::Do(Atmos::ModifyProperties(entity.ID(), Atmos::Property[](), string[](), replace));\n" \
                        "\n" \
                        "    auto properties = Atmos::DataCore(entity.ID()).Properties();\n" \
                        "    string[] propertiesAsStrings;\n" \
                        "    for (uint i = 0; i < properties.length(); ++i)\n" \
                        "    {\n" \
                        "        auto element = properties[i];\n" \
                        "        propertiesAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                        "    }\n" \
                        "    return Atmos::Join(\", \", propertiesAsStrings);\n" \
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

        GIVEN("entity prototype")
        {
            const auto names = dataGeneration.RandomGroup<String>(3);
            const auto values = dataGeneration.RandomGroup<std::int64_t>(3);

            GIVEN("script that modifies properties by add")
            {
                auto script = CompileAndCreateScript(
                    "basic_script.as",
                    "void main(string name1, int64 value1, string name2, int64 value2, string name3, int64 value3)\n" \
                    "{\n" \
                    "    auto property1 = Atmos::Property(name1, value1);\n" \
                    "    auto property2 = Atmos::Property(name2, value2);\n" \
                    "    auto property3 = Atmos::Property(name3, value3);\n" \
                    "    Atmos::Property[] add = { property1, property2, property3 };\n" \
                    "    auto entity = Atmos::CurrentEntity();\n" \
                    "    Arca::Reliquary::Do(Atmos::ModifyProperties(entity.ID(), add, string[](), Atmos::Property[]()));\n" \
                    "}",
                    { names[0], values[0], names[1], values[1], names[2], values[2] },
                    fieldReliquary);

                fieldReliquary.Do(
                    Arca::Create<Entity::Prototype>(script, dataGeneration.Random<String>(), position));

                WHEN("actualizing all prototypes")
                {
                    fieldReliquary.Do(Entity::ActualizeAllPrototypes{});

                    THEN("entity has been created with the correct properties")
                    {
                        auto entities = fieldReliquary.Batch<Entity::Entity>();
                        REQUIRE(entities.Size() == 2);

                        auto entityFromPrototype = ++entities.begin();
                        auto& properties = Arca::Index<DataCore>(entityFromPrototype.ID(), fieldReliquary)->properties;
                        REQUIRE(properties.size() == 3);
                        REQUIRE(properties[0] == Atmos::Property{ names[0], values[0] });
                        REQUIRE(properties[1] == Atmos::Property{ names[1], values[1] });
                        REQUIRE(properties[2] == Atmos::Property{ names[2], values[2] });
                    }
                }
            }
        }
    }

    GIVEN("script that creates entity and returns relic ID")
    {
        const auto name = dataGeneration.Random<std::string>();
        const auto position = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        const auto isSolid = dataGeneration.Random<bool>();

        CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main(string name, int x, int y, bool isSolid)\n" \
            "{\n" \
            "    auto entity = Arca::Reliquary::Do(\n" \
            "        Arca::Create<Atmos::Entity::Entity>(name, Atmos::Spatial::Grid::Point(x, y), isSolid));\n" \
            "    return entity.ID();\n" \
            "}",
            { name, position.x, position.y, isSolid },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) != Arca::nullRelicID);
            }
        }
    }
}