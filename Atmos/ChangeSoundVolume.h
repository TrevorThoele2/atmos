#pragma once

#include <Arca/Signal.h>
#include <Arca/RelicID.h>
#include "Volume.h"

namespace Atmos::Audio
{
    struct ChangeSoundVolume
    {
        Arca::RelicID id;
        Volume to;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::ChangeSoundVolume>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Audio::ChangeSoundVolume"; }
    };
}