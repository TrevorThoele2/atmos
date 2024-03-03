#include "DerivedEngine.h"

#include <Atmos/NullInputManager.h>
#include <Atmos/NullAudioManager.h>

auto DerivedEngine::CreateInitializationProperties(Arca::Reliquary& reliquary)
-> InitializationProperties
{
    this->globalReliquary = &reliquary;

    using Properties = InitializationProperties;

    auto window = std::make_unique<MockWindow>();
    auto input = std::make_unique<Input::NullManager>();
    auto graphics = std::make_unique<MockGraphicsManager>(reliquary);
    auto audio = std::make_unique<Audio::NullAudioManager>();

    mockWindow = window.get();
    mockGraphicsManager = graphics.get();

    Properties properties;
    properties.window = std::move(window);
    properties.inputManager = std::move(input);
    properties.graphicsManager = std::move(graphics);
    properties.audioManager = std::move(audio);
    return properties;
}

void DerivedEngine::DoExit()
{}