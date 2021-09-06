#include <catch.hpp>

#include "AngelScriptWorldTests.h"

#include <Atmos/ModifyWorldProperties.h>

SCENARIO_METHOD(AngelScriptWorldTestsFixture, "running world AngelScript scripts", "[script][angelscript][world]")
{
    GIVEN("script that requests field")
    {
        const auto nextFieldID = dataGeneration.Random<World::FieldID>();

        CompileAndCreateScript(
            "basic_script.as",
            "void main(uint fieldID)\n" \
            "{\n" \
            "    Arca::Reliquary::Do(Atmos::World::RequestField(fieldID));\n" \
            "}",
            { nextFieldID },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has requested field")
            {
                REQUIRE(finishes.size() == 1);

                REQUIRE(engine.mockWorldManager->requestCalls.size() == 1);
                REQUIRE(engine.mockWorldManager->requestCalls[0] == nextFieldID);
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
                    "string main(string name1, int64 value1, string name2, int64 value2, string name3, int64 value3)\n" \
                    "{\n" \
                    "    auto property1 = Atmos::Property(name1, value1);\n" \
                    "    auto property2 = Atmos::Property(name2, value2);\n" \
                    "    auto property3 = Atmos::Property(name3, value3);\n" \
                    "    Atmos::Property[] add = { property1, property2, property3 };\n" \
                    "    Arca::Reliquary::Do(Atmos::World::ModifyProperties(add, string[](), Atmos::Property[]()));\n" \
                    "\n" \
                    "    auto properties = Arca::Reliquary::Do(Atmos::World::RetrieveProperties());\n" \
                    "    string[] propertiesAsStrings;\n" \
                    "    for (uint i = 0; i < properties.length(); ++i)\n" \
                    "    {\n" \
                    "        auto element = properties[i];\n" \
                    "        propertiesAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                    "    }\n" \
                    "    return Atmos::Join(\", \", propertiesAsStrings);\n" \
                    "}",
                    { names[0], values[0], names[1], values[1], names[2], values[2] },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

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

            fieldReliquary->Do(World::ModifyProperties{ properties, {}, {} });

            GIVEN("script that modifies properties by remove")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(string name1, string name2, string name3)\n" \
                    "{\n" \
                    "    string[] remove = { name1, name2, name3 };\n" \
                    "    Arca::Reliquary::Do(Atmos::World::ModifyProperties(Atmos::Property[](), remove, Atmos::Property[]()));\n" \
                    "\n" \
                    "    auto properties = Arca::Reliquary::Do(Atmos::World::RetrieveProperties());\n" \
                    "    string[] propertiesAsStrings;\n" \
                    "    for (uint i = 0; i < properties.length(); ++i)\n" \
                    "    {\n" \
                    "        auto element = properties[i];\n" \
                    "        propertiesAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                    "    }\n" \
                    "    return Atmos::Join(\", \", propertiesAsStrings);\n" \
                    "}",
                    { names[0], names[2], names[4] },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

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

            fieldReliquary->Do(World::ModifyProperties{ properties, {}, {} });

            GIVEN("script that modifies properties by replace")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(string name1, int64 value1, string name2, int64 value2, string name3, int64 value3)\n" \
                    "{\n" \
                    "    auto property1 = Atmos::Property(name1, value1);\n" \
                    "    auto property2 = Atmos::Property(name2, value2);\n" \
                    "    auto property3 = Atmos::Property(name3, value3);\n" \
                    "    Atmos::Property[] replace = { property1, property2, property3 };\n" \
                    "    Arca::Reliquary::Do(Atmos::World::ModifyProperties(Atmos::Property[](), string[](), replace));\n" \
                    "\n" \
                    "    auto properties = Arca::Reliquary::Do(Atmos::World::RetrieveProperties());\n" \
                    "    string[] propertiesAsStrings;\n" \
                    "    for (uint i = 0; i < properties.length(); ++i)\n" \
                    "    {\n" \
                    "        auto element = properties[i];\n" \
                    "        propertiesAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                    "    }\n" \
                    "    return Atmos::Join(\", \", propertiesAsStrings);\n" \
                    "}",
                    { names[0], values[6], names[2], values[7], names[4], values[8] },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

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
}