#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "AudioAssetInstance.h"

namespace Atmos::Audio
{
    class Music final : public Arca::ClosedTypedRelic<Music>
    {
    public:
        using AssetInstance = Asset::AudioInstance;
        AssetInstance audioAsset;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Audio::Music>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Audio::Music";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::Music, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Audio::Music, BinaryArchive>
    {};
}