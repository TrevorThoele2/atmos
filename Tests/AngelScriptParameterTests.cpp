#include <catch.hpp>

#include "AngelScriptParameterTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>

SCENARIO_METHOD(AngelScriptParameterTestsFixture, "running AngelScript scripts with parameters", "[script][angelscript]")
{
    GIVEN("script that returns bool")
    {
        auto value = dataGeneration.Random<bool>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(bool value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns int8")
    {
        auto value = dataGeneration.Random<std::int8_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "int8 main(int8 value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int8_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns int16")
    {
        auto value = dataGeneration.Random<std::int16_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "int16 main(int16 value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int16_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns int")
    {
        auto value = dataGeneration.Random<std::int32_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "int main(int value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int32_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns int64")
    {
        auto value = dataGeneration.Random<std::int64_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "int64 main(int64 value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int64_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns uint8")
    {
        auto value = dataGeneration.Random<std::uint8_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "uint8 main(uint8 value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint8_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns uint16")
    {
        auto value = dataGeneration.Random<std::uint16_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "uint16 main(uint16 value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint16_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns uint")
    {
        auto value = dataGeneration.Random<std::uint32_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "uint main(uint value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint32_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns uint64")
    {
        auto value = dataGeneration.Random<std::uint64_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "uint64 main(uint64 value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint64_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns float")
    {
        auto value = dataGeneration.Random<float>();

        CompileAndCreateScript(
            "basic_script.as",
            "float main(float value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns double")
    {
        auto value = dataGeneration.Random<double>();

        CompileAndCreateScript(
            "basic_script.as",
            "double main(double value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that returns string")
    {
        auto value = dataGeneration.Random<String>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(string value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { Scripting::Variant{ value } },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::string>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }
}