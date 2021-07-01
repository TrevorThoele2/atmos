#include <catch.hpp>

#include "AngelScriptBoundsTests.h"

#include "ScriptEngine.h"

#include <Atmos/AngelScriptBounds.h>

#include <Atmos/SpatialAlgorithms.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>
#include <Arca/OpenRelic.h>

SCENARIO_METHOD(AngelScriptBoundsTestsFixture, "running bounds AngelScript scripts", "[script][angelscript]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    logger.Add<Logging::FileSink>();
    ScriptEngine engine(logger);

    auto fieldOrigin = Arca::ReliquaryOrigin();
    fieldOrigin.Register<Arca::OpenRelic>();
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

    GIVEN("Bounds")
    {
        auto openRelic = fieldReliquary.Do(Arca::Create<Arca::OpenRelic>());

        auto position = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto baseSize = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();
        auto scalers = dataGeneration.RandomStack<
            Spatial::Scalers2D, Spatial::Scalers2D::Value, Spatial::Scalers2D::Value>();
        auto rotation = dataGeneration.Random<Spatial::Angle2D>();
        auto bounds = fieldReliquary.Do(Arca::Create<Spatial::Bounds>{openRelic, Spatial::Space::World, position, baseSize, scalers, rotation});

        GIVEN("script that returns position")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(Arca::RelicID boundsID)\n" \
                "{\n" \
                "    auto bounds = Atmos::Spatial::Bounds(boundsID);\n" \
                "    return Atmos::ToString(bounds.Position().x) + \" \" + Atmos::ToString(bounds.Position().y) + \" \" + Atmos::ToString(bounds.Position().z);\n" \
                "}",
                { bounds.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult =
                        ToString(position.x) +
                        " " +
                        ToString(position.y) +
                        " " +
                        ToString(position.z);

                    const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == expectedResult);
                }
            }
        }

        GIVEN("script that returns base size")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(Arca::RelicID boundsID)\n" \
                "{\n" \
                "    auto bounds = Atmos::Spatial::Bounds(boundsID);\n" \
                "    return Atmos::ToString(bounds.BaseSize().width) + \" \" + Atmos::ToString(bounds.BaseSize().height);\n" \
                "}",
                { bounds.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult =
                        ToString(baseSize.width) +
                        " " +
                        ToString(baseSize.height);

                    const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == expectedResult);
                }
            }
        }

        GIVEN("script that returns size")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(Arca::RelicID boundsID)\n" \
                "{\n" \
                "    auto bounds = Atmos::Spatial::Bounds(boundsID);\n" \
                "    return Atmos::ToString(bounds.Size().width) + \" \" + Atmos::ToString(bounds.Size().height);\n" \
                "}",
                { bounds.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult =
                        ToString(bounds->Size().width) +
                        " " +
                        ToString(bounds->Size().height);

                    const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == expectedResult);
                }
            }
        }

        GIVEN("script that returns scalers")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(Arca::RelicID boundsID)\n" \
                "{\n" \
                "    auto bounds = Atmos::Spatial::Bounds(boundsID);\n" \
                "    return Atmos::ToString(bounds.Scalers().x) + \" \" + Atmos::ToString(bounds.Scalers().y);\n" \
                "}",
                { bounds.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult =
                        ToString(bounds->Scalers().x) +
                        " " +
                        ToString(bounds->Scalers().y);

                    const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == expectedResult);
                }
            }
        }

        GIVEN("script that returns rotation")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "Atmos::Spatial::Angle2D main(Arca::RelicID boundsID)\n" \
                "{\n" \
                "    auto bounds = Atmos::Spatial::Bounds(boundsID);\n" \
                "    return bounds.Rotation();\n" \
                "}",
                { bounds.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto result = std::get<Spatial::Angle2D>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == bounds->Rotation());
                }
            }
        }

        GIVEN("script that returns space")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "Atmos::Spatial::Space main(Arca::RelicID boundsID)\n" \
                "{\n" \
                "    auto bounds = Atmos::Spatial::Bounds(boundsID);\n" \
                "    return bounds.Space();\n" \
                "}",
                { bounds.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    
                    const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                    REQUIRE(static_cast<Spatial::Space>(result) == bounds->Space());
                }
            }
        }
        
        GIVEN("script that moves bounds to")
        {
            const auto toPosition = dataGeneration.RandomStack<
                Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

            CompileAndCreateScript(
                "basic_script.as",
                "class SignalHandler\n" \
                "{\n" \
                "    Atmos::Spatial::BoundsMoved signal;\n" \
                "    void Handle(Atmos::Spatial::BoundsMoved signal) { this.signal = signal; }\n" \
                "}\n" \
                "\n" \
                "string main(Arca::RelicID boundsID, float setX, float setY, float setZ)\n" \
                "{\n" \
                "    SignalHandler signalHandler;\n" \
                "    Arca::Reliquary::On(Atmos::Spatial::OnBoundsMoved(signalHandler.Handle));\n" \
                "\n" \
                "    auto bounds = Atmos::Spatial::Bounds(boundsID);\n" \
                "    auto startingPosition = bounds.Position();\n" \
                "\n" \
                "    auto command = Atmos::Spatial::MoveBounds(boundsID, Atmos::Spatial::Point3D(setX, setY, setZ));\n" \
                "    Arca::Reliquary::Do(command);\n" \
                "\n" \
                "    return Atmos::ToString(startingPosition.x) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(startingPosition.y) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(startingPosition.z) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(signalHandler.signal.id) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(signalHandler.signal.previousPosition.x) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(signalHandler.signal.previousPosition.y) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(signalHandler.signal.previousPosition.z);\n" \
                "}",
                { bounds.ID(), toPosition.x, toPosition.y, toPosition.z },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(position != bounds->Position());
                    REQUIRE(bounds->Position() == toPosition);

                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult =
                        ToString(position.x) +
                        " " +
                        ToString(position.y) +
                        " " +
                        ToString(position.z) +
                        " " +
                        ToString(bounds.ID()) +
                        " " +
                        ToString(position.x) +
                        " " +
                        ToString(position.y) +
                        " " +
                        ToString(position.z);

                    const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == expectedResult);
                }
            }
        }

        GIVEN("script that rotates bounds")
        {
            const auto toRotation = dataGeneration.Random<Spatial::Angle2D>();

            CompileAndCreateScript(
                "basic_script.as",
                "class SignalHandler\n" \
                "{\n" \
                "    Atmos::Spatial::BoundsRotated signal;\n" \
                "    void Handle(Atmos::Spatial::BoundsRotated signal) { this.signal = signal; }\n" \
                "}\n" \
                "\n" \
                "string main(Arca::RelicID boundsID, Atmos::Spatial::Angle2D angle)\n" \
                "{\n" \
                "    SignalHandler signalHandler;\n" \
                "    Arca::Reliquary::On(Atmos::Spatial::OnBoundsRotated(signalHandler.Handle));\n" \
                "\n" \
                "    auto bounds = Atmos::Spatial::Bounds(boundsID);\n" \
                "    auto startingRotation = bounds.Rotation();\n" \
                "\n" \
                "    auto command = Atmos::Spatial::RotateBounds(boundsID, angle);\n" \
                "    Arca::Reliquary::Do(command);\n" \
                "\n" \
                "    return Atmos::ToString(startingRotation) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(signalHandler.signal.id) +\n"
                "        \" \" +\n" \
                "        Atmos::ToString(signalHandler.signal.previousRotation);\n" \
                "}",
                { bounds.ID(), toRotation },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(rotation != bounds->Rotation());
                    REQUIRE(bounds->Rotation() == toRotation);

                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult = ToString(rotation) + " " + ToString(bounds.ID()) + " " + ToString(rotation);

                    const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == expectedResult);
                }
            }
        }

        GIVEN("script that scales bounds")
        {
            const auto toScalers = dataGeneration.RandomStack<Spatial::Scalers2D, Spatial::Scalers2D::Value, Spatial::Scalers2D::Value>();

            CompileAndCreateScript(
                "basic_script.as",
                "class SignalHandler\n" \
                "{\n" \
                "    Atmos::Spatial::BoundsScaled signal;\n" \
                "    void Handle(Atmos::Spatial::BoundsScaled signal) { this.signal = signal; }\n" \
                "}\n" \
                "\n" \
                "string main(Arca::RelicID boundsID, float scalerX, float scalerY)\n" \
                "{\n" \
                "    SignalHandler signalHandler;\n" \
                "    Arca::Reliquary::On(Atmos::Spatial::OnBoundsScaled(signalHandler.Handle));\n" \
                "\n" \
                "    auto bounds = Atmos::Spatial::Bounds(boundsID);\n" \
                "    auto startingScalers = bounds.Scalers();\n" \
                "\n" \
                "    auto command = Atmos::Spatial::ScaleBounds(boundsID, Atmos::Spatial::Scalers2D(scalerX, scalerY));\n" \
                "    Arca::Reliquary::Do(command);\n" \
                "\n" \
                "    return Atmos::ToString(startingScalers.x) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(startingScalers.y) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(signalHandler.signal.id) +\n"
                "        \" \" +\n" \
                "        Atmos::ToString(signalHandler.signal.previousScalers.x) +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(signalHandler.signal.previousScalers.y);\n" \
                "}",
                { bounds.ID(), toScalers.x, toScalers.y },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(scalers != bounds->Scalers());
                    REQUIRE(bounds->Scalers() == toScalers);

                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult = ToString(scalers.x) +
                        " " +
                        ToString(scalers.y) +
                        " " +
                        ToString(bounds.ID()) +
                        " " +
                        ToString(scalers.x) +
                        " " +
                        ToString(scalers.y);

                    const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result == expectedResult);
                }
            }
        }

        GIVEN("script that destroys Bounds")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "void main(Arca::RelicID id)\n" \
                "{\n"
                "    Arca::Reliquary::Do(Arca::Destroy<Atmos::Spatial::Bounds>(id));\n" \
                "}",
                { openRelic.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(!bounds);
                }
            }
        }
    }

    GIVEN("script that creates default Bounds")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main()\n" \
            "{\n"
            "    auto relic = Arca::Reliquary::Do(Arca::Create<Arca::OpenRelic>());\n" \
            "    auto bounds = Arca::Reliquary::Do(Arca::Create<Atmos::Spatial::Bounds>(relic.ID()));\n"
            "    return bounds.ID();\n"
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result));
                REQUIRE(Arca::Index<Spatial::Bounds>(result, fieldReliquary));
            }
        }
    }

    GIVEN("script that creates Bounds with arguments")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main()\n" \
            "{\n"
            "    auto relic = Arca::Reliquary::Do(Arca::Create<Arca::OpenRelic>());\n" \
            "    auto position = Atmos::Spatial::Point3D();\n" \
            "    auto baseSize = Atmos::Spatial::Size2D();\n" \
            "    auto scalers = Atmos::Spatial::Scalers2D();\n" \
            "    auto rotation = 0.0;\n" \
            "    auto bounds = Arca::Reliquary::Do(Arca::Create<Atmos::Spatial::Bounds>(relic.ID(), Atmos::Spatial::Space::World, position, baseSize, scalers, rotation));\n" \
            "    return bounds.ID();\n"\
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result));
                REQUIRE(Arca::Index<Spatial::Bounds>(result, fieldReliquary));
            }
        }
    }
}