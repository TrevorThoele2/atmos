#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "Bounds.h"
#include "AudioAssetInstance.h"

namespace Atmos::Audio
{
    class Sound final : public Arca::ClosedTypedRelic<Sound>
    {
    public:
        using Asset = Asset::AudioInstance;
    public:
        void PostConstruct();
        void Initialize(Asset&& asset);
    private:
        Arca::Index<Spatial::Bounds> bounds;

        Asset asset;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Audio::Sound>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Audio::Sound";
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Audio::Sound, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Audio::Sound>;
    };
}