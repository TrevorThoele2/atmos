
#include "SoundBase.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(SoundBase)
    {
        if (scribe.IsOutput())
        {
            scribe.Save(assetInstance);
            scribe.Save(assetInstance.Loop());
        }
        else
        {
            scribe.Load(assetInstance);

            bool loop = false;
            scribe.Load(loop);
            assetInstance.Loop(loop);
        }
    }

    SoundBase::SoundBase(const AssetReference<AudioAsset> &asset, bool loop) : assetInstance(asset->MakeInstance())
    {
        Loop(loop);
        assetInstance.Resubmit();
    }

    SoundBase::SoundBase(const SoundBase &arg) : assetInstance(arg.assetInstance)
    {
        assetInstance.Resubmit();
    }

    SoundBase::SoundBase(SoundBase &&arg) : assetInstance(std::move(arg.assetInstance))
    {
        assetInstance.Resubmit();
    }

    void SoundBase::SetVolumeImpl(Volume set)
    {
        assetInstance.SetVolume(set);
    }

    void SoundBase::StartAsset()
    {
        assetInstance.Start();
    }

    void SoundBase::StopAsset()
    {
        assetInstance.Stop();
    }

    SoundBase& SoundBase::operator=(const SoundBase &arg)
    {
        assetInstance = arg.assetInstance;
        assetInstance.Resubmit();
        return *this;
    }

    SoundBase& SoundBase::operator=(SoundBase &&arg)
    {
        assetInstance = std::move(arg.assetInstance);
        return *this;
    }

    bool SoundBase::operator==(const SoundBase &arg) const
    {
        return this == &arg;
    }

    void SoundBase::Play()
    {
        PlayImpl();
    }

    void SoundBase::Resume()
    {
        StartAsset();
    }

    void SoundBase::Stop()
    {
        StopImpl();
    }

    void SoundBase::Pause()
    {
        StopAsset();
    }

    void SoundBase::Loop(bool loop)
    {
        assetInstance.Loop(loop);
    }

    bool SoundBase::IsPlaying() const
    {
        return assetInstance.IsPlaying();
    }

    void SoundBase::SetAsset(const AudioAsset &asset)
    {
        assetInstance = asset.MakeInstance();
        assetInstance.Resubmit();
    }

    void SoundBase::SetAsset(AudioAsset::Instance &&instance)
    {
        assetInstance = std::move(instance);
        assetInstance.Resubmit();
    }

    const AudioAsset& SoundBase::GetAsset() const
    {
        return assetInstance.GetOriginal();
    }

    const FileName& SoundBase::GetFileName() const
    {
        return assetInstance.GetOriginal().GetFileName();
    }
}