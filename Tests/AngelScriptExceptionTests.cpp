#include <catch.hpp>

#include "AngelScriptExceptionTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/LoadScriptAssetResourceData.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Inscription/InputTextArchive.h>

SCENARIO_METHOD(AngelScriptExceptionTestsFixture, "running AngelScript scripts that throw", "[script][angelscript]")
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

    GIVEN("script that throws")
    {
        auto thrownMessage = dataGeneration.Random<String>();

        WHEN("script throws")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "void main()\n" \
                "{\n" \
                "    throw(\"" + thrownMessage + "\");\n" \
                "}",
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has logged message")
                {
                    String allLogged;
                    {
                        const auto filePath = std::filesystem::current_path() / "log.txt";
                        auto file = Inscription::InputTextFile(filePath);

                        while (!file.IsAtEndOfFile())
                            allLogged += file.ReadLine() + "\n";
                    }

                    const auto expectedMessage = String("<ERROR> ") + thrownMessage;
                    REQUIRE(Chroma::Contains(allLogged, expectedMessage));
                }
            }
        }
    }
}