#include <catch.hpp>

#include "AngelScriptSpatialObjectsTests.h"

#include "ScriptEngine.h"

#include <Atmos/Angle3D.h>
#include <Atmos/Scalers2D.h>
#include <Atmos/Scalers3D.h>
#include <Atmos/Size3D.h>
#include <Atmos/AxisAlignedBox3D.h>
#include <Atmos/SpatialAlgorithms.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>

SCENARIO_METHOD(AngelScriptSpatialObjectsTestsFixture, "running spatial object AngelScript scripts", "[script][angelscript]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    logger.Add<Logging::FileSink>();
    ScriptEngine engine(logger);

    auto fieldOrigin = Arca::ReliquaryOrigin();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockImageAssetManager,
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

    GIVEN("script that constructs Angle2D")
    {
        auto value = dataGeneration.Random<Spatial::Angle2D>();

        CompileAndCreateScript(
            "basic_script.as",
            "Atmos::Spatial::Angle2D main(Atmos::Spatial::Angle2D value)\n" \
            "{\n" \
            "    return value;\n" \
            "}",
            { value },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) == value);
            }
        }
    }

    GIVEN("script that constructs Angle3D")
    {
        auto yaw = dataGeneration.Random<Spatial::Angle3D::Value>();
        auto pitch = dataGeneration.Random<Spatial::Angle3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float yaw, float pitch)\n" \
            "{\n" \
            "    Atmos::Spatial::Angle3D angle(yaw, pitch);\n" \
            "    return Atmos::ToString(angle.yaw) + \" \" + Atmos::ToString(angle.pitch);\n" \
            "}",
            { yaw, pitch },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(yaw) + " " + Atmos::ToString(pitch));
            }
        }
    }

    GIVEN("script that returns operator== from Angle3D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Angle3D, Spatial::Angle3D::Value, Spatial::Angle3D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Angle3D, Spatial::Angle3D::Value, Spatial::Angle3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftYaw, float leftPitch, float rightYaw, float rightPitch)\n" \
            "{\n" \
            "    Atmos::Spatial::Angle3D left(leftYaw, leftPitch);\n" \
            "    Atmos::Spatial::Angle3D right(rightYaw, rightPitch);\n" \
            "    return left == right;\n"
            "}",
            { left.yaw, left.pitch, right.yaw, right.pitch },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left == right));
            }
        }
    }

    GIVEN("script that returns operator!= from Angle3D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Angle3D, Spatial::Angle3D::Value, Spatial::Angle3D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Angle3D, Spatial::Angle3D::Value, Spatial::Angle3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftYaw, float leftPitch, float rightYaw, float rightPitch)\n" \
            "{\n" \
            "    Atmos::Spatial::Angle3D left(leftYaw, leftPitch);\n" \
            "    Atmos::Spatial::Angle3D right(rightYaw, rightPitch);\n" \
            "    return left != right;\n"
            "}",
            { left.yaw, left.pitch, right.yaw, right.pitch },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left != right));
            }
        }
    }

    GIVEN("script that constructs Point2D")
    {
        auto x = dataGeneration.Random<Spatial::Point2D::Value>();
        auto y = dataGeneration.Random<Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D point(x, y);\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y);\n" \
            "}",
            { x, y },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(x) + " " + Atmos::ToString(y));
            }
        }
    }

    GIVEN("script that returns operator== from Point2D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float rightX, float rightY)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D left(leftX, leftY);\n" \
            "    Atmos::Spatial::Point2D right(rightX, rightY);\n" \
            "    return left == right;\n"
            "}",
            { left.x, left.y, right.x, right.y },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left == right));
            }
        }
    }

    GIVEN("script that returns operator!= from Point2D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float rightX, float rightY)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D left(leftX, leftY);\n" \
            "    Atmos::Spatial::Point2D right(rightX, rightY);\n" \
            "    return left != right;\n"
            "}",
            { left.x, left.y, right.x, right.y },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left != right));
            }
        }
    }

    GIVEN("script that returns operator+ from Point2D")
    {
        auto leftX = dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000));
        auto leftY = dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000));
        auto rightX = dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000));
        auto rightY = dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000));

        auto left = Spatial::Point2D{ leftX, leftY };
        auto right = Spatial::Point2D{ rightX, rightY };

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float leftX, float leftY, float rightX, float rightY)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D left(leftX, leftY);\n" \
            "    Atmos::Spatial::Point2D right(rightX, rightY);\n" \
            "    auto result = left + right;\n" \
            "    return Atmos::ToString(result.x) + \" \" + Atmos::ToString(result.y);\n" \
            "}",
            { left.x, left.y, right.x, right.y },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = left + right;

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y));
            }
        }
    }

    GIVEN("script that returns operator- from Point2D")
    {
        auto leftX = dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000));
        auto leftY = dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000));
        auto rightX = dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000));
        auto rightY = dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000));

        auto left = Spatial::Point2D{ leftX, leftY };
        auto right = Spatial::Point2D{ rightX, rightY };

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float leftX, float leftY, float rightX, float rightY)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D left(leftX, leftY);\n" \
            "    Atmos::Spatial::Point2D right(rightX, rightY);\n" \
            "    auto result = left - right;\n" \
            "    return Atmos::ToString(result.x) + \" \" + Atmos::ToString(result.y);\n" \
            "}",
            { left.x, left.y, right.x, right.y },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = left - right;

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y));
            }
        }
    }

    GIVEN("script that constructs Point3D")
    {
        auto x = dataGeneration.Random<Spatial::Point3D::Value>();
        auto y = dataGeneration.Random<Spatial::Point3D::Value>();
        auto z = dataGeneration.Random<Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y, float z)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D point(x, y, z);\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y) + \" \" + Atmos::ToString(point.z);\n" \
            "}",
            { x, y, z },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(x) + " " + Atmos::ToString(y) + " " + Atmos::ToString(z));
            }
        }
    }

    GIVEN("script that returns operator+ from Point3D")
    {
        auto leftX = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));
        auto leftY = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));
        auto leftZ = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));
        auto rightX = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));
        auto rightY = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));
        auto rightZ = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));

        auto left = Spatial::Point3D{ leftX, leftY, leftZ };
        auto right = Spatial::Point3D{ rightX, rightY, rightZ };

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float leftX, float leftY, float leftZ, float rightX, float rightY, float rightZ)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D left(leftX, leftY, leftZ);\n" \
            "    Atmos::Spatial::Point3D right(rightX, rightY, rightZ);\n" \
            "    auto result = left + right;\n" \
            "    return Atmos::ToString(result.x) + \" \" + Atmos::ToString(result.y) + \" \" + Atmos::ToString(result.z);\n" \
            "}",
            { left.x, left.y, left.z, right.x, right.y, right.z },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = left + right;

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y) + " " + Atmos::ToString(expectedResult.z));
            }
        }
    }

    GIVEN("script that returns operator- from Point3D")
    {
        auto leftX = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));
        auto leftY = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));
        auto leftZ = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));
        auto rightX = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));
        auto rightY = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));
        auto rightZ = dataGeneration.Random<Spatial::Point3D::Value>(TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000));

        auto left = Spatial::Point3D{ leftX, leftY, leftZ };
        auto right = Spatial::Point3D{ rightX, rightY, rightZ };

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float leftX, float leftY, float leftZ, float rightX, float rightY, float rightZ)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D left(leftX, leftY, leftZ);\n" \
            "    Atmos::Spatial::Point3D right(rightX, rightY, rightZ);\n" \
            "    auto result = left - right;\n" \
            "    return Atmos::ToString(result.x) + \" \" + Atmos::ToString(result.y) + \" \" + Atmos::ToString(result.z);\n" \
            "}",
            { left.x, left.y, left.z, right.x, right.y, right.z },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = left - right;

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y) + " " + Atmos::ToString(expectedResult.z));
            }
        }
    }

    GIVEN("script that returns operator== from Point3D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float leftZ, float rightX, float rightY, float rightZ)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D left(leftX, leftY, leftZ);\n" \
            "    Atmos::Spatial::Point3D right(rightX, rightY, rightZ);\n" \
            "    return left == right;\n"
            "}",
            { left.x, left.y, left.z, right.x, right.y, right.z },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left == right));
            }
        }
    }

    GIVEN("script that returns operator!= from Point3D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float leftZ, float rightX, float rightY, float rightZ)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D left(leftX, leftY, leftZ);\n" \
            "    Atmos::Spatial::Point3D right(rightX, rightY, rightZ);\n" \
            "    return left != right;\n"
            "}",
            { left.x, left.y, left.z, right.x, right.y, right.z },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left != right));
            }
        }
    }

    GIVEN("script that default constructs GridPoint")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "string main()\n" \
            "{\n" \
            "    Atmos::Spatial::Grid::Point point;\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y);\n" \
            "}",
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has 0 for coordinates")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == Chroma::ToString(0) + " " + Chroma::ToString(0));
            }
        }
    }

    GIVEN("script that constructs GridPoint with properties")
    {
        auto x = dataGeneration.Random<Spatial::Grid::Point::Value>();
        auto y = dataGeneration.Random<Spatial::Grid::Point::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(int x, int y)\n" \
            "{\n" \
            "    Atmos::Spatial::Grid::Point point(x, y);\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y);\n" \
            "}",
            { x, y },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == Chroma::ToString(x) + " " + Chroma::ToString(y));
            }
        }
    }

    GIVEN("script that returns operator== from GridPoint")
    {
        auto leftX = dataGeneration.Random<Spatial::Grid::Point::Value>();
        auto leftY = dataGeneration.Random<Spatial::Grid::Point::Value>();
        auto rightX = dataGeneration.Random<Spatial::Grid::Point::Value>();
        auto rightY = dataGeneration.Random<Spatial::Grid::Point::Value>();

        auto left = Spatial::Grid::Point{ leftX, leftY };
        auto right = Spatial::Grid::Point{ rightX, rightY };

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(int leftX, int leftY, int rightX, int rightY)\n" \
            "{\n" \
            "    Atmos::Spatial::Grid::Point left(leftX, leftY);\n" \
            "    Atmos::Spatial::Grid::Point right(rightX, rightY);\n" \
            "    return left == right;\n" \
            "}",
            { leftX, leftY, rightX, rightY },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left == right));
            }
        }
    }

    GIVEN("script that returns operator!= from GridPoint")
    {
        auto leftX = dataGeneration.Random<Spatial::Grid::Point::Value>();
        auto leftY = dataGeneration.Random<Spatial::Grid::Point::Value>();
        auto rightX = dataGeneration.Random<Spatial::Grid::Point::Value>();
        auto rightY = dataGeneration.Random<Spatial::Grid::Point::Value>();

        auto left = Spatial::Grid::Point{ leftX, leftY };
        auto right = Spatial::Grid::Point{ rightX, rightY };

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(int leftX, int leftY, int rightX, int rightY)\n" \
            "{\n" \
            "    Atmos::Spatial::Grid::Point left(leftX, leftY);\n" \
            "    Atmos::Spatial::Grid::Point right(rightX, rightY);\n" \
            "    return left != right;\n" \
            "}",
            { leftX, leftY, rightX, rightY },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left != right));
            }
        }
    }

    GIVEN("script that constructs Scalers2D")
    {
        auto x = dataGeneration.Random<Spatial::Scalers2D::Value>();
        auto y = dataGeneration.Random<Spatial::Scalers2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y)\n" \
            "{\n" \
            "    Atmos::Spatial::Scalers2D scalers(x, y);\n" \
            "    return Atmos::ToString(scalers.x) + \" \" + Atmos::ToString(scalers.y);\n" \
            "}",
            { x, y },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(x) + " " + Atmos::ToString(y));
            }
        }
    }

    GIVEN("script that returns operator== from Scalers2D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Scalers2D, Spatial::Scalers2D::Value, Spatial::Scalers2D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Scalers2D, Spatial::Scalers2D::Value, Spatial::Scalers2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float rightX, float rightY)\n" \
            "{\n" \
            "    Atmos::Spatial::Scalers2D left(leftX, leftY);\n" \
            "    Atmos::Spatial::Scalers2D right(rightX, rightY);\n" \
            "    return left == right;\n"
            "}",
            { left.x, left.y, right.x, right.y },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left == right));
            }
        }
    }

    GIVEN("script that returns operator!= from Scalers2D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Scalers2D, Spatial::Scalers2D::Value, Spatial::Scalers2D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Scalers2D, Spatial::Scalers2D::Value, Spatial::Scalers2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float rightX, float rightY)\n" \
            "{\n" \
            "    Atmos::Spatial::Scalers2D left(leftX, leftY);\n" \
            "    Atmos::Spatial::Scalers2D right(rightX, rightY);\n" \
            "    return left != right;\n"
            "}",
            { left.x, left.y, right.x, right.y },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left != right));
            }
        }
    }

    GIVEN("script that constructs Scalers3D")
    {
        auto x = dataGeneration.Random<Spatial::Scalers3D::Value>();
        auto y = dataGeneration.Random<Spatial::Scalers3D::Value>();
        auto z = dataGeneration.Random<Spatial::Scalers3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y, float z)\n" \
            "{\n" \
            "    Atmos::Spatial::Scalers3D scalers(x, y, z);\n" \
            "    return Atmos::ToString(scalers.x) + \" \" + Atmos::ToString(scalers.y) + \" \" + Atmos::ToString(scalers.z);\n" \
            "}",
            { x, y, z },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(x) + " " + Atmos::ToString(y) + " " + Atmos::ToString(z));
            }
        }
    }

    GIVEN("script that returns operator== from Scalers3D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Scalers3D, Spatial::Scalers3D::Value, Spatial::Scalers3D::Value, Spatial::Scalers3D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Scalers3D, Spatial::Scalers3D::Value, Spatial::Scalers3D::Value, Spatial::Scalers3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float leftZ, float rightX, float rightY, float rightZ)\n" \
            "{\n" \
            "    Atmos::Spatial::Scalers3D left(leftX, leftY, leftZ);\n" \
            "    Atmos::Spatial::Scalers3D right(rightX, rightY, rightZ);\n" \
            "    return left == right;\n"
            "}",
            { left.x, left.y, left.z, right.x, right.y, right.z },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left == right));
            }
        }
    }

    GIVEN("script that returns operator!= from Scalers3D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Scalers3D, Spatial::Scalers3D::Value, Spatial::Scalers3D::Value, Spatial::Scalers3D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Scalers3D, Spatial::Scalers3D::Value, Spatial::Scalers3D::Value, Spatial::Scalers3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float leftZ, float rightX, float rightY, float rightZ)\n" \
            "{\n" \
            "    Atmos::Spatial::Scalers3D left(leftX, leftY, leftZ);\n" \
            "    Atmos::Spatial::Scalers3D right(rightX, rightY, rightZ);\n" \
            "    return left != right;\n"
            "}",
            { left.x, left.y, left.z, right.x, right.y, right.z },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left != right));
            }
        }
    }

    GIVEN("script that constructs Size2D")
    {
        auto width = dataGeneration.Random<Spatial::Size2D::Value>();
        auto height = dataGeneration.Random<Spatial::Size2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float width, float height)\n" \
            "{\n" \
            "    Atmos::Spatial::Size2D size(width, height);\n" \
            "    return Atmos::ToString(size.width) + \" \" + Atmos::ToString(size.height);\n" \
            "}",
            { width, height },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(width) + " " + Atmos::ToString(height));
            }
        }
    }

    GIVEN("script that returns operator== from Size2D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftWidth, float leftHeight, float rightWidth, float rightHeight)\n" \
            "{\n" \
            "    Atmos::Spatial::Size2D left(leftWidth, leftHeight);\n" \
            "    Atmos::Spatial::Size2D right(rightWidth, rightHeight);\n" \
            "    return left == right;\n"
            "}",
            { left.width, left.height, right.width, right.height },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left == right));
            }
        }
    }

    GIVEN("script that returns operator!= from Size2D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftWidth, float leftHeight, float rightWidth, float rightHeight)\n" \
            "{\n" \
            "    Atmos::Spatial::Size2D left(leftWidth, leftHeight);\n" \
            "    Atmos::Spatial::Size2D right(rightWidth, rightHeight);\n" \
            "    return left != right;\n"
            "}",
            { left.width, left.height, right.width, right.height },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left != right));
            }
        }
    }

    GIVEN("script that constructs Size3D")
    {
        auto width = dataGeneration.Random<Spatial::Size3D::Value>();
        auto height = dataGeneration.Random<Spatial::Size3D::Value>();
        auto depth = dataGeneration.Random<Spatial::Size3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float width, float height, float depth)\n" \
            "{\n" \
            "    Atmos::Spatial::Size3D size(width, height, depth);\n" \
            "    return Atmos::ToString(size.width) + \" \" + Atmos::ToString(size.height) + \" \" + Atmos::ToString(size.depth);\n" \
            "}",
            { width, height, depth },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(width) + " " + Atmos::ToString(height) + " " + Atmos::ToString(depth));
            }
        }
    }

    GIVEN("script that returns operator== from Size3D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftWidth, float leftHeight, float leftDepth, float rightWidth, float rightHeight, float rightDepth)\n" \
            "{\n" \
            "    Atmos::Spatial::Size3D left(leftWidth, leftHeight, leftDepth);\n" \
            "    Atmos::Spatial::Size3D right(rightWidth, rightHeight, rightDepth);\n" \
            "    return left == right;\n"
            "}",
            { left.width, left.height, left.depth, right.width, right.height, right.depth },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left == right));
            }
        }
    }

    GIVEN("script that returns operator!= from Size3D")
    {
        auto left = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();
        auto right = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftWidth, float leftHeight, float leftDepth, float rightWidth, float rightHeight, float rightDepth)\n" \
            "{\n" \
            "    Atmos::Spatial::Size3D left(leftWidth, leftHeight, leftDepth);\n" \
            "    Atmos::Spatial::Size3D right(rightWidth, rightHeight, rightDepth);\n" \
            "    return left != right;\n"
            "}",
            { left.width, left.height, left.depth, right.width, right.height, right.depth },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left != right));
            }
        }
    }

    GIVEN("script that returns AxisAlignedBox2D properties")
    {
        auto x = dataGeneration.Random<Spatial::Point2D::Value>();
        auto y = dataGeneration.Random<Spatial::Point2D::Value>();
        auto width = dataGeneration.Random<Spatial::Size2D::Value>();
        auto height = dataGeneration.Random<Spatial::Size2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y, float width, float height)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D center(x, y);\n" \
            "    Atmos::Spatial::Size2D size(width, height);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D box(center, size);\n" \
            "    return " \
            "Atmos::ToString(box.center.x) + \" \" + " \
            "Atmos::ToString(box.center.y) + \" \" + " \
            "Atmos::ToString(box.size.width) + \" \" + " \
            "Atmos::ToString(box.size.height);\n" \
            "}",
            { x, y, width, height },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result ==
                    Atmos::ToString(x) + " " +
                    Atmos::ToString(y) + " " +
                    Atmos::ToString(width) + " " +
                    Atmos::ToString(height));
            }
        }
    }

    GIVEN("script that returns operator== from AxisAlignedBox2D")
    {
        auto leftX = dataGeneration.Random<Spatial::Point2D::Value>();
        auto leftY = dataGeneration.Random<Spatial::Point2D::Value>();
        auto leftWidth = dataGeneration.Random<Spatial::Size2D::Value>();
        auto leftHeight = dataGeneration.Random<Spatial::Size2D::Value>();
        auto rightX = dataGeneration.Random<Spatial::Point2D::Value>();
        auto rightY = dataGeneration.Random<Spatial::Point2D::Value>();
        auto rightWidth = dataGeneration.Random<Spatial::Size2D::Value>();
        auto rightHeight = dataGeneration.Random<Spatial::Size2D::Value>();

        auto left = Spatial::AxisAlignedBox2D{ Spatial::Point2D{ leftX, leftY }, Spatial::Size2D { leftWidth, leftHeight } };
        auto right = Spatial::AxisAlignedBox2D{ Spatial::Point2D{ rightX, rightY }, Spatial::Size2D { rightWidth, rightHeight } };

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float leftWidth, float leftHeight, float rightX, float rightY, float rightWidth, float rightHeight)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D leftCenter(leftX, leftY);\n" \
            "    Atmos::Spatial::Size2D leftSize(leftWidth, leftHeight);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D leftBox(leftCenter, leftSize);\n" \
            "\n" \
            "    Atmos::Spatial::Point2D rightCenter(rightX, rightY);\n" \
            "    Atmos::Spatial::Size2D rightSize(rightWidth, rightHeight);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D rightBox(rightCenter, rightSize);\n" \
            "\n" \
            "    return leftBox == rightBox;\n"
            "}",
            { leftX, leftY, leftWidth, leftHeight, rightX, rightY, rightWidth, rightHeight },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left == right));
            }
        }
    }

    GIVEN("script that returns operator!= from AxisAlignedBox2D")
    {
        auto leftX = dataGeneration.Random<Spatial::Point2D::Value>();
        auto leftY = dataGeneration.Random<Spatial::Point2D::Value>();
        auto leftWidth = dataGeneration.Random<Spatial::Size2D::Value>();
        auto leftHeight = dataGeneration.Random<Spatial::Size2D::Value>();
        auto rightX = dataGeneration.Random<Spatial::Point2D::Value>();
        auto rightY = dataGeneration.Random<Spatial::Point2D::Value>();
        auto rightWidth = dataGeneration.Random<Spatial::Size2D::Value>();
        auto rightHeight = dataGeneration.Random<Spatial::Size2D::Value>();

        auto left = Spatial::AxisAlignedBox2D{ Spatial::Point2D{ leftX, leftY }, Spatial::Size2D { leftWidth, leftHeight } };
        auto right = Spatial::AxisAlignedBox2D{ Spatial::Point2D{ rightX, rightY }, Spatial::Size2D { rightWidth, rightHeight } };

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float leftWidth, float leftHeight, float rightX, float rightY, float rightWidth, float rightHeight)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D leftCenter(leftX, leftY);\n" \
            "    Atmos::Spatial::Size2D leftSize(leftWidth, leftHeight);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D leftBox(leftCenter, leftSize);\n" \
            "\n" \
            "    Atmos::Spatial::Point2D rightCenter(rightX, rightY);\n" \
            "    Atmos::Spatial::Size2D rightSize(rightWidth, rightHeight);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D rightBox(rightCenter, rightSize);\n" \
            "\n" \
            "    return leftBox != rightBox;\n" \
            "}",
            { leftX, leftY, leftWidth, leftHeight, rightX, rightY, rightWidth, rightHeight },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left != right));
            }
        }
    }

    GIVEN("script that returns AxisAlignedBox2D coordinates")
    {
        auto x = dataGeneration.Random<Spatial::Point2D::Value>();
        auto y = dataGeneration.Random<Spatial::Point2D::Value>();
        auto width = dataGeneration.Random<Spatial::Size2D::Value>();
        auto height = dataGeneration.Random<Spatial::Size2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y, float width, float height)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D center(x, y);\n" \
            "    Atmos::Spatial::Size2D size(width, height);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D box(center, size);\n" \
            "    return " \
            "Atmos::ToString(box.Left()) + \" \" + " \
            "Atmos::ToString(box.Top()) + \" \" + " \
            "Atmos::ToString(box.Right()) + \" \" + " \
            "Atmos::ToString(box.Bottom());\n" \
            "}",
            { x, y, width, height },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box = Spatial::AxisAlignedBox2D{
                    Spatial::Point2D{ x, y },
                    Spatial::Size2D{ width, height } };

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result ==
                    Atmos::ToString(box.Left()) + " " +
                    Atmos::ToString(box.Top()) + " " +
                    Atmos::ToString(box.Right()) + " " +
                    Atmos::ToString(box.Bottom()));
            }
        }
    }

    GIVEN("script that returns AxisAlignedBox3D properties")
    {
        auto x = dataGeneration.Random<Spatial::Point3D::Value>();
        auto y = dataGeneration.Random<Spatial::Point3D::Value>();
        auto z = dataGeneration.Random<Spatial::Point3D::Value>();
        auto width = dataGeneration.Random<Spatial::Size3D::Value>();
        auto height = dataGeneration.Random<Spatial::Size3D::Value>();
        auto depth = dataGeneration.Random<Spatial::Size3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y, float z, float width, float height, float depth)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D center(x, y, z);\n" \
            "    Atmos::Spatial::Size3D size(width, height, depth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D box(center, size);\n" \
            "    return " \
            "Atmos::ToString(box.center.x) + \" \" + " \
            "Atmos::ToString(box.center.y) + \" \" + " \
            "Atmos::ToString(box.center.z) + \" \" + " \
            "Atmos::ToString(box.size.width) + \" \" + " \
            "Atmos::ToString(box.size.height) + \" \" + " \
            "Atmos::ToString(box.size.depth);\n" \
            "}",
            { x, y, z, width, height, depth },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result ==
                    Atmos::ToString(x) + " " +
                    Atmos::ToString(y) + " " +
                    Atmos::ToString(z) + " " +
                    Atmos::ToString(width) + " " +
                    Atmos::ToString(height) + " " +
                    Atmos::ToString(depth));
            }
        }
    }

    GIVEN("script that returns operator== from AxisAlignedBox2D")
    {
        auto leftX = dataGeneration.Random<Spatial::Point3D::Value>();
        auto leftY = dataGeneration.Random<Spatial::Point3D::Value>();
        auto leftZ = dataGeneration.Random<Spatial::Point3D::Value>();
        auto leftWidth = dataGeneration.Random<Spatial::Size3D::Value>();
        auto leftHeight = dataGeneration.Random<Spatial::Size3D::Value>();
        auto leftDepth = dataGeneration.Random<Spatial::Size3D::Value>();
        auto rightX = dataGeneration.Random<Spatial::Point3D::Value>();
        auto rightY = dataGeneration.Random<Spatial::Point3D::Value>();
        auto rightZ = dataGeneration.Random<Spatial::Point3D::Value>();
        auto rightWidth = dataGeneration.Random<Spatial::Size3D::Value>();
        auto rightHeight = dataGeneration.Random<Spatial::Size3D::Value>();
        auto rightDepth = dataGeneration.Random<Spatial::Size3D::Value>();

        auto left = Spatial::AxisAlignedBox3D{ Spatial::Point3D{ leftX, leftY, leftZ }, Spatial::Size3D { leftWidth, leftHeight, leftDepth } };
        auto right = Spatial::AxisAlignedBox3D{ Spatial::Point3D{ rightX, rightY, rightZ }, Spatial::Size3D { rightWidth, rightHeight, rightDepth } };

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float leftZ, float leftWidth, float leftHeight, float leftDepth, " \
            "float rightX, float rightY, float rightZ, float rightWidth, float rightHeight, float rightDepth)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D leftCenter(leftX, leftY, leftZ);\n" \
            "    Atmos::Spatial::Size3D leftSize(leftWidth, leftHeight, leftDepth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D leftBox(leftCenter, leftSize);\n" \
            "\n" \
            "    Atmos::Spatial::Point3D rightCenter(rightX, rightY, rightZ);\n" \
            "    Atmos::Spatial::Size3D rightSize(rightWidth, rightHeight, rightDepth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D rightBox(rightCenter, rightSize);\n" \
            "\n" \
            "    return leftBox == rightBox;\n" \
            "}",
            { leftX, leftY, leftWidth, leftHeight, rightX, rightY, rightWidth, rightHeight },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left == right));
            }
        }
    }

    GIVEN("script that returns operator!= from AxisAlignedBox3D")
    {
        auto leftX = dataGeneration.Random<Spatial::Point3D::Value>();
        auto leftY = dataGeneration.Random<Spatial::Point3D::Value>();
        auto leftZ = dataGeneration.Random<Spatial::Point3D::Value>();
        auto leftWidth = dataGeneration.Random<Spatial::Size3D::Value>();
        auto leftHeight = dataGeneration.Random<Spatial::Size3D::Value>();
        auto leftDepth = dataGeneration.Random<Spatial::Size3D::Value>();
        auto rightX = dataGeneration.Random<Spatial::Point3D::Value>();
        auto rightY = dataGeneration.Random<Spatial::Point3D::Value>();
        auto rightZ = dataGeneration.Random<Spatial::Point3D::Value>();
        auto rightWidth = dataGeneration.Random<Spatial::Size3D::Value>();
        auto rightHeight = dataGeneration.Random<Spatial::Size3D::Value>();
        auto rightDepth = dataGeneration.Random<Spatial::Size3D::Value>();

        auto left = Spatial::AxisAlignedBox3D{ Spatial::Point3D{ leftX, leftY, leftZ }, Spatial::Size3D { leftWidth, leftHeight, leftDepth } };
        auto right = Spatial::AxisAlignedBox3D{ Spatial::Point3D{ rightX, rightY, rightZ }, Spatial::Size3D { rightWidth, rightHeight, rightDepth } };

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float leftX, float leftY, float leftZ, float leftWidth, float leftHeight, float leftDepth, " \
            "float rightX, float rightY, float rightZ, float rightWidth, float rightHeight, float rightDepth)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D leftCenter(leftX, leftY, leftZ);\n" \
            "    Atmos::Spatial::Size3D leftSize(leftWidth, leftHeight, leftDepth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D leftBox(leftCenter, leftSize);\n" \
            "\n" \
            "    Atmos::Spatial::Point3D rightCenter(rightX, rightY, rightZ);\n" \
            "    Atmos::Spatial::Size3D rightSize(rightWidth, rightHeight, rightDepth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D rightBox(rightCenter, rightSize);\n" \
            "\n" \
            "    return leftBox != rightBox;\n" \
            "}",
            { leftX, leftY, leftWidth, leftHeight, rightX, rightY, rightWidth, rightHeight },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == (left != right));
            }
        }
    }

    GIVEN("script that returns AxisAlignedBox3D coordinates")
    {
        auto x = dataGeneration.Random<Spatial::Point3D::Value>();
        auto y = dataGeneration.Random<Spatial::Point3D::Value>();
        auto z = dataGeneration.Random<Spatial::Point3D::Value>();
        auto width = dataGeneration.Random<Spatial::Size3D::Value>();
        auto height = dataGeneration.Random<Spatial::Size3D::Value>();
        auto depth = dataGeneration.Random<Spatial::Size3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y, float z, float width, float height, float depth)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D center(x, y, z);\n" \
            "    Atmos::Spatial::Size3D size(width, height, depth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D box(center, size);\n" \
            "    return " \
            "Atmos::ToString(box.Left()) + \" \" + " \
            "Atmos::ToString(box.Top()) + \" \" + " \
            "Atmos::ToString(box.FarZ()) + \" \" + " \
            "Atmos::ToString(box.Right()) + \" \" + " \
            "Atmos::ToString(box.Bottom()) + \" \" + " \
            "Atmos::ToString(box.NearZ());\n" \
            "}",
            { x, y, z, width, height, depth },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box = Spatial::AxisAlignedBox3D{
                    Spatial::Point3D { x, y, z },
                    Spatial::Size3D { width, height, depth } };

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result ==
                    Atmos::ToString(box.Left()) + " " +
                    Atmos::ToString(box.Top()) + " " +
                    Atmos::ToString(box.FarZ()) + " " +
                    Atmos::ToString(box.Right()) + " " +
                    Atmos::ToString(box.Bottom()) + " " +
                    Atmos::ToString(box.NearZ()));
            }
        }
    }
}