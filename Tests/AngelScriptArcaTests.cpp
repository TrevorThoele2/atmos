#include <catch.hpp>

#include "AngelScriptArcaTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>

#include "Atmos/FileLoggingSink.h"

SCENARIO_METHOD(AngelScriptArcaTestsFixture, "running Arca AngelScript scripts", "[script][angelscript]")
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

    GIVEN("script that returns Arca::nullRelicID")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main()\n" \
            "{\n" \
            "    return Arca::nullRelicID;\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == Arca::nullRelicID);
            }
        }
    }
}