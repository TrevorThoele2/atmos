#include <catch.hpp>

#include "AngelScriptTests.h"

#include "ScriptEngine.h"

#include <Atmos/Script.h>
#include <Atmos/Work.h>
#include <Atmos/ScriptFinished.h>

SCENARIO_METHOD(AngelScriptTestsFixture, "running AngelScript scripts", "[script][angelscript]")
{
    GIVEN("empty script")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "void main()\n" \
            "{\n" \
            "}",
            *fieldReliquary);

        WHEN("working reliquary")
        {
            THEN("no error is thrown")
            {
                REQUIRE_NOTHROW(fieldReliquary->Do(Work{}));
            }

            THEN("script is destroyed")
            {
                fieldReliquary->Do(Work{});

                auto scriptCount = fieldReliquary->Batch<Scripting::Script>().Size();
                
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
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<int>(std::get<Variant>(finishes[0].result)) == 123);
            }
        }
    }

    GIVEN("script that dereferences null")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "class MyClass\n" \
            "{\n" \
            "    void Do() {}\n" \
            "}\n" \
            "\n" \
            "void main()\n" \
            "{\n" \
            "    MyClass@ myClass = null;\n" \
            "    myClass.Do();\n" \
            "}",
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has failed")
            {
                REQUIRE(finishes.size() == 1);

                const auto failure = std::get<Scripting::Failure>(finishes[0].result);
                REQUIRE(failure.function == "main");
                REQUIRE(failure.message == "Null pointer access");
            }
        }
    }
}