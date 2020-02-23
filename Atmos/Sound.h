#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "Bounds.h"
#include "AudioAssetInstance.h"

namespace Atmos::Audio
{
    class Sound final : public Arca::ClosedTypedRelic<Sound>
    {
    public:
        using Asset = Asset::AudioAssetInstance;
    public:
        void PostConstruct();
        void Initialize(Asset&& asset);
    private:
        Arca::Index<Bounds> bounds;

        Asset asset;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Audio::Sound>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Sound";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::Sound, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Audio::Sound, BinaryArchive>
    {};
}