#include <catch.hpp>

#include "JavaScriptLoggingTests.h"

#include <Inscription/Plaintext.h>

String JavaScriptLoggingTestsFixture::AllLogged() const
{
    String allLogged;
    const auto filePath = std::filesystem::current_path() / "log.txt";
    auto file = Inscription::File::InputText(filePath);
    auto archive = Inscription::Archive::InputText(file);
    auto format = Inscription::Format::InputPlaintext(archive);
    format.ReadSize(allLogged);
    return allLogged;
}

SCENARIO_METHOD(JavaScriptLoggingTestsFixture, "running logging JavaScript scripts", "[script][javascript][logging]")
{
    GIVEN("script that logs")
    {
        auto loggedMessage = dataGeneration.Random<String>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (loggedMessage: string): Atmos.Result => {
    Atmos.Reliquary.send(
        Atmos.Traits.Logging.Log.typeName,
        {
            message: loggedMessage,
            severity: Atmos.Logging.Severity.Error,
            details: [{ name: "test", value: "other" }]
        });
    return {
        done: true
    };
};)V0G0N",
            { loggedMessage },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                String allLogged;
                {
                    const auto filePath = std::filesystem::current_path() / "log.txt";
                    auto file = Inscription::File::InputText(filePath);
                    auto archive = Inscription::Archive::InputText(file);
                    auto format = Inscription::Format::InputPlaintext(archive);
                    format.ReadSize(allLogged);
                }

                const auto expectedMessage = String("<ERROR> ") + loggedMessage;
                REQUIRE(Chroma::Contains(allLogged, expectedMessage));
                REQUIRE(Chroma::Contains(allLogged, "test: other"));
            }
        }
    }
}