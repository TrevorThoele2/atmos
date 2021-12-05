#include <catch.hpp>

#include "JavaScriptParameterTests.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>

void JavaScriptParameterTestsFixture::CompileAndCreateScript(const Scripting::Variant& value)
{
    JavaScriptFixture::CompileAndCreateScript(
        "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (value: any): Atmos.Result => {
    return {
        done: true,
        value
    };
};)V0G0N",
        { value },
        *fieldReliquary);
}

SCENARIO_METHOD(JavaScriptParameterTestsFixture, "running JavaScript scripts with parameters", "[script][javascript]")
{
    GIVEN("script that accepts and returns bool")
    {
        auto value = dataGeneration.Random<bool>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns bool")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns int8")
    {
        auto value = dataGeneration.Random<std::int8_t>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns int32")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int32_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns int16")
    {
        auto value = dataGeneration.Random<std::int16_t>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns int32")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int32_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns int")
    {
        auto value = dataGeneration.Random<std::int32_t>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns int32")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int32_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns int64")
    {
        auto value = dataGeneration.Random<std::int64_t>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns double")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int64_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns uint8")
    {
        auto value = dataGeneration.Random<std::uint8_t>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns uint8")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int32_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns uint16")
    {
        auto value = dataGeneration.Random<std::uint16_t>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns uint16")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int32_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns uint")
    {
        auto value = dataGeneration.Random<std::uint32_t>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns uint32")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int64_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns uint64")
    {
        auto value = dataGeneration.Random<std::uint64_t>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns uint64")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int64_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns float")
    {
        auto value = dataGeneration.Random<float>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns float")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns double")
    {
        auto value = dataGeneration.Random<double>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns double")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that accepts and returns string")
    {
        auto value = dataGeneration.Random<String>();

        CompileAndCreateScript(value);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script returns string")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::string>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }
}