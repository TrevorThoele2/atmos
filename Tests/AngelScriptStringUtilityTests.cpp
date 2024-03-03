#include <catch.hpp>

#include "AngelScriptStringUtilityTests.h"

#include "ScriptEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>

SCENARIO_METHOD(AngelScriptStringUtilityTestsFixture, "running StringUtility AngelScript scripts", "[script][angelscript]")
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
        *engine.mockTextManager,
        *engine.scriptManager,
        *engine.mockWorldManager,
        Spatial::Size2D{
            std::numeric_limits<Spatial::Size2D::Value>::max(),
            std::numeric_limits<Spatial::Size2D::Value>::max() },
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

    GIVEN("script that converts string to bool")
    {
        auto value = dataGeneration.Random<bool>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToBool(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that converts string to int8")
    {
        auto value = dataGeneration.Random<std::int8_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "int8 main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToInt8(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int8_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that converts string to int16")
    {
        auto value = dataGeneration.Random<std::int16_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "int16 main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToInt16(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int16_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that converts string to int")
    {
        auto value = dataGeneration.Random<std::int32_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "int main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToInt(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int32_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that converts string to int64")
    {
        auto value = dataGeneration.Random<std::int64_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "int64 main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToInt64(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int64_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that converts string to uint8")
    {
        auto value = dataGeneration.Random<std::uint8_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "uint8 main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToUint8(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint8_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that converts string to uint16")
    {
        auto value = dataGeneration.Random<std::uint16_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "uint16 main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToUint16(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint16_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that converts string to uint")
    {
        auto value = dataGeneration.Random<std::uint32_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "uint main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToUint(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint32_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that converts string to uint64")
    {
        auto value = dataGeneration.Random<std::uint64_t>();

        CompileAndCreateScript(
            "basic_script.as",
            "uint64 main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToUint64(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint64_t>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that converts string to float")
    {
        auto value = dataGeneration.Random<float>();

        CompileAndCreateScript(
            "basic_script.as",
            "float main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToFloat(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) == Approx(value));
            }
        }
    }

    GIVEN("script that converts string to double")
    {
        auto value = dataGeneration.Random<double>();

        CompileAndCreateScript(
            "basic_script.as",
            "double main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToDouble(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) == Approx(value));
            }
        }
    }

    GIVEN("script that converts string to string")
    {
        auto value = dataGeneration.Random<String>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(string value)\n" \
            "{\n" \
            "    auto fromString = Atmos::ToString(value);\n" \
            "    return fromString;\n" \
            "}",
            { Scripting::Variant{ Chroma::ToString(value) } },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::string>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }
}