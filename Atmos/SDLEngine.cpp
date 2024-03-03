#include "SDLEngine.h"

#include "SDLSystem.h"
#include "SDLWindow.h"
#include "RealAssetResourceManager.h"
#include "SDLInputManager.h"
#include "VulkanGraphicsManager.h"
#include "SDLAudioManager.h"
#include "AngelScriptManager.h"
#include "RealWorldManager.h"

namespace Atmos
{
    SDLEngine::SDLEngine(Logging::Logger& logger) :
        Engine(CreateInitializationProperties(logger), logger)
    {}

    SDLEngine::InitializationProperties SDLEngine::CreateInitializationProperties(Logging::Logger& logger)
    {
        using Properties = InitializationProperties;
        
        auto system = std::make_unique<System::SDL>();
        auto imageAssetManager = std::make_unique<Asset::Resource::RealManager>(logger);
        auto window = std::make_unique<Window::SDLWindow>(logger);
        auto input = std::make_unique<Input::SDLManager>(logger);
        auto graphics = std::make_unique<Render::Vulkan::GraphicsManager>(logger);
        auto audio = std::make_unique<Audio::SDL::Manager>(logger);
        auto scripts = std::make_unique<Scripting::Angel::Manager>(logger);
        auto world = std::make_unique<World::RealManager>();

        Properties properties;
        properties.system = std::move(system);
        properties.assetResourceManager = std::move(imageAssetManager);
        properties.window = std::move(window);
        properties.inputManager = std::move(input);
        properties.graphicsManager = std::move(graphics);
        properties.audioManager = std::move(audio);
        properties.scriptManager = std::move(scripts);
        properties.worldManager = std::move(world);
        return properties;
    }
}
