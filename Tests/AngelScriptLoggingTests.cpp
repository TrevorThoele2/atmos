#include <catch.hpp>

#include "AngelScriptLoggingTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/LoadScriptAssetResourceData.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Inscription/InputTextArchive.h>

String AngelScriptLoggingTestsFixture::AllLogged() const
{
    String allLogged;
    {
        const auto filePath = std::filesystem::current_path() / "log.txt";
        auto file = Inscription::File::InputText(filePath);

        while (!file.IsAtEnd())
            allLogged += file.ReadLine() + "\n";
    }
    return allLogged;
}

SCENARIO_METHOD(AngelScriptLoggingTestsFixture, "running logging AngelScript scripts", "[script][angelscript][logging]")
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

    GIVEN("script that logs")
    {
        auto loggedMessage = dataGeneration.Random<String>();

        WHEN("script embedded with log")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "void main()\n" \
                "{\n" \
                "    auto loggedMessage = \"" + loggedMessage + "\";" \
                "    Atmos::Logging::Log log(loggedMessage, Atmos::Logging::Severity::Error);\n" \
                "    Arca::Reliquary::Do(log);\n" \
                "}",
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("script has executed")
                {
                    String allLogged;
                    {
                        const auto filePath = std::filesystem::current_path() / "log.txt";
                        auto file = Inscription::File::InputText(filePath);

                        while (!file.IsAtEnd())
                            allLogged += file.ReadLine() + "\n";
                    }

                    const auto expectedMessage = String("<ERROR> ") + loggedMessage;
                    REQUIRE(Chroma::Contains(allLogged, expectedMessage));
                }
            }
        }
    }
}