#include <catch.hpp>

#include "AngelScriptTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/Work.h>
#include <Atmos/ScriptFinished.h>

SCENARIO_METHOD(AngelScriptTestsFixture, "running AngelScript scripts", "[script][angelscript]")
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

    GIVEN("empty script")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "void main()\n" \
            "{\n" \
            "}",
            fieldReliquary);

        WHEN("working reliquary")
        {
            THEN("no error is thrown")
            {
                REQUIRE_NOTHROW(fieldReliquary.Do(Work{}));
            }

            THEN("script is destroyed")
            {
                fieldReliquary.Do(Work{});

                auto scriptCount = fieldReliquary.Batch<Scripting::Script>().Size();
                
                REQUIRE(scriptCount == 0);
            }
        }
    }

    GIVEN("script that runs function from shared module")
    {
        CompileAndCreateScriptWithSharedModules(
            "basic_script.as",
            "int main()\n" \
            "{\n" \
            "    return DoThing();\n" \
            "}",
            std::vector<Scripting::Module>{
                {
                    "shared_script.as",
                    "int DoThing()\n" \
                    "{\n" \
                    "    return 123;\n" \
                    "}" \
                }
            },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<int>(std::get<Variant>(finishes[0].result)) == 123);
            }
        }
    }
}