#include "ScriptEngine.h"

ScriptEngine::ScriptEngine(Logging::Logger& logger) : Engine(CreateInitializationProperties(logger), logger)
{}

auto ScriptEngine::CreateInitializationProperties(Logging::Logger& logger)
    -> InitializationProperties
{
    using Properties = InitializationProperties;

    auto imageAssetManager = std::make_unique<MockAssetResourceManager>(logger);
    auto window = std::make_unique<MockWindow>(logger);
    auto input = std::make_unique<MockInputManager>(logger);
    auto graphics = std::make_unique<MockGraphicsManager>(logger);
    auto text = std::make_unique<MockTextManager>();
    auto audio = std::make_unique<MockAudioManager>(logger);
    auto scripts = std::make_unique<Scripting::Angel::Manager>(logger);
    auto world = std::make_unique<MockWorldManager>();

    mockAssetResourceManager = imageAssetManager.get();
    mockWindow = window.get();
    mockAudioManager = audio.get();
    mockInputManager = input.get();
    mockGraphicsManager = graphics.get();
    mockTextManager = text.get();
    scriptManager = scripts.get();
    mockWorldManager = world.get();

    Properties properties;
    properties.assetResourceManager = std::move(imageAssetManager);
    properties.window = std::move(window);
    properties.inputManager = std::move(input);
    properties.graphicsManager = std::move(graphics);
    properties.textManager = std::move(text);
    properties.audioManager = std::move(audio);
    properties.scriptManager = std::move(scripts);
    properties.worldManager = std::move(world);
    return properties;
}