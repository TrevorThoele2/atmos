#include <catch.hpp>

#include "AngelScriptDurationTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>

SCENARIO_METHOD(AngelScriptDurationTestsFixture, "running duration AngelScript scripts", "[script][angelscript][time]")
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
    
    GIVEN("script that returns equals for same duration")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "bool main()\n" \
            "{\n" \
            "    auto duration = Atmos::Time::Nanoseconds(500);\n" \
            "    return duration == duration;\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == true);
            }
        }
    }

    GIVEN("script that returns not equals for same duration")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "bool main()\n" \
            "{\n" \
            "    auto duration = Atmos::Time::Nanoseconds(500);\n" \
            "    return duration != duration;\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == false);
            }
        }
    }
}