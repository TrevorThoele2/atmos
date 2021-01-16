#pragma once

#include <memory>

#include "AssetResourceManager.h"
#include "WindowBase.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"
#include "ScriptManager.h"
#include "WorldManager.h"

namespace Atmos
{
    class EngineInitializationProperties
    {
    public:
        using AssetResourceManager = std::unique_ptr<Asset::Resource::Manager>;
        using Window = std::unique_ptr<Window::WindowBase>;
        using InputManager = std::unique_ptr<Input::Manager>;
        using GraphicsManager = std::unique_ptr<Render::GraphicsManager>;
        using AudioManager = std::unique_ptr<Audio::Manager>;
        using ScriptManager = std::unique_ptr<Scripting::Manager>;
        using WorldManager = std::unique_ptr<World::Manager>;

        AssetResourceManager assetResourceManager;
        Window window;
        InputManager inputManager;
        GraphicsManager graphicsManager;
        AudioManager audioManager;
        ScriptManager scriptManager;
        WorldManager worldManager;
    };
}