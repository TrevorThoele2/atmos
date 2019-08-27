#include "AudioAssetInstance.h"

namespace Atmos::Asset
{
    AudioAssetInstance::AudioAssetInstance(const AudioAsset& audioAsset, DataPtr&& data) :
        data(std::move(data))
    {}

    AudioAssetInstance::AudioAssetInstance(const AudioAssetInstance& arg) :
        data(arg.data->Clone()), volume(arg.volume), isLooping(arg.isLooping)
    {}

    AudioAssetInstance& AudioAssetInstance::operator=(const AudioAssetInstance& arg)
    {
        data = arg.data->Clone();
        volume = arg.volume;
        isLooping = arg.isLooping;
        return *this;
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

    void AudioAssetInstance::Loop(bool set)
    {
        isLooping = set;
        data->Loop(set);
    }

    bool AudioAssetInstance::IsLooping() const
    {
        return isLooping;
    }

    void AudioAssetInstance::ChangeVolume(Audio::Volume volume)
    {
        this->volume = volume;
        data->SetVolume(this->volume);
    }

    Audio::Volume AudioAssetInstance::Volume() const
    {
        return volume;
    }

    AudioAssetInstanceData::~AudioAssetInstanceData() = default;
}