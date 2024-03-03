#pragma once

#include <Arca/Relic.h>

#include "Bounds.h"
#include "AudioAssetInstance.h"

namespace Atmos::Audio
{
    class Sound final
    {
    public:
        using Asset = Asset::AudioInstance;
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
        static TypeName TypeName() { return "Atmos::Audio::Sound"; }
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