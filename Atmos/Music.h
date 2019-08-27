#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "AudioAssetInstance.h"

namespace Atmos::Audio
{
    class Music
    {
    public:
        using AssetInstance = Asset::AudioAssetInstance;
        AssetInstance audioAsset;
    public:
        Music() = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Audio::Music>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::Music, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Audio::Music, BinaryArchive>
    {};
}