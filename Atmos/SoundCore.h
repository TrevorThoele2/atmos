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
        static constexpr ObjectType objectType = ObjectType::Shard;
        static const inline TypeName typeName = "Atmos::Audio::SoundCore";
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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("asset", object.asset);
            format("volume", object.volume);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Audio::SoundCore, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Audio::SoundCore>;
    };
}