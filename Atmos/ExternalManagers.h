#pragma once

namespace Atmos::Audio
{
    class AudioManager;
}

namespace Atmos::Input
{
    class Manager;
}

namespace Atmos::Render
{
    class GraphicsManager;
}

namespace Atmos
{
    struct ExternalManagers
    {
        Audio::AudioManager* audio;
        Input::Manager* input;
        Render::GraphicsManager* graphics;

        ExternalManagers(Audio::AudioManager& audio, Input::Manager& input, Render::GraphicsManager& graphics) :
            audio(&audio), input(&input), graphics(&graphics)
        {}
    };
}