#include <catch.hpp>

#include "JavaScriptInputTests.h"

#include <Atmos/ActionAsset.h>
#include <Arca/Create.h>

SCENARIO_METHOD(JavaScriptInputTestsFixture, "running input JavaScript scripts", "[script][javascript][input]")
{
    GIVEN("script that returns mouse moved")
    {
        const auto previousMousePosition = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        const auto currentMousePosition = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

let signal: Atmos.Input.MouseMoved | null = null;
const onSignal = (receivedSignal: Atmos.Input.MouseMoved) => {
    signal = receivedSignal;
}

export const main = (): Atmos.Result => {
    Atmos.Reliquary.listen(Atmos.Traits.Input.MouseMoved.typeName, onSignal);
    return {
        done: signal !== null,
        value: signal !== null ? `${signal.previous.x} ${signal.previous.y} ${signal.current.x} ${signal.current.y}` : null
    };
};)V0G0N",
            *fieldReliquary);

        WHEN("working reliquary")
        {
            engine.mockInputManager->sendState.mousePosition = previousMousePosition;
            fieldReliquary->Do(Work{});
            engine.mockInputManager->sendState.mousePosition = currentMousePosition;
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                const auto tokens = Chroma::Split(result, String(" "));
                const auto previousMouseX = Chroma::FromString<float>(tokens[0]);
                const auto previousMouseY = Chroma::FromString<float>(tokens[1]);
                const auto currentMouseX = Chroma::FromString<float>(tokens[2]);
                const auto currentMouseY = Chroma::FromString<float>(tokens[3]);
                REQUIRE(previousMouseX == Approx(previousMousePosition.x));
                REQUIRE(previousMouseY == Approx(previousMousePosition.y));
                REQUIRE(currentMouseX == Approx(currentMousePosition.x));
                REQUIRE(currentMouseY == Approx(currentMousePosition.y));
            }
        }
    }
}