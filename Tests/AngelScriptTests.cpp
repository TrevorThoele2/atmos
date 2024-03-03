#include <catch.hpp>

#include "AngelScriptTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/LoadScriptAssetResourceData.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Inscription/InputTextArchive.h>

SCENARIO_METHOD(AngelScriptTestsFixture, "running AngelScript scripts", "[script][angelscript]")
{
    ScriptEngine engine;
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
}