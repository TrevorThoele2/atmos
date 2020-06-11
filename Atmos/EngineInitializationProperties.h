#pragma once

#include <memory>

#include "WindowBase.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

namespace Atmos
{
    class EngineInitializationProperties
    {
    public:
        using Window = std::unique_ptr<Window::WindowBase>;
        using InputManager = std::unique_ptr<Input::Manager>;
        using GraphicsManager = std::unique_ptr<Render::GraphicsManager>;
        using AudioManager = std::unique_ptr<Audio::AudioManager>;

        Window window;
        InputManager inputManager;
        GraphicsManager graphicsManager;
        AudioManager audioManager;
    };
}