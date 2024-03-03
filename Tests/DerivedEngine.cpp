#include "DerivedEngine.h"

DerivedEngine::DerivedEngine(Logging::Logger& logger) : Engine(CreateInitializationProperties(logger), logger)
{}

auto DerivedEngine::CreateInitializationProperties(Logging::Logger& logger)
    -> InitializationProperties
{
    using Properties = InitializationProperties;

    auto assetResourceManager = std::make_unique<MockAssetResourceManager>(logger);
    auto window = std::make_unique<MockWindow>(logger);
    auto input = std::make_unique<MockInputManager>(logger);
    auto graphics = std::make_unique<MockGraphicsManager>(logger);
    auto audio = std::make_unique<MockAudioManager>(logger);
    auto scripts = std::make_unique<MockScriptManager>(logger);
    auto world = std::make_unique<World::RealManager>();

    mockAssetResourceManager = assetResourceManager.get();
    mockWindow = window.get();
    mockAudioManager = audio.get();
    mockInputManager = input.get();
    mockGraphicsManager = graphics.get();
    mockScriptManager = scripts.get();
    worldManager = world.get();

    Properties properties;
    properties.assetResourceManager = std::move(assetResourceManager);
    properties.window = std::move(window);
    properties.inputManager = std::move(input);
    properties.graphicsManager = std::move(graphics);
    properties.audioManager = std::move(audio);
    properties.scriptManager = std::move(scripts);
    properties.worldManager = std::move(world);
    return properties;
}