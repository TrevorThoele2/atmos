#pragma once

#include "UniqueProviderSystem.h"
#include "AudioManager.h"

namespace Atmos
{
    class AudioSystem : public UniqueProviderSystem<AudioManager>
    {
    public:
        AudioSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(AudioSystem);
    };

    template<>
    struct ObjectSystemTraits<AudioSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::AudioSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}