#include "WindowsEngine.h"
#include <utility>

#include "FreeImageAssetManager.h"
#include "WindowsWindow.h"
#include "WindowsInputManager.h"
#include "VulkanGraphicsManager.h"
#include "NullAudioManager.h"
#include "AngelScriptManager.h"

#include "Environment.h"
#include "FileManagerProvider.h"
#include "WindowsFileManager.h"

namespace Atmos
{
    WindowsEngine::WindowsEngine(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, String className, Logging::Logger& logger) :
        Engine(logger), hInstance(hInstance), lpCmdLine(lpCmdLine), nCmdShow(nCmdShow), className(std::move(className))
    {}

    void WindowsEngine::SetupImplementation()
    {
        environment.Setup<Environment>("\n");
        File::manager.Setup<File::WindowsFileManager>();
    }

    WindowsEngine::InitializationProperties WindowsEngine::CreateInitializationProperties(Logging::Logger& logger)
    {
        using Properties = InitializationProperties;

        auto imageAssetManager = std::make_unique<Asset::FreeImageManager>(logger);
        auto window = std::make_unique<Window::WindowsWindow>(hInstance, nCmdShow, className);
        auto input = std::make_unique<Input::WindowsManager>(HWND(window->Handle()));
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

    void WindowsEngine::DoExit()
    {
        PostQuitMessage(0);
    }
}
