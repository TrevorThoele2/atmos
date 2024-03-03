#pragma once

#include <memory>

#include "WindowBase.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"
#include "FileManager.h"

namespace Atmos
{
    class CreatedEngineContext
    {
    public:
        typedef std::unique_ptr<WindowBase> Window;
        typedef std::unique_ptr<Input::Manager> InputManager;
        typedef std::unique_ptr<GraphicsManager> GraphicsManager;
        typedef std::unique_ptr<AudioManager> AudioManager;
        typedef std::unique_ptr<FileManager> FileManager;

        Window window;
        InputManager inputManager;
        GraphicsManager graphicsManager;
        AudioManager audioManager;
        FileManager fileSystemManager;
    };
}