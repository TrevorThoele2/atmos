#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "Bounds.h"
#include "AudioAssetInstance.h"

namespace Atmos::Audio
{
    class Sound final : public Arca::ClosedTypedRelicAutomation<Sound, Bounds>
    {
    public:
        using Asset = Asset::AudioAssetInstance;
    public:
        void PostConstruct(ShardTuple shards);
        void Initialize(Asset&& asset);
    private:
        Arca::Ptr<Bounds> bounds;

        Asset asset;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Audio::Sound>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::Sound, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Audio::Sound, BinaryArchive>
    {};
}