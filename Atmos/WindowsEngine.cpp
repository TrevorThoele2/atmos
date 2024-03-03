#include "WindowsEngine.h"
#include <utility>

#include "WindowsWindow.h"
#include "WindowsInputManager.h"
#include "DirectX9GraphicsManager.h"
#include "DirectX9AudioManager.h"

#include "Environment.h"
#include "FileManagerProvider.h"
#include "WindowsFileManager.h"

namespace Atmos
{
    WindowsEngine::WindowsEngine(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, String className) :
        hInstance(hInstance), lpCmdLine(lpCmdLine), nCmdShow(nCmdShow), className(std::move(className))
    {}

    void WindowsEngine::SetupImplementation()
    {
        environment.Setup<Environment>("\n");
        File::manager.Setup<File::WindowsFileManager>();
    }

    WindowsEngine::InitializationProperties WindowsEngine::CreateInitializationProperties()
    {
        using Properties = InitializationProperties;

        auto window = std::make_unique<Window::WindowsWindow>(nCmdShow, className);
        auto input = std::make_unique<Input::WindowsManager>(HWND(window->Handle()));
        auto graphics = std::make_unique<Render::DirectX9::GraphicsManager>
        (
            HWND(window->Handle()),
            window->IsFullscreen()
        );
        auto audio = std::make_unique<Audio::DirectX9AudioManager>();

        Properties properties;
        properties.window = std::move(window);
        properties.inputManager = std::move(input);
        properties.graphicsManager = std::move(graphics);
        properties.audioManager = std::move(audio);
        return properties;
    }

    void WindowsEngine::DoExit()
    {
        PostQuitMessage(0);
    }
}
