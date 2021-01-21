#pragma once

#include "SoundResource.h"

#include "SDLChannel.h"
#include <SDL_mixer.h>

namespace Atmos::Audio::Resource::SDL
{
    class Sound final : public Resource::Sound
    {
    public:
        using Channel = Audio::SDL::Channel;
    public:
        Sound(SDL_RWops* ops, Channel channel, Volume volume);
        Sound(const Sound& arg) = delete;
        Sound(Sound&& arg) noexcept;
        ~Sound();

        Sound& operator=(const Sound& arg) = delete;
        Sound& operator=(Sound&& arg) noexcept;

        void Restart() override;
        void Pause() override;
        void Resume() override;
        void SetVolume(Volume volume) override;
        void SetPosition(const Spatial::Point2D& position) override;
    private:
        Mix_Chunk* chunk = nullptr;
        Channel channel = 0;

        const float minMixDistance = 1;
        const float maxMixDistance = 255;
        const float minMixVolume = 0;
        const float maxMixVolume = MIX_MAX_VOLUME;

        void LoadChunk(SDL_RWops* ops);
        void FreeChunk();
        void StartChunk();
        void StopChunk();
        void PauseChunk();
        void ResumeChunk();
        void SetChunkVolume(Volume volume);
        void SetChunkPosition(const Spatial::Point2D& position);
    };
}