#pragma once

#include <memory>

#include "WindowBase.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"
#include "FileManager.h"

namespace Atmos
{
    class EngineInitializationProperties
    {
    public:
        typedef std::unique_ptr<Window::WindowBase> Window;
        typedef std::unique_ptr<Input::Manager> InputManager;
        typedef std::unique_ptr<Render::GraphicsManager> GraphicsManager;
        typedef std::unique_ptr<Audio::AudioManager> AudioManager;
        typedef std::unique_ptr<File::FileManager> FileManager;

        Window window;
        InputManager inputManager;
        GraphicsManager graphicsManager;
        AudioManager audioManager;
        FileManager fileSystemManager;
    };
}