#pragma once

#include <Arca/ReliquaryOrigin.h>

#include "AssetResourceManager.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "TextManager.h"
#include "AudioManager.h"
#include "ScriptManager.h"
#include "WindowBase.h"
#include "WorldManager.h"

namespace Atmos
{
    void RegisterArcaTypes(Arca::ReliquaryOrigin& origin);

    void RegisterCommonTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::Resource::Manager& assetResourceManager,
        Logging::Logger& logger);

    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::Resource::Manager& assetResourceManager,
        Logging::Logger& logger);
    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::Resource::Manager& assetResourceManager,
        Audio::Manager& audio,
        Input::Manager& input,
        Render::GraphicsManager& graphics,
        Render::TextManager& text,
        Scripting::Manager& scripts,
        World::Manager& world,
        Spatial::Size2D screenSize,
        Window::WindowBase& window,
        Logging::Logger& logger);

    void RegisterFieldStages(Arca::ReliquaryOrigin& origin);

    namespace Input
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager);
        Arca::Stage Stage();
    }

    namespace Render
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        void RegisterTypes(
            Arca::ReliquaryOrigin& origin,
            GraphicsManager& graphicsManager,
            TextManager& textManager,
            Spatial::Size2D screenSize,
            void* window);
        Arca::Stage Stage();
    }

    namespace UI
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Audio
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager);
        Arca::Stage Stage();
    }

    namespace Window
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, WindowBase& window);
    }

    namespace Spatial
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace File
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Asset
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Resource::Manager& resourceManager);
    }

    namespace Entity
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        Arca::Stage ActualizationStage();
    }

    namespace Scripting
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager);
        Arca::Stage Stage();
    }

    namespace Frame
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        Arca::Stage StartStage();
        Arca::Stage EndStage();
    }

    namespace World
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager);
    }

    namespace Data
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Logging
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Logger& logger);
    }

    namespace Debug
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Random
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }
}
