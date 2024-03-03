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
        static constexpr ObjectType objectType = ObjectType::Signal;
        static const inline TypeName typeName = "Atmos::Audio::SoundFinished";
    };
}