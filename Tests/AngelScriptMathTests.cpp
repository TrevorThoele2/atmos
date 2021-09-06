#include <catch.hpp>

#include "AngelScriptMathTests.h"

SCENARIO_METHOD(AngelScriptMathTestsFixture, "running math AngelScript scripts", "[script][angelscript]")
{
    GIVEN("script that returns floor with float")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "float main(float value)\n" \
            "{\n" \
            "    return Atmos::Floor(value);\n" \
            "}",
            { 1.5f },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) == 1);
            }
        }
    }

    GIVEN("script that returns floor with double")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "double main(double value)\n" \
            "{\n" \
            "    return Atmos::Floor(value);\n" \
            "}",
            { 1.5 },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) == 1);
            }
        }
    }

    GIVEN("script that returns ceiling with float")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "float main(float value)\n" \
            "{\n" \
            "    return Atmos::Ceiling(value);\n" \
            "}",
            { 1.5f },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) == 2);
            }
        }
    }

    GIVEN("script that returns ceiling with double")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "double main(double value)\n" \
            "{\n" \
            "    return Atmos::Ceiling(value);\n" \
            "}",
            { 1.5 },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) == 1);
            }
        }
    }

    GIVEN("script that returns round with float")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "float main(float value)\n" \
            "{\n" \
            "    return Atmos::Round(value);\n" \
            "}",
            { 1.5f },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) == 2);
            }
        }
    }

    GIVEN("script that returns round with double")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            "double main(double value)\n" \
            "{\n" \
            "    return Atmos::Round(value);\n" \
            "}",
            { 1.5 },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) == 2);
            }
        }
    }
}