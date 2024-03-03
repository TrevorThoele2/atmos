#include "AudioAssetInstance.h"

namespace Atmos::Asset
{
    AudioAssetInstance::AudioAssetInstance(const AudioAsset& audioAsset, DataPtr&& data) :
        data(std::move(data)), isPlaying(false)
    {
        SubscribeToProperties();
    }

    AudioAssetInstance::AudioAssetInstance(const AudioAssetInstance& arg) : data(arg.data->Clone()), isPlaying(false)
    {
        SubscribeToProperties();
    }

    void AudioAssetInstance::Start()
    {
        data->Start();
    }

    void AudioAssetInstance::Stop()
    {
        data->Stop();
    }

    bool AudioAssetInstance::IsPlaying() const
    {
        return isPlaying;
    }

    void AudioAssetInstance::SubscribeToProperties()
    {
        volume.onValueChanged.Subscribe([this](Audio::Volume newValue)
        {
            this->OnVolumeChanged(newValue);
        });

        volume.onValueChanged.Subscribe([this](bool newValue)
        {
            this->OnLoopChanged(newValue);
        });
    }

    void AudioAssetInstance::OnVolumeChanged(Audio::Volume newValue)
    {
        data->SetVolume(newValue);
    }

    void AudioAssetInstance::OnLoopChanged(bool newValue)
    {
        data->Loop(newValue);
    }

    AudioAssetInstanceData::~AudioAssetInstanceData()
    {}
}