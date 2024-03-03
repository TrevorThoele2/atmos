#include <catch.hpp>

#include "AngelScriptRandomTests.h"

#include "ScriptEngine.h"

#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/StringUtility.h>
#include <Arca/LocalRelic.h>

SCENARIO_METHOD(AngelScriptRandomTestsFixture, "running random AngelScript scripts", "[script][angelscript]")
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

    GIVEN("script that returns random bool")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "bool main()\n" \
            "{\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateBool());\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
            }
        }
    }

    GIVEN("script that returns default random int8")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "int8 main()\n" \
            "{\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateInt8());\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int8_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns ranged random int8")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "int8 main()\n" \
            "{\n" \
            "    const auto range = Atmos::Int8Range(1, 10);\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateInt8(range));\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int8_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns default random int16")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "int16 main()\n" \
            "{\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateInt16());\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int16_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns ranged random int16")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "int16 main()\n" \
            "{\n" \
            "    const auto range = Atmos::Int16Range(1, 10);\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateInt16(range));\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int16_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns default random int32")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "int32 main()\n" \
            "{\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateInt());\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int32_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns ranged random int32")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "int32 main()\n" \
            "{\n" \
            "    const auto range = Atmos::IntRange(1, 10);\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateInt(range));\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int32_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns default random int64")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "int64 main()\n" \
            "{\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateInt64());\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int64_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns ranged random int64")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "int64 main()\n" \
            "{\n" \
            "    const auto range = Atmos::Int64Range(1, 10);\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateInt64(range));\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::int64_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns default random uint8")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "uint8 main()\n" \
            "{\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateUint8());\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint8_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns ranged random uint8")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "uint8 main()\n" \
            "{\n" \
            "    const auto range = Atmos::Uint8Range(1, 10);\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateUint8(range));\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint8_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns default random uint16")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "uint16 main()\n" \
            "{\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateUint16());\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint16_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns ranged random uint16")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "uint16 main()\n" \
            "{\n" \
            "    const auto range = Atmos::Uint16Range(1, 10);\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateUint16(range));\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint16_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns default random uint32")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "uint32 main()\n" \
            "{\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateUint());\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint32_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns ranged random uint32")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "uint32 main()\n" \
            "{\n" \
            "    const auto range = Atmos::UintRange(1, 10);\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateUint(range));\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint32_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns default random uint64")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "uint64 main()\n" \
            "{\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateUint64());\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint64_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns ranged random uint64")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "uint64 main()\n" \
            "{\n" \
            "    const auto range = Atmos::Uint64Range(1, 10);\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateUint64(range));\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::uint64_t>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns default random float")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "float main()\n" \
            "{\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateFloat());\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns ranged random double")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "double main()\n" \
            "{\n" \
            "    const auto range = Atmos::DoubleRange(1, 10);\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateDouble(range));\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }

    GIVEN("script that returns ranged random double")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "double main()\n" \
            "{\n" \
            "    const auto range = Atmos::DoubleRange(1, 10);\n" \
            "    return Arca::Reliquary::Do(Atmos::Random::GenerateDouble(range));\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<double>(std::get<Variant>(finishes[0].result)) != 0);
            }
        }
    }
}