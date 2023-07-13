#include "JavaScriptEngine.h"

JavaScriptEngine::JavaScriptEngine(Logging::Logger& logger) :
    Engine(CreateInitializationProperties(logger), Chroma::TypeIdentity<MockWorldManager>{}, logger),
    mockWorldManager(dynamic_cast<MockWorldManager*>(WorldManager()))
{}

auto JavaScriptEngine::CreateInitializationProperties(Logging::Logger& logger) -> InitializationProperties
{
    using Properties = InitializationProperties;

    auto imageAssetManager = std::make_unique<MockAssetResourceManager>(logger);
    auto window = std::make_unique<MockWindow>(logger);
    auto input = std::make_unique<MockInputManager>(logger);
    auto graphics = std::make_unique<MockGraphicsManager>(logger);
    auto text = std::make_unique<MockTextManager>();
    auto audio = std::make_unique<MockAudioManager>(logger);
    auto scripts = std::make_unique<Scripting::JavaScript::Manager>(logger);

    mockAssetResourceManager = imageAssetManager.get();
    mockWindow = window.get();
    mockAudioManager = audio.get();
    mockInputManager = input.get();
    mockGraphicsManager = graphics.get();
    mockTextManager = text.get();
    scriptManager = scripts.get();

    Properties properties;
    properties.assetResourceManager = std::move(imageAssetManager);
    properties.window = std::move(window);
    properties.inputManager = std::move(input);
    properties.graphicsManager = std::move(graphics);
    properties.textManager = std::move(text);
    properties.audioManager = std::move(audio);
    properties.scriptManager = std::move(scripts);
    return properties;
}