#pragma once

#include <Arca/ReliquaryOrigin.h>

#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"
#include "ScriptManager.h"
#include "WindowBase.h"
#include "ImageAssetManager.h"

namespace Atmos
{
    void RegisterArcaTypes(Arca::ReliquaryOrigin& origin);

    void RegisterCommonTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::ImageManager& imageAssetManager,
        Logging::Logger& logger);

    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::ImageManager& imageAssetManager,
        Logging::Logger& logger);
    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Asset::ImageManager& imageAssetManager,
        Audio::AudioManager& audio,
        Input::Manager& input,
        Render::GraphicsManager& graphics,
        Scripting::Manager& scripts,
        Spatial::ScreenSize screenSize,
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
            Arca::ReliquaryOrigin& origin, GraphicsManager& manager, Spatial::ScreenSize screenSize, void* window);
        Arca::Stage Stage();
    }

    namespace Audio
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        void RegisterTypes(Arca::ReliquaryOrigin& origin, AudioManager& manager);
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
        void RegisterTypes(Arca::ReliquaryOrigin& origin, ImageManager& manager);
    }

    namespace Entity
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
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

    namespace Logging
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Logger& logger);
    }

    namespace Debug
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }
}
