#include "SoundCore.h"

namespace Atmos::Audio
{
    SoundCore::SoundCore(ResourcePtr&& resource, Arca::Index<Asset::Audio> asset, Volume volume) :
        asset(asset), volume(volume), resource(std::move(resource))
    {}

    SoundCore::SoundCore(Arca::Serialization)
    {}

    SoundCore::SoundCore(SoundCore&& arg) noexcept :
        asset(std::move(arg.asset)),
        volume(arg.volume),
        resource(std::move(arg.resource))
    {}

    SoundCore& SoundCore::operator=(SoundCore&& arg) noexcept
    {
        resource = std::move(arg.resource);
        asset = std::move(arg.asset);
        volume = arg.volume;
        return *this;
    }

    bool SoundCore::operator==(const SoundCore& arg) const
    {
        return resource == arg.resource && IndexEqualsValue(asset, arg.asset) && volume == arg.volume;
    }

    bool SoundCore::operator!=(const SoundCore& arg) const
    {
        return !(*this == arg);
    }
}