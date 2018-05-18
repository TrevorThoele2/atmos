
#include "Music.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Music)
    {
        ::inscription::BaseSerialize<SoundBase>(scribe, *this);
    }

    void Music::PlayImpl()
    {
        Resume();
    }

    void Music::StopImpl()
    {
        Pause();
    }

    Music::Music()
    {}

    Music::Music(const AssetReference<AudioAsset> &asset) : SoundBase(asset, true)
    {}

    Music::Music(const Music &arg) : SoundBase(arg)
    {}

    Music::Music(Music &&arg) : SoundBase(std::move(arg))
    {}

    Music& Music::operator=(const Music &arg)
    {
        SoundBase::operator=(arg);
        return *this;
    }

    Music& Music::operator=(Music &&arg)
    {
        SoundBase::operator=(std::move(arg));
        return *this;
    }
}