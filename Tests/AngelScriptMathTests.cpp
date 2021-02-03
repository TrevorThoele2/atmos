#include <catch.hpp>

#include "AngelScriptMathTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/ModifyProperties.h>

#include <Atmos/StringUtility.h>

SCENARIO_METHOD(AngelScriptMathTestsFixture, "running math AngelScript scripts", "[script][angelscript]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    logger.Add<Logging::FileSink>();
    ScriptEngine engine(logger);

    auto fieldOrigin = Arca::ReliquaryOrigin();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockImageAssetManager,
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

    GIVEN("script that returns floor with float")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "float main(float value)\n" \
            "{\n" \
            "    return Atmos::Floor(value);\n" \
            "}",
            { 1.5f },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) == 1);
            }
        }
    }

    GIVEN("script that returns floor with double")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "double main(double value)\n" \
            "{\n" \
            "    return Atmos::Floor(value);\n" \
            "}",
            { 1.5 },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) == 1);
            }
        }
    }

    GIVEN("script that returns ceiling with float")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "float main(float value)\n" \
            "{\n" \
            "    return Atmos::Ceiling(value);\n" \
            "}",
            { 1.5f },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) == 2);
            }
        }
    }

    GIVEN("script that returns ceiling with double")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "double main(double value)\n" \
            "{\n" \
            "    return Atmos::Ceiling(value);\n" \
            "}",
            { 1.5 },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) == 1);
            }
        }
    }

    GIVEN("script that returns round with float")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "float main(float value)\n" \
            "{\n" \
            "    return Atmos::Round(value);\n" \
            "}",
            { 1.5f },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) == 2);
            }
        }
    }

    GIVEN("script that returns round with double")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "double main(double value)\n" \
            "{\n" \
            "    return Atmos::Round(value);\n" \
            "}",
            { 1.5 },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) == 2);
            }
        }
    }
}