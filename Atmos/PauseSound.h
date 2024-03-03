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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Audio::PauseSound";
    };
}