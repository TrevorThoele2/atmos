#pragma once

#include <Arca/Signal.h>

#include "PositionedSound.h"

namespace Atmos::Audio
{
    struct SoundFinished
    {
        Arca::RelicID id;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::SoundFinished>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::Audio::SoundFinished"; }
    };
}