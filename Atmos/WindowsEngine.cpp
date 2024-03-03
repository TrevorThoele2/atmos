
#include "WindowsEngine.h"

#include "WindowsWindow.h"
#include "WindowsInputManager.h"
#include "DXGraphics.h"
#include "DXAudio.h"
#include "WindowsFileManager.h"

namespace Atmos
{
    WindowsEngine::WindowsEngine(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, const String& className) :
        hInstance(hInstance), lpCmdLine(lpCmdLine), nCmdShow(nCmdShow), className(className)
    {}

    WindowsEngine::CreatedContext WindowsEngine::CreateDefaultContext()
    {
        auto window = new WindowsWindow(globalObjectManager, hInstance, lpCmdLine, nCmdShow, className);
        auto input = new Input::WindowsManager();
        auto backbufferSize = ScreenDimensions(window->ClientSize().width, window->ClientSize().height);
        auto graphics = new DX9GraphicsManager(globalObjectManager, window->GetHwnd(), backbufferSize, window->IsFullscreen());
        auto audio = new DX9AudioManager();
        auto fileSystem = new WindowsFileManager();

        CreatedContext context;
        context.window = CreatedContext::Window(window);
        context.inputManager = CreatedContext::InputManager(input);
        context.graphicsManager = CreatedContext::GraphicsManager(graphics);
        context.audioManager = CreatedContext::AudioManager(audio);
        context.fileSystemManager = CreatedContext::FileManager(fileSystem);
        return context;
    }

    void WindowsEngine::DoExit()
    {
        PostQuitMessage(0);
    }
}