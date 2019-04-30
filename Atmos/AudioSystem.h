#pragma once

#include "UniqueProviderSystem.h"
#include "AudioManager.h"

namespace Atmos
{
    class AudioSystem : public UniqueProviderSystem<AudioManager>
    {
    public:
        AudioSystem(ObjectManager& manager);
        AudioSystem(const ::Inscription::Table<AudioSystem>& table);
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
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}