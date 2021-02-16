#include <catch.hpp>

#include "AngelScriptNumericLimitsTests.h"
#include <Atmos/AngelScriptRegistration.h>
#include <Atmos/AngelScriptPrimitive.h>

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>

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

    std::vector<Scripting::Finished> finishes;
    fieldReliquary.On<Scripting::Finished>([&finishes](const Scripting::Finished& signal)
        {
            finishes.push_back(signal);
        });

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
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<TestType>(std::get<Variant>(finishes[0].result)) == std::numeric_limits<TestType>::min());
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
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<TestType>(std::get<Variant>(finishes[0].result)) == std::numeric_limits<TestType>::lowest());
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
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("returns ID")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<TestType>(std::get<Variant>(finishes[0].result)) == std::numeric_limits<TestType>::max());
            }
        }
    }
}