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

    namespace Spatial
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace File
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

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

    namespace Asset
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Time
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }

    namespace Debug
    {
        void RegisterTypes(Arca::ReliquaryOrigin& origin);
    }
}
