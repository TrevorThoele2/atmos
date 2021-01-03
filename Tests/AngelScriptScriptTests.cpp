#include <catch.hpp>

#include "AngelScriptScriptTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/ModifyScriptData.h>

#include <Atmos/StringUtility.h>

SCENARIO_METHOD(AngelScriptScriptTestsFixture, "running script AngelScript scripts", "[script][angelscript]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    ScriptEngine engine(logger);
    engine.Setup();

    auto fieldOrigin = Arca::ReliquaryOrigin();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockImageAssetManager,
        *engine.nullAudioManager,
        *engine.mockInputManager,
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

    const auto assetName = "script_asset.as";
    const auto asset = CompileAndCreateScriptAsset(
        assetName,
        "void main()\n" \
        "{\n" \
        "    Arca::Reliquary::Do(Atmos::Scripting::Suspend(Atmos::CurrentScript()));\n" \
        "}",
        fieldReliquary);

    const auto parameters = Scripting::Parameters{};

    auto index = fieldReliquary.Do(Arca::Create<Scripting::Script>{ asset, "main", parameters });

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
                    "string main(Arca::RelicID scriptID, string name1, int64 value1, string name2, int64 value2, string name3, int64 value3)\n" \
                    "{\n" \
                    "    auto datum1 = Atmos::Datum(name1, value1);\n" \
                    "    auto datum2 = Atmos::Datum(name2, value2);\n" \
                    "    auto datum3 = Atmos::Datum(name3, value3);\n" \
                    "    Atmos::Datum[] add = { datum1, datum2, datum3 };\n" \
                    "    auto script = Atmos::Scripting::Script(scriptID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Scripting::ModifyData(script, add, string[](), Atmos::Datum[]()));\n" \
                    "\n" \
                    "    auto data = script.Data();\n" \
                    "    string[] dataAsStrings;\n" \
                    "    for (uint i = 0; i < data.length(); ++i)\n" \
                    "    {\n" \
                    "        auto element = data[i];\n" \
                    "        dataAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                    "    }\n" \
                    "    return Atmos::Join(\", \", dataAsStrings);\n" \
                    "}",
                    { index.ID(), names[0], values[0], names[1], values[1], names[2], values[2] },
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
            for (size_t i = 0; i < names.size(); ++i)
                data.push_back(Datum{ names[i], { values[i] } });

            fieldReliquary.Do(Scripting::ModifyData{ index, data, {}, {} });

            GIVEN("script that modifies data by remove")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID scriptID, string name1, string name2, string name3)\n" \
                    "{\n" \
                    "    string[] remove = { name1, name2, name3 };\n" \
                    "    auto script = Atmos::Scripting::Script(scriptID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Scripting::ModifyData(script, Atmos::Datum[](), remove, Atmos::Datum[]()));\n" \
                    "\n" \
                    "    auto data = script.Data();\n" \
                    "    string[] dataAsStrings;\n" \
                    "    for (uint i = 0; i < data.length(); ++i)\n" \
                    "    {\n" \
                    "        auto element = data[i];\n" \
                    "        dataAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                    "    }\n" \
                    "    return Atmos::Join(\", \", dataAsStrings);\n" \
                    "}",
                    { index.ID(), names[0], names[2], names[4] },
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

            fieldReliquary.Do(Scripting::ModifyData{ index, data, {}, {} });

            GIVEN("script that modifies data by replace")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID scriptID, string name1, int64 value1, string name2, int64 value2, string name3, int64 value3)\n" \
                    "{\n" \
                    "    auto datum1 = Atmos::Datum(name1, value1);\n" \
                    "    auto datum2 = Atmos::Datum(name2, value2);\n" \
                    "    auto datum3 = Atmos::Datum(name3, value3);\n" \
                    "    Atmos::Datum[] replace = { datum1, datum2, datum3 };\n" \
                    "    auto script = Atmos::Scripting::Script(scriptID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Scripting::ModifyData(script, Atmos::Datum[](), string[](), replace));\n" \
                    "\n" \
                    "    auto data = script.Data();\n" \
                    "    string[] dataAsStrings;\n" \
                    "    for (uint i = 0; i < data.length(); ++i)\n" \
                    "    {\n" \
                    "        auto element = data[i];\n" \
                    "        dataAsStrings.insertLast(element.name + \" \" + Atmos::ToString(element.value.AsInt()));\n" \
                    "    }\n" \
                    "    return Atmos::Join(\", \", dataAsStrings);\n" \
                    "}",
                    { index.ID(), names[0], values[6], names[2], values[7], names[4], values[8] },
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

    GIVEN("script that returns id")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main(Arca::RelicID id)\n" \
            "{\n" \
            "    auto index = Atmos::Scripting::Script(id);\n" \
            "    return index.ID();\n" \
            "}",
            { index.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == index.ID());
            }
        }
    }

    GIVEN("script that returns equality from two indices")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "bool main(Arca::RelicID id)\n" \
            "{\n" \
            "    auto index1 = Atmos::Scripting::Script(id);\n" \
            "    auto index2 = Atmos::Scripting::Script(id);\n" \
            "    return index1 == index2;\n" \
            "}",
            { index.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns true")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == true);
            }
        }
    }

    GIVEN("script that returns not equality with default")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "bool main(Arca::RelicID id)\n" \
            "{\n" \
            "    auto index1 = Atmos::Scripting::Script(id);\n" \
            "    auto index2 = Atmos::Scripting::Script();\n" \
            "    return index1 == index2;\n" \
            "}",
            { index.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns false")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == false);
            }
        }
    }

    GIVEN("script that returns implicit cast to handle")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "string main(Arca::RelicID id)\n" \
            "{\n" \
            "    auto index = Atmos::Scripting::Script(id);\n" \
            "    auto handle = Arca::Handle(index);\n"\
            "    return Atmos::ToString(handle.ID()) +\n" \
            "        \" \" +\n" \
            "        handle.Type().name +\n" \
            "        \" \" +\n" \
            "        Atmos::ToString(handle.Type().isConst) +\n" \
            "        \" \" +\n" \
            "        Atmos::ToString(handle.ObjectType());\n" \
            "}",
            { index.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns correct string")
            {
                REQUIRE(finishes.size() == 1);

                Arca::Handle handle = index;
                const auto expectedResult =
                    ToString(handle.ID()) +
                    " " +
                    handle.Type().name +
                    " " +
                    ToString(handle.Type().isConst) +
                    " " +
                    ToString(static_cast<std::underlying_type_t<Arca::HandleObjectType>>(handle.ObjectType()));

                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
            }
        }
    }

    GIVEN("script that returns implicit cast to boolean with occupied")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "bool main(Arca::RelicID id)\n" \
            "{\n" \
            "    auto index = Atmos::Scripting::Script(id);\n" \
            "    return bool(index);\n" \
            "}",
            { index.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns true")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == true);
            }
        }
    }

    GIVEN("script that returns implicit cast to boolean with not occupied")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "bool main()\n" \
            "{\n" \
            "    auto index = Atmos::Scripting::Script();\n" \
            "    return bool(index);\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns false")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == false);
            }
        }
    }

    GIVEN("script that returns traits type name")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "string main()\n" \
            "{\n" \
            "    return Arca::Traits<Atmos::Scripting::Script>().Type().name;\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns false")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == Arca::TypeFor<Scripting::Script>().name);
            }
        }
    }

    GIVEN("script that returns traits type isConst")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "bool main()\n" \
            "{\n" \
            "    return Arca::Traits<Atmos::Scripting::Script>().Type().isConst;\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns false")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == Arca::TypeFor<Scripting::Script>().isConst);
            }
        }
    }

    GIVEN("script that returns ID from batch")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main(Arca::RelicID id)\n" \
            "{\n" \
            "    auto values = Arca::Batch<Atmos::Scripting::Script>().Values();\n" \
            "    for(uint i = 0; i < values.length(); ++i)\n" \
            "        if (values[i].ID() == id)\n" \
            "            return values[i].ID();\n" \
            "    return 0;\n" \
            "}",
            { index.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns false")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == index.ID());
            }
        }
    }
}