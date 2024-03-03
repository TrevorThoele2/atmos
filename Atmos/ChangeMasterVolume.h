#pragma once

#include <Arca/Signal.h>
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
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Audio::ChangeMasterVolume"; }
    };
}