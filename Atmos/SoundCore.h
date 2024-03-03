#pragma once

#include "ArcaShardIncludes.h"

#include "SoundResource.h"
#include "AudioAsset.h"
#include "Volume.h"

#include <Arca/Serialization.h>

namespace Atmos::Audio
{
    class SoundCore
    {
    public:
        Arca::Index<Asset::Audio> asset;
        Volume volume = 0;
    public:
        using ResourceT = Resource::Sound;
        using ResourcePtr = std::unique_ptr<ResourceT>;
        ResourcePtr resource;
    public:
        SoundCore() = default;
        SoundCore(ResourcePtr&& resource, Arca::Index<Asset::Audio> asset, Volume volume);
        SoundCore(Arca::Serialization serialization);
        SoundCore(SoundCore&& arg) noexcept;

        SoundCore& operator=(SoundCore&& arg) noexcept;

        bool operator==(const SoundCore& arg) const;
        bool operator!=(const SoundCore& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::SoundCore>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static TypeName TypeName() { return "Atmos::Audio::SoundCore"; }
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Audio::SoundCore> final
    {
    public:
        using ObjectT = Atmos::Audio::SoundCore;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("asset", object.asset);
            archive("volume", object.volume);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Audio::SoundCore, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Audio::SoundCore>;
    };
}