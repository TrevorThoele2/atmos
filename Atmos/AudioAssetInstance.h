#pragma once

#include <memory>

#include "Volume.h"

namespace Atmos::Asset
{
    class AudioAsset;
    class AudioAssetInstanceData;

    class AudioAssetInstance
    {
    public:
        using DataPtr = std::unique_ptr<AudioAssetInstanceData>;
    public:
        AudioAssetInstance() = default;
        AudioAssetInstance(const AudioAsset& audioAsset, DataPtr&& data);
        AudioAssetInstance(const AudioAssetInstance& arg);
        AudioAssetInstance(AudioAssetInstance&& arg) = default;

        AudioAssetInstance& operator=(const AudioAssetInstance& arg);
        AudioAssetInstance& operator=(AudioAssetInstance&& arg) = default;

        void Start();
        void Stop();

        [[nodiscard]] bool IsPlaying() const;

        void Loop(bool set = true);
        [[nodiscard]] bool IsLooping() const;

        void ChangeVolume(Audio::Volume volume);
        [[nodiscard]] Audio::Volume Volume() const;
    private:
        DataPtr data;

        Audio::Volume volume = 0;
        bool isLooping = false;
    private:
        bool isPlaying = false;
    };

    class AudioAssetInstanceData
    {
    public:
        virtual ~AudioAssetInstanceData() = 0;
    private:
        [[nodiscard]] virtual std::unique_ptr<AudioAssetInstanceData> Clone() const = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual void SetVolume(Audio::Volume set) = 0;
        virtual void Loop(bool set) = 0;
        virtual void Resubmit() = 0;
    private:
        friend AudioAssetInstance;
    };
}