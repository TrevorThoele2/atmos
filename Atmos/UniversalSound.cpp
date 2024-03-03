#include "UniversalSound.h"

namespace Atmos::Audio
{
    UniversalSound::UniversalSound(
        Arca::RelicInit init,
        Arca::Index<Asset::Audio> asset,
        Audio::Volume volume)
        :
        Sound(init, asset, volume)
    {}

    UniversalSound::UniversalSound(Arca::RelicInit init, Arca::Serialization serialization) :
        Sound(init, serialization)
    {}

    UniversalSound::UniversalSound(UniversalSound&& arg) noexcept :
        Sound(std::move(arg))
    {}

    UniversalSound& UniversalSound::operator=(UniversalSound&& arg) noexcept
    {
        Sound::operator=(std::move(arg));
        return *this;
    }

    bool UniversalSound::operator==(const UniversalSound& arg) const
    {
        return Sound::operator==(arg);
    }

    bool UniversalSound::operator!=(const UniversalSound& arg) const
    {
        return !(*this == arg);
    }
}