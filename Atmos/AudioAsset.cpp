
#include <vorbis/vorbisfile.h>
#include <XAudio2.h>
#include "AudioAsset.h"

#include "Environment.h"
#include "DXAudio.h"

#include "SimpleFile.h"
#include "Buffer.h"

#include <Inscription\Inscripter.h>
#include <Inscription\Table.h>

namespace Atmos
{
    void AudioAsset::Instance::Serialize(::Inscription::Scribe &scribe)
    {
        scribe.AssumeExists(::Inscription::RemoveConst(asset));
        if (scribe.IsInput())
            *this = asset->MakeInstance();
    }

    AudioAsset::Instance* AudioAsset::Instance::Data::GetOwner() const
    {
        return owner;
    }

    void AudioAsset::Instance::SetData(Data *set)
    {
        data.reset(set);
        data->owner = this;
    }

    void AudioAsset::Instance::SetData(std::unique_ptr<Data> &&set)
    {
        data = std::move(set);
        data->owner = this;
    }

    AudioAsset::Instance::Instance() : asset(nullptr)
    {}

    AudioAsset::Instance::Instance(const AudioAsset &asset, Data *data) : asset(&asset)
    {
        SetData(data);
    }

    AudioAsset::Instance::Instance(const Instance &arg) : asset(arg.asset)
    {
        SetData(arg.data->Clone());
    }

    AudioAsset::Instance::Instance(Instance &&arg) : asset(arg.asset)
    {
        SetData(std::move(arg.data));
    }

    AudioAsset::Instance& AudioAsset::Instance::operator=(const Instance &arg)
    {
        asset = arg.asset;
        SetData(std::move(arg.data->Clone()));
        return *this;
    }

    AudioAsset::Instance& AudioAsset::Instance::operator=(Instance &&arg)
    {
        asset = arg.asset;
        SetData(std::move(arg.data));
        return *this;
    }

    void AudioAsset::Instance::Start()
    {
        data->Start();
    }

    void AudioAsset::Instance::Stop()
    {
        data->Stop();
    }

    void AudioAsset::Instance::SetVolume(Volume set)
    {
        data->SetVolume(set);
    }

    void AudioAsset::Instance::Loop(bool set)
    {
        data->Loop(set);
    }

    bool AudioAsset::Instance::Loop() const
    {
        return data->Loop();
    }

    bool AudioAsset::Instance::IsPlaying() const
    {
        return data->IsPlaying();
    }

    void AudioAsset::Instance::Resubmit()
    {
        data->Resubmit();
    }

    const AudioAsset& AudioAsset::Instance::GetOriginal() const
    {
        return *asset;
    }

    AudioAsset* AudioAsset::Data::GetOwner() const
    {
        return owner;
    }

    void AudioAsset::SetData(Data *set)
    {
        data.reset(set);
        data->owner = this;
    }

    void AudioAsset::SetData(std::unique_ptr<Data> &&set)
    {
        data = std::move(set);
        data->owner = this;
    }

    String AudioAsset::GetStringImpl() const
    {
        return name.GetValue();
    }

    AudioAsset::AudioAsset(Data *data, const FileName &name) : name(name)
    {
        SetData(data);
    }

    AudioAsset::AudioAsset(AudioAsset &&arg) : name(std::move(arg.name))
    {
        SetData(std::move(arg.data));
    }

    AudioAsset& AudioAsset::operator=(AudioAsset &&arg)
    {
        SetData(std::move(arg.data));
        name = std::move(arg.name);
        return *this;
    }

    bool AudioAsset::operator==(const AudioAsset &arg) const
    {
        return Asset::operator==(arg);
    }

    bool AudioAsset::operator!=(const AudioAsset &arg) const
    {
        return !(*this == arg);
    }

    AudioAsset::Instance AudioAsset::MakeInstance() const
    {
        return data->MakeInstance(*this);
    }

    const FileName& AudioAsset::GetFileName() const
    {
        return name;
    }
}