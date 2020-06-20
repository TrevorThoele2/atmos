#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "Volume.h"

namespace Atmos::Audio
{
    class VolumeInformation final : public Arca::ClosedTypedRelic<VolumeInformation>
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
        static inline const TypeName typeName = "Atmos::Audio::VolumeInformation";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Audio::VolumeInformation, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Audio::VolumeInformation, BinaryArchive>
    {};
}