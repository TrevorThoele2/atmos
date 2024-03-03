#include "Sound.h"

#include "CreateSoundResource.h"

namespace Atmos::Audio
{
    Sound::~Sound() = default;

    Sound& Sound::operator=(Sound&& arg) noexcept
    {
        core = std::move(arg.core);
        return *this;
    }

    Arca::Index<Asset::Audio> Sound::Asset() const
    {
        return core->asset;
    }

    Volume Sound::Volume() const
    {
        return core->volume;
    }

    auto Sound::Resource() -> ResourceT*
    {
        return core->resource.get();
    }

    auto Sound::Resource() const -> const ResourceT*
    {
        return core->resource.get();
    }

    bool Sound::ContainsResource() const
    {
        return core->resource.get();
    }

    Sound::Sound(Arca::RelicInit init, Arca::Index<Asset::Audio> asset, Audio::Volume volume) : init(init)
    {
        auto resource = init.owner.Do(Resource::CreateSound{ asset, volume });
        core = init.Create<SoundCore>(std::move(resource), asset, volume);
    }

    Sound::Sound(Arca::RelicInit init, Arca::Serialization serialization) : init(init)
    {}

    Sound::Sound(Sound&& arg) noexcept :
        core(std::move(arg.core)),
        init(std::move(arg.init))
    {}
}