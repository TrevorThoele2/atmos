#include "DerivedEngine.h"

#include <Atmos/NullInputManager.h>
#include <Atmos/NullAudioManager.h>

DerivedEngine::DerivedEngine(Logging::Logger& logger) : Engine(logger)
{}

void DerivedEngine::SetupImplementation()
{
    
}

auto DerivedEngine::CreateInitializationProperties(Logging::Logger& logger)
    -> InitializationProperties
{
    using Properties = InitializationProperties;

    auto imageAssetManager = std::make_unique<MockImageAssetManager>();
    auto window = std::make_unique<MockWindow>();
    auto input = std::make_unique<Input::NullManager>();
    auto graphics = std::make_unique<MockGraphicsManager>(logger);
    auto audio = std::make_unique<Audio::NullAudioManager>();
    auto scripts = std::make_unique<MockScriptManager>();

    mockImageAssetManager = imageAssetManager.get();
    mockWindow = window.get();
    nullAudioManager = audio.get();
    nullInputManager = input.get();
    mockGraphicsManager = graphics.get();
    mockScriptManager = scripts.get();

    Properties properties;
    properties.imageAssetManager = std::move(imageAssetManager);
    properties.window = std::move(window);
    properties.inputManager = std::move(input);
    properties.graphicsManager = std::move(graphics);
    properties.audioManager = std::move(audio);
    properties.scriptManager = std::move(scripts);
    return properties;
}

void DerivedEngine::DoExit()
{}