#include <catch.hpp>

#include "AngelScriptDiagnosticsTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Arca/OpenRelic.h>

SCENARIO_METHOD(AngelScriptDiagnosticsTestsFixture, "running diagnostics AngelScript scripts", "[script][angelscript]")
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

    GIVEN("DiagnosticsStatistics")
    {
        GIVEN("script that returns RelicCount")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "uint32 main()\n" \
                "{\n" \
                "    Atmos::Diagnostics::Statistics statistics;\n" \
                "    return statistics.RelicCount();\n" \
                "}",
                {},
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto result = std::get<size_t>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == 0);
                }
            }
        }

        GIVEN("script that returns ShardCount")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "uint32 main()\n" \
                "{\n" \
                "    Atmos::Diagnostics::Statistics statistics;\n" \
                "    return statistics.ShardCount();\n" \
                "}",
                {},
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto result = std::get<size_t>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == 0);
                }
            }
        }
    }
}