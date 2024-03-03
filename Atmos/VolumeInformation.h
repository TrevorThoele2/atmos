#pragma once

#include <Arca/Relic.h>
#include "Volume.h"

namespace Atmos::Audio
{
    class VolumeInformation final
    {
    public:
        Volume master;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::VolumeInformation>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Audio::VolumeInformation"; }
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Audio::VolumeInformation, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Audio::VolumeInformation>;
    };
}