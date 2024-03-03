#pragma once

#include <Arca/Signal.h>
#include "AudioAsset.h"
#include "SoundResource.h"

namespace Atmos::Audio::Resource
{
    struct CreateSound
    {
        Arca::Index<Asset::Audio> asset;
        Volume volume;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::Resource::CreateSound>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Audio::Resource::CreateSound"; }
        using Result = std::unique_ptr<Atmos::Audio::Resource::Sound>;
    };
}