#pragma once

#include "StoredProperty.h"

#include "Volume.h"

namespace Atmos
{
    class AudioAsset;
    class AudioAssetInstanceData;

    class AudioAssetInstance
    {
    public:
        typedef StoredProperty<Volume> VolumeProperty;
        VolumeProperty volume;
    public:
        typedef StoredProperty<bool> LoopProperty;
        LoopProperty loop;
    public:
        typedef std::unique_ptr<AudioAssetInstanceData> DataPtr;
    public:
        AudioAssetInstance(const AudioAsset& audioAsset, DataPtr&& data);
        AudioAssetInstance(const AudioAssetInstance& arg);

        void Start();
        void Stop();

        bool IsPlaying() const;
    private:
        DataPtr data;
    private:
        bool isPlaying;
    private:
        void SubscribeToProperties();
    private:
        void OnVolumeChanged(Volume newValue);
        void OnLoopChanged(bool newValue);
    };

    class AudioAssetInstanceData
    {
    public:
        virtual ~AudioAssetInstanceData() = 0;
    private:
        virtual std::unique_ptr<AudioAssetInstanceData> Clone() const = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual void SetVolume(Volume set) = 0;
        virtual void Loop(bool set) = 0;
        virtual void Resubmit() = 0;
    private:
        friend AudioAssetInstance;
    };
}