#include <catch.hpp>

#include "AngelScriptGlobalIndexTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>

TEMPLATE_TEST_CASE_METHOD(
    AngelScriptGlobalTestsFixture,
    "running global index AngelScript scripts",
    "[script][angelscript][global][index]",
    Render::Camera)
{
    Logging::Logger logger(Logging::Severity::Verbose);
    logger.Add<Logging::FileSink>();
    ScriptEngine engine(logger);

    auto fieldOrigin = Arca::ReliquaryOrigin();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockAssetResourceManager,
        *engine.mockAudioManager,
        *engine.mockInputManager,
        *engine.mockGraphicsManager,
        *engine.mockTextManager,
        *engine.scriptManager,
        *engine.mockWorldManager,
        Spatial::Size2D{
            std::numeric_limits<Spatial::Size2D::Value>::max(),
            std::numeric_limits<Spatial::Size2D::Value>::max() },
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

    GIVEN("type")
    {
        auto [index, angelScriptName] = this->template CreateObject<TestType>(fieldReliquary);

        GIVEN("script that returns id")
        {
            auto script = this->CompileAndCreateScript(
                "basic_script.as",
                "Arca::RelicID main()\n" \
                "{\n" \
                "    auto index = " + angelScriptName + "();\n" \
                "    return index.ID();\n" \
                "}",
                {},
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
            auto script = this->CompileAndCreateScript(
                "basic_script.as",
                "bool main()\n" \
                "{\n" \
                "    auto index1 = " + angelScriptName + "();\n" \
                "    auto index2 = " + angelScriptName + "();\n" \
                "    return index1 == index2;\n" \
                "}",
                {},
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

        GIVEN("script that returns implicit cast to handle")
        {
            auto script = this->CompileAndCreateScript(
                "basic_script.as",
                "string main()\n" \
                "{\n" \
                "    auto index = " + angelScriptName + "();\n" \
                "    auto handle = Arca::Handle(index);\n"\
                "    return Atmos::ToString(handle.ID()) +\n" \
                "        \" \" +\n" \
                "        handle.Type().name +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(handle.Type().isConst) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(handle.ObjectType());\n" \
                "}",
                {},
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
            auto script = this->CompileAndCreateScript(
                "basic_script.as",
                "bool main()\n" \
                "{\n" \
                "    auto index = " + angelScriptName + "();\n" \
                "    return bool(index);\n" \
                "}",
                {},
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

        GIVEN("script that returns traits type name")
        {
            auto script = this->CompileAndCreateScript(
                "basic_script.as",
                "string main()\n" \
                "{\n" \
                "    return Arca::Traits<" + angelScriptName + ">().Type().name;\n" \
                "}",
                {},
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("returns false")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == Arca::TypeFor<TestType>().name);
                }
            }
        }

        GIVEN("script that returns traits type isConst")
        {
            auto script = this->CompileAndCreateScript(
                "basic_script.as",
                "bool main()\n" \
                "{\n" \
                "    return Arca::Traits<" + angelScriptName + ">().Type().isConst;\n" \
                "}",
                {},
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("returns false")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == Arca::TypeFor<TestType>().isConst);
                }
            }
        }
    }
}