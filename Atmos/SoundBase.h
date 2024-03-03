#pragma once

#include "AudioAsset.h"
#include "AssetReference.h"

#include "FilePath.h"

#include "Serialization.h"

namespace Atmos
{
    template<class T>
    class AssetRegistry;

    class SoundBase
    {
    public:
        typedef float Volume;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        friend AssetRegistry<AudioAsset>;
    private:
        AudioAsset::Instance assetInstance;
    protected:
        SoundBase() = default;
        SoundBase(const AssetReference<AudioAsset> &asset, bool loop = false);
        SoundBase(const SoundBase &arg);
        SoundBase(SoundBase &&arg);
        void SetVolumeImpl(Volume set);

        void StartAsset();
        void StopAsset();

        virtual void PlayImpl() = 0;
        virtual void StopImpl() = 0;
    public:
        virtual ~SoundBase() = 0 {}
        SoundBase& operator=(const SoundBase &arg);
        SoundBase& operator=(SoundBase &&arg);
        bool operator==(const SoundBase &arg) const;

        void Play();
        void Resume();
        void Stop();
        void Pause();
        void Loop(bool loop);

        bool IsPlaying() const;

        void SetAsset(const AudioAsset &asset);
        void SetAsset(AudioAsset::Instance &&instance);
        const AudioAsset& GetAsset() const;
        const FileName& GetFileName() const;
    };
}