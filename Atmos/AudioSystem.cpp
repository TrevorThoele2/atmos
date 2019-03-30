
#include "AudioSystem.h"

namespace Atmos
{
    AudioSystem::AudioSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    AudioSystem::AudioSystem(const ::Inscription::Table<AudioSystem>& table) : INSCRIPTION_TABLE_GET_BASE(AssetSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::AudioSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;
        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::AudioSystem, "AudioSystem");
}