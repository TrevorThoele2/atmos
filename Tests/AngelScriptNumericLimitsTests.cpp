#include <catch.hpp>

#include "AngelScriptNumericLimitsTests.h"
#include <Atmos/AngelScriptRegistration.h>
#include <Atmos/AngelScriptPrimitive.h>

TEMPLATE_TEST_CASE_METHOD(
    AngelScriptRelicTestsFixture,
    "running numeric limit AngelScript scripts",
    "[script][angelscript]",
    std::int8_t,
    std::int16_t,
    std::int32_t,
    std::int64_t,
    std::uint8_t,
    std::uint16_t,
    std::uint32_t,
    std::uint64_t,
    float,
    double)
{
    const auto tAngelScriptName = Scripting::Angel::CreateName(
        {
            Scripting::Angel::Registration<TestType>::ContainingNamespace()
        },
        Scripting::Angel::Registration<TestType>::Name());
    const auto limitsAngelScriptName = Scripting::Angel::Registration<std::numeric_limits<TestType>>::ContainingNamespace();

    GIVEN("script that returns min")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            tAngelScriptName + " main()\n" \
            "{\n" \
            "    return " + limitsAngelScriptName + "::Min();\n" \
            "}",
            {},
            *this->fieldReliquary);

        WHEN("working reliquary")
        {
            this->fieldReliquary->Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(this->finishes.size() == 1);
                REQUIRE(std::get<TestType>(std::get<Variant>(this->finishes[0].result)) == std::numeric_limits<TestType>::min());
            }
        }
    }

    GIVEN("script that returns lowest")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            tAngelScriptName + " main()\n" \
            "{\n" \
            "    return " + limitsAngelScriptName + "::Lowest();\n" \
            "}",
            {},
            *this->fieldReliquary);

        WHEN("working reliquary")
        {
            this->fieldReliquary->Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(this->finishes.size() == 1);
                REQUIRE(std::get<TestType>(std::get<Variant>(this->finishes[0].result)) == std::numeric_limits<TestType>::lowest());
            }
        }
    }

    GIVEN("script that returns max")
    {
        this->CompileAndCreateScript(
            "basic_script.as",
            tAngelScriptName + " main()\n" \
            "{\n" \
            "    return " + limitsAngelScriptName + "::Max();\n" \
            "}",
            {},
            *this->fieldReliquary);

        WHEN("working reliquary")
        {
            this->fieldReliquary->Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(this->finishes.size() == 1);
                REQUIRE(std::get<TestType>(std::get<Variant>(this->finishes[0].result)) == std::numeric_limits<TestType>::max());
            }
        }
    }
}