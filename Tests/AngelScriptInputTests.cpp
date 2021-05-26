#include <catch.hpp>

#include "AngelScriptInputTests.h"

#include "ScriptEngine.h"

#include <Atmos/ActionAsset.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>

SCENARIO_METHOD(AngelScriptInputTestsFixture, "running input AngelScript scripts", "[script][angelscript][input]")
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
    fieldOrigin.CuratorCommandPipeline<Work>(Arca::Pipeline{ Input::Stage(), Scripting::Stage() });
    World::Field field(0, fieldOrigin.Actualize());

    auto& fieldReliquary = field.Reliquary();

    std::vector<Scripting::Finished> finishes;
    fieldReliquary.On<Scripting::Finished>([&finishes](const Scripting::Finished& signal)
        {
            finishes.push_back(signal);
        });

    GIVEN("script that returns current mouse position")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "string main()\n" \
            "{\n" \
            "    auto information = Atmos::Input::Information();\n" \
            "    return Atmos::ToString(information.CurrentMousePosition().x) + \" \" + Atmos::ToString(information.CurrentMousePosition().y);\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            const auto mousePosition =
                dataGeneration.RandomStack<Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

            engine.mockInputManager->sendState.mousePosition = mousePosition;
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = ToString(mousePosition.x) + " " + ToString(mousePosition.y);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
            }
        }
    }

    GIVEN("script that returns previous mouse position")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "string main()\n" \
            "{\n" \
            "    auto information = Atmos::Input::Information();\n" \
            "    return Atmos::ToString(information.PreviousMousePosition().x) + \" \" + Atmos::ToString(information.PreviousMousePosition().y);\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            const auto mousePosition =
                dataGeneration.RandomStack<Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

            engine.mockInputManager->sendState.mousePosition = mousePosition;
            fieldReliquary.Do(Work{});
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = ToString(mousePosition.x) + " " + ToString(mousePosition.y);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
            }
        }
    }

    GIVEN("script that returns action active")
    {
        auto name = dataGeneration.Random<std::string>();
        const auto boundKey = Input::Key::Z;

        fieldReliquary.Do(Arca::Create<Asset::Action>{ name, boundKey, Asset::Action::Modifiers{} });

        CompileAndCreateScript(
            "basic_script.as",
            "class SignalHandler\n" \
            "{\n" \
            "    Atmos::Input::ActionActive signal;\n" \
            "    void Handle(Atmos::Input::ActionActive signal) { this.signal = signal; }\n" \
            "}\n" \
            "\n" \
            "string main()\n" \
            "{\n" \
            "    SignalHandler signalHandler;\n" \
            "    Arca::Reliquary::On(Atmos::Input::OnActionActive(signalHandler.Handle));\n" \
            "\n" \
            "    Arca::Reliquary::Do(Atmos::Scripting::Suspend(Atmos::CurrentScript()));\n" \
            "\n" \
            "    return signalHandler.signal.action.Name();\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            engine.mockInputManager->sendState.keyStates.z = Input::KeyState::Down;
            fieldReliquary.Do(Work{});
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == name);
            }
        }
    }

    GIVEN("script that returns action depressed")
    {
        auto name = dataGeneration.Random<std::string>();
        const auto boundKey = Input::Key::Z;

        fieldReliquary.Do(Arca::Create<Asset::Action>{ name, boundKey, Asset::Action::Modifiers{} });

        CompileAndCreateScript(
            "basic_script.as",
            "class SignalHandler\n" \
            "{\n" \
            "    Atmos::Input::ActionDepressed signal;\n" \
            "    void Handle(Atmos::Input::ActionDepressed signal) { this.signal = signal; }\n" \
            "}\n" \
            "\n" \
            "string main()\n" \
            "{\n" \
            "    SignalHandler signalHandler;\n" \
            "    Arca::Reliquary::On(Atmos::Input::OnActionDepressed(signalHandler.Handle));\n" \
            "\n" \
            "    Arca::Reliquary::Do(Atmos::Scripting::Suspend(Atmos::CurrentScript()));\n" \
            "\n" \
            "    return signalHandler.signal.action.Name();\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            engine.mockInputManager->sendState.keyStates.z = Input::KeyState::Down;
            fieldReliquary.Do(Work{});
            engine.mockInputManager->sendState.keyStates.z = Input::KeyState::Up;
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == name);
            }
        }
    }

    GIVEN("script that returns action pressed")
    {
        auto name = dataGeneration.Random<std::string>();
        const auto boundKey = Input::Key::Z;

        fieldReliquary.Do(Arca::Create<Asset::Action>{ name, boundKey, Asset::Action::Modifiers{} });

        CompileAndCreateScript(
            "basic_script.as",
            "class SignalHandler\n" \
            "{\n" \
            "    Atmos::Input::ActionPressed signal;\n" \
            "    void Handle(Atmos::Input::ActionPressed signal) { this.signal = signal; }\n" \
            "}\n" \
            "\n" \
            "string main()\n" \
            "{\n" \
            "    SignalHandler signalHandler;\n" \
            "    Arca::Reliquary::On(Atmos::Input::OnActionPressed(signalHandler.Handle));\n" \
            "\n" \
            "    Arca::Reliquary::Do(Atmos::Scripting::Suspend(Atmos::CurrentScript()));\n" \
            "\n" \
            "    return signalHandler.signal.action.Name();\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            engine.mockInputManager->sendState.keyStates.z = Input::KeyState::Up;
            fieldReliquary.Do(Work{});
            engine.mockInputManager->sendState.keyStates.z = Input::KeyState::Down;
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == name);
            }
        }
    }

    GIVEN("script that returns mouse moved")
    {
        const auto previousMousePosition = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        const auto currentMousePosition = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "class SignalHandler\n" \
            "{\n" \
            "    Atmos::Input::MouseMoved signal;\n" \
            "    void Handle(Atmos::Input::MouseMoved signal) { this.signal = signal; }\n" \
            "}\n" \
            "\n" \
            "string main()\n" \
            "{\n" \
            "    SignalHandler signalHandler;\n" \
            "    Arca::Reliquary::On(Atmos::Input::OnMouseMoved(signalHandler.Handle));\n" \
            "\n" \
            "    Arca::Reliquary::Do(Atmos::Scripting::Suspend(Atmos::CurrentScript()));\n" \
            "\n" \
            "    return\n" \
            "        Atmos::ToString(signalHandler.signal.previous.x) + \" \" +\n" \
            "        Atmos::ToString(signalHandler.signal.previous.y) + \" \" +\n" \
            "        Atmos::ToString(signalHandler.signal.current.x) + \" \" +\n" \
            "        Atmos::ToString(signalHandler.signal.current.y);\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            engine.mockInputManager->sendState.mousePosition = previousMousePosition;
            fieldReliquary.Do(Work{});
            engine.mockInputManager->sendState.mousePosition = currentMousePosition;
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedString =
                    ToString(previousMousePosition.x) + " " +
                    ToString(previousMousePosition.y) + " " +
                    ToString(currentMousePosition.x) + " " +
                    ToString(currentMousePosition.y);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedString);
            }
        }
    }

    GIVEN("script that returns text entered")
    {
        const auto text = dataGeneration.Random<String>();

        CompileAndCreateScript(
            "basic_script.as",
            "class SignalHandler\n" \
            "{\n" \
            "    Atmos::Input::TextEntered signal;\n" \
            "    void Handle(Atmos::Input::TextEntered signal) { this.signal = signal; }\n" \
            "}\n" \
            "\n" \
            "string main()\n" \
            "{\n" \
            "    SignalHandler signalHandler;\n" \
            "    Arca::Reliquary::On(Atmos::Input::OnTextEntered(signalHandler.Handle));\n" \
            "\n" \
            "    Arca::Reliquary::Do(Atmos::Scripting::Suspend(Atmos::CurrentScript()));\n" \
            "\n" \
            "    return signalHandler.signal.text;\n" \
            "}",
            {},
            fieldReliquary);

        WHEN("working reliquary")
        {
            engine.mockInputManager->sendState.enteredText = {};
            fieldReliquary.Do(Work{});
            engine.mockInputManager->sendState.enteredText = {text};
            fieldReliquary.Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == text);
            }
        }
    }
}