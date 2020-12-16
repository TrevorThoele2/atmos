#include <catch.hpp>

#include "AngelScriptStopwatchTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>

SCENARIO_METHOD(AngelScriptStopwatchTestsFixture, "running stopwatch AngelScript scripts", "[script][angelscript][time]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    ScriptEngine engine(logger);
    engine.Setup();

    auto fieldOrigin = Arca::ReliquaryOrigin();
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

    GIVEN("real stopwatch")
    {
        GIVEN("script that returns restart")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "int64 main()\n" \
                "{\n" \
                "    auto stopwatch = Atmos::Time::CreateRealStopwatch();\n" \
                "    return stopwatch.Restart().TimeSinceEpoch().Count();\n" \
                "}",
                {},
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<long long>(std::get<Variant>(finishes[0].result)) > 0);
                }
            }
        }
    }
}