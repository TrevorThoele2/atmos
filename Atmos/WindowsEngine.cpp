#include "WindowsEngine.h"
#include <utility>

#include "WindowsWindow.h"
#include "WindowsInputManager.h"
#include "DirectX9GraphicsManager.h"
#include "DirectX9AudioManager.h"
#include "WindowsFileManager.h"

namespace Atmos
{
    WindowsEngine::WindowsEngine(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, String className) :
        hInstance(hInstance), lpCmdLine(lpCmdLine), nCmdShow(nCmdShow), className(std::move(className))
    {}

    WindowsEngine::InitializationProperties WindowsEngine::CreateInitializationProperties(
        Arca::Reliquary& reliquary)
    {
        using Properties = InitializationProperties;

        auto window = std::make_unique<Window::WindowsWindow>(reliquary, nCmdShow, className);
        auto input = std::make_unique<Input::WindowsManager>(window->Hwnd());
        auto backbufferSize = ScreenDimensions(window->ClientSize().width, window->ClientSize().height);
        auto graphics = std::make_unique<Render::DirectX9::GraphicsManager>
        (
            reliquary,
            window->Hwnd(),
            backbufferSize,
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