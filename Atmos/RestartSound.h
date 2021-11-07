#pragma once

#include <Arca/Signal.h>
#include <Arca/RelicID.h>

namespace Atmos::Audio
{
    struct RestartSound
    {
        Arca::RelicID id;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::RestartSound>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Audio::RestartSound";
    };
}