#pragma once

#include <Arca/SignalTraits.h>
#include "Volume.h"

namespace Atmos::Audio
{
    struct ChangeMasterVolume
    {
        Volume to;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::ChangeMasterVolume>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}