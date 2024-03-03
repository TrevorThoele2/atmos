#pragma once

#include <Arca/ReliquaryOrigin.h>

#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

namespace Atmos
{
    void RegisterCommonTypes(Arca::ReliquaryOrigin& origin);

    void RegisterFieldTypes(Arca::ReliquaryOrigin& origin);
    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Audio::AudioManager& audio,
        Input::Manager& input,
        Render::GraphicsManager& graphics,
        Spatial::ScreenSize screenSize,
        void* window);

    void RegisterFieldStages(Arca::ReliquaryOrigin& origin);

    namespace Input
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager);
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
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
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
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Entity
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Script
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Frame
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
        Arca::Stage StartStage();
        Arca::Stage EndStage();
    }

    namespace Logging
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Debug
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }
}
