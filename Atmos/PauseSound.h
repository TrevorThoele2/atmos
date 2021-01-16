#pragma once

#include <Arca/Signal.h>
#include <Arca/RelicID.h>

namespace Atmos::Audio
{
    struct PauseSound
    {
        Arca::RelicID id;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::PauseSound>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Audio::PauseSound"; }
    };
}