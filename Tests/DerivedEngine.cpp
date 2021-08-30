#include "DerivedEngine.h"

DerivedEngine::DerivedEngine(Logging::Logger& logger) :
    Engine(CreateInitializationProperties(logger), logger),
    worldManager(std::make_unique<World::RealManager>(
        [this]() { return std::unique_ptr<Arca::Reliquary>{}; },
        [this]() { return std::unique_ptr<Inscription::LoadAssetsUserContext>{}; }))
{}

auto DerivedEngine::CreateInitializationProperties(Logging::Logger& logger)
    -> InitializationProperties
{
    using Properties = InitializationProperties;

    auto assetResourceManager = std::make_unique<MockAssetResourceManager>(logger);
    auto window = std::make_unique<MockWindow>(logger);
    auto input = std::make_unique<MockInputManager>(logger);
    auto graphics = std::make_unique<MockGraphicsManager>(logger);
    auto text = std::make_unique<MockTextManager>();
    auto audio = std::make_unique<MockAudioManager>(logger);
    auto scripts = std::make_unique<MockScriptManager>(logger);

    mockAssetResourceManager = assetResourceManager.get();
    mockWindow = window.get();
    mockAudioManager = audio.get();
    mockInputManager = input.get();
    mockGraphicsManager = graphics.get();
    mockTextManager = text.get();
    mockScriptManager = scripts.get();

    Properties properties;
    properties.assetResourceManager = std::move(assetResourceManager);
    properties.window = std::move(window);
    properties.inputManager = std::move(input);
    properties.graphicsManager = std::move(graphics);
    properties.textManager = std::move(text);
    properties.audioManager = std::move(audio);
    properties.scriptManager = std::move(scripts);
    return properties;
}