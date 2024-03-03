#pragma once

#include "AssetResourceManager.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "TextManager.h"
#include "ScriptManager.h"
#include "WindowBase.h"
#include "Logger.h"

namespace Atmos::World
{
    struct FieldInitialization
    {
        using AssetResourceManager = Asset::Resource::Manager;
        using AudioManager = Audio::Manager;
        using InputManager = Input::Manager;
        using GraphicsManager = Render::GraphicsManager;
        using TextManager = Render::TextManager;
        using ScriptManager = Scripting::Manager;
        using Window = Window::WindowBase;

        AssetResourceManager* assetResourceManager = nullptr;
        AudioManager* audioManager = nullptr;
        InputManager* inputManager = nullptr;
        GraphicsManager* graphicsManager = nullptr;
        TextManager* textManager = nullptr;
        ScriptManager* scriptManager = nullptr;
        Window* window = nullptr;
        Logging::Logger* logger = nullptr;
        File::Path assetsFilePath;
    };
}