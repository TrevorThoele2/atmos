#pragma once

#include <memory>

#include "System.h"
#include "AssetResourceManager.h"
#include "WindowBase.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "TextManager.h"
#include "AudioManager.h"
#include "ScriptManager.h"
#include "WorldManager.h"

namespace Atmos
{
    class EngineInitializationProperties
    {
    public:
        using System = std::unique_ptr<System::System>;
        using AssetResourceManager = std::unique_ptr<Asset::Resource::Manager>;
        using Window = std::unique_ptr<Window::WindowBase>;
        using InputManager = std::unique_ptr<Input::Manager>;
        using GraphicsManager = std::unique_ptr<Render::GraphicsManager>;
        using TextManager = std::unique_ptr<Render::TextManager>;
        using AudioManager = std::unique_ptr<Audio::Manager>;
        using ScriptManager = std::unique_ptr<Scripting::Manager>;
        using WorldManager = std::unique_ptr<World::Manager>;

        System system;
        AssetResourceManager assetResourceManager;
        Window window;
        InputManager inputManager;
        GraphicsManager graphicsManager;
        TextManager textManager;
        AudioManager audioManager;
        ScriptManager scriptManager;
        WorldManager worldManager;
    };
}