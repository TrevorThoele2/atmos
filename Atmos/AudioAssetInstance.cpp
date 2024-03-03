#include "AudioAssetInstance.h"

#include "AudioAssetInstanceResource.h"

namespace Atmos::Asset
{
    AudioInstance::AudioInstance(ResourcePtr&& resource) :
        resource(std::move(resource))
    {}

    AudioInstance::AudioInstance(const AudioInstance& arg) :
        resource(arg.resource->Clone()), volume(arg.volume), isLooping(arg.isLooping)
    {}

    AudioInstance& AudioInstance::operator=(const AudioInstance& arg)
    {
        resource = arg.resource->Clone();
        volume = arg.volume;
        isLooping = arg.isLooping;
        return *this;
    }

    void AudioInstance::Start()
    {
        resource->Start();
    }

    void AudioInstance::Stop()
    {
        resource->Stop();
    }

    bool AudioInstance::IsPlaying() const
    {
        return isPlaying;
    }

    void AudioInstance::Loop(bool set)
    {
        isLooping = set;
        resource->Loop(set);
    }

    bool AudioInstance::IsLooping() const
    {
        return isLooping;
    }

    void AudioInstance::ChangeVolume(Atmos::Audio::Volume volume)
    {
        this->volume = volume;
        resource->SetVolume(this->volume);
    }

    Atmos::Audio::Volume AudioInstance::Volume() const
    {
        return volume;
    }
}