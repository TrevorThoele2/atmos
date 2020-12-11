#include "ScriptEngine.h"

ScriptEngine::ScriptEngine(Logging::Logger& logger) : Engine(logger)
{}

void ScriptEngine::SetupImplementation()
{

}

auto ScriptEngine::CreateInitializationProperties(Logging::Logger& logger)
    -> InitializationProperties
{
    using Properties = InitializationProperties;

    auto imageAssetManager = std::make_unique<MockImageAssetManager>();
    auto window = std::make_unique<MockWindow>();
    auto input = std::make_unique<Input::NullManager>();
    auto graphics = std::make_unique<MockGraphicsManager>(logger);
    auto audio = std::make_unique<Audio::NullAudioManager>();
    auto scripts = std::make_unique<Scripting::Angel::Manager>(logger);

    mockImageAssetManager = imageAssetManager.get();
    mockWindow = window.get();
    nullAudioManager = audio.get();
    nullInputManager = input.get();
    mockGraphicsManager = graphics.get();
    scriptManager = scripts.get();

    Properties properties;
    properties.imageAssetManager = std::move(imageAssetManager);
    properties.window = std::move(window);
    properties.inputManager = std::move(input);
    properties.graphicsManager = std::move(graphics);
    properties.audioManager = std::move(audio);
    properties.scriptManager = std::move(scripts);
    return properties;
}

void ScriptEngine::DoExit()
{}