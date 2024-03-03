#pragma once

#include <Arca/ReliquaryOrigin.h>

#include "InputManager.h"
#include "GraphicsManager.h"
#include "AudioManager.h"

namespace Atmos
{
    void RegisterFieldTypes(
        Arca::ReliquaryOrigin& origin,
        Audio::AudioManager& audio,
        Input::Manager& input,
        Render::GraphicsManager& graphics);

    namespace Input
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, Manager& manager);
    }

    namespace Render
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, GraphicsManager& manager);
        Arca::Stage Stage();
    }

    namespace Audio
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin, AudioManager& manager);
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

    namespace Time
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
