#include "WindowsEngine.h"

#include "WindowsWindow.h"
#include "WindowsInputManager.h"
#include "DirectX9GraphicsManager.h"
#include "DirectX9AudioManager.h"
#include "WindowsFileManager.h"

namespace Atmos
{
    WindowsEngine::WindowsEngine(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, const String& className) :
        hInstance(hInstance), lpCmdLine(lpCmdLine), nCmdShow(nCmdShow), className(className)
    {}

    WindowsEngine::InitializationProperties WindowsEngine::CreateInitializationProperties(ObjectManager& globalObjectManager)
    {
        typedef InitializationProperties Properties;

        auto window = new Window::WindowsWindow(globalObjectManager, hInstance, lpCmdLine, nCmdShow, className);
        auto input = new Input::WindowsManager();
        auto backbufferSize = ScreenDimensions(window->ClientSize().width, window->ClientSize().height);
        auto graphics = new Render::DirectX9GraphicsManager(
            globalObjectManager,
            window->GetHwnd(),
            backbufferSize,
            window->IsFullscreen());
        auto audio = new Audio::DirectX9AudioManager();
        auto fileSystem = new File::WindowsFileManager();

        Properties properties;
        properties.window = Properties::Window(window);
        properties.inputManager = Properties::InputManager(input);
        properties.graphicsManager = Properties::GraphicsManager(graphics);
        properties.audioManager = Properties::AudioManager(audio);
        properties.fileSystemManager = Properties::FileManager(fileSystem);
        return properties;
    }

    void WindowsEngine::DoExit()
    {
        PostQuitMessage(0);
    }
}