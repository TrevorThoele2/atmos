#pragma once

#include <memory>

#include "AudioAssetInstanceResource.h"
#include "Volume.h"

namespace Atmos::Asset
{
    class AudioInstance
    {
    public:
        using ResourcePtr = std::unique_ptr<Resource::AudioInstance>;
    public:
        AudioInstance() = default;
        AudioInstance(ResourcePtr&& resource);
        AudioInstance(const AudioInstance& arg);
        AudioInstance(AudioInstance&& arg) = default;

        AudioInstance& operator=(const AudioInstance& arg);
        AudioInstance& operator=(AudioInstance&& arg) = default;

        void Start();
        void Stop();

        [[nodiscard]] bool IsPlaying() const;

        void Loop(bool set = true);
        [[nodiscard]] bool IsLooping() const;

        void ChangeVolume(Atmos::Audio::Volume volume);
        [[nodiscard]] Atmos::Audio::Volume Volume() const;
    private:
        ResourcePtr resource;

        Atmos::Audio::Volume volume = 0;
        bool isLooping = false;
    private:
        bool isPlaying = false;
    };
}