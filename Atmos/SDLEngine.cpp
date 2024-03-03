#include "SDLEngine.h"

#include "SDLWindow.h"
#include "FreeImageAssetManager.h"
#include "SDLInputManager.h"
#include "VulkanGraphicsManager.h"
#include "NullAudioManager.h"
#include "AngelScriptManager.h"

namespace Atmos
{
    SDLEngine::SDLEngine(Logging::Logger& logger) :
        Engine(logger)
    {}

    SDLEngine::InitializationProperties SDLEngine::CreateInitializationProperties(Logging::Logger& logger)
    {
        using Properties = InitializationProperties;

        auto imageAssetManager = std::make_unique<Asset::FreeImageManager>(logger);
        auto window = std::make_unique<Window::SDLWindow>();
        auto input = std::make_unique<Input::SDLManager>();
        auto graphics = std::make_unique<Render::Vulkan::GraphicsManager>(logger);
        auto audio = std::make_unique<Audio::NullAudioManager>();
        auto scripts = std::make_unique<Scripting::Angel::Manager>(logger);

        Properties properties;
        properties.imageAssetManager = std::move(imageAssetManager);
        properties.window = std::move(window);
        properties.inputManager = std::move(input);
        properties.graphicsManager = std::move(graphics);
        properties.audioManager = std::move(audio);
        properties.scriptManager = std::move(scripts);
        return properties;
    }
}
