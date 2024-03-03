#include "DerivedEngine.h"

#include <Atmos/NullInputManager.h>
#include <Atmos/NullAudioManager.h>

Arca::Reliquary* DerivedEngine::TheGlobalReliquary()
{
    return GlobalReliquary();
}

const Arca::Reliquary* DerivedEngine::TheGlobalReliquary() const
{
    return GlobalReliquary();
}

void DerivedEngine::SetupImplementation()
{
    
}

auto DerivedEngine::CreateInitializationProperties()
    -> InitializationProperties
{
    using Properties = InitializationProperties;

    auto window = std::make_unique<MockWindow>();
    auto input = std::make_unique<Input::NullManager>();
    auto graphics = std::make_unique<MockGraphicsManager>();
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