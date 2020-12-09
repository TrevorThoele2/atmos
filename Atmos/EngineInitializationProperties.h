#pragma once

#include <memory>

#include "ImageAssetManager.h"
#include "WindowBase.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"
#include "ScriptManager.h"

namespace Atmos
{
    class EngineInitializationProperties
    {
    public:
        using ImageAssetManager = std::unique_ptr<Asset::ImageManager>;
        using Window = std::unique_ptr<Window::WindowBase>;
        using InputManager = std::unique_ptr<Input::Manager>;
        using GraphicsManager = std::unique_ptr<Render::GraphicsManager>;
        using AudioManager = std::unique_ptr<Audio::AudioManager>;
        using ScriptManager = std::unique_ptr<Scripting::Manager>;

        ImageAssetManager imageAssetManager;
        Window window;
        InputManager inputManager;
        GraphicsManager graphicsManager;
        AudioManager audioManager;
        ScriptManager scriptManager;
    };
}