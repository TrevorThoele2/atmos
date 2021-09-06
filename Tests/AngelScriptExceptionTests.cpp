#include <catch.hpp>

#include "AngelScriptExceptionTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Work.h>
#include <Inscription/InputTextArchive.h>

SCENARIO_METHOD(AngelScriptExceptionTestsFixture, "running AngelScript scripts that throw", "[script][angelscript]")
{
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
                *fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary->Do(Work{});

                THEN("has logged message")
                {
                    String allLogged;
                    {
                        const auto filePath = std::filesystem::current_path() / "log.txt";
                        auto file = Inscription::File::InputText(filePath);

                        while (!file.IsAtEnd())
                            allLogged += file.ReadLine() + "\n";
                    }

                    const auto expectedMessage = String("<ERROR> ") + thrownMessage;
                    REQUIRE(Chroma::Contains(allLogged, expectedMessage));
                }
            }
        }
    }
}