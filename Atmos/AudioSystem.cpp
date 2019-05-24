
#include "AudioSystem.h"

#include "NullAudio.h"

namespace Atmos
{
    AudioSystem::AudioSystem(ObjectManager& manager) :
        UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullAudioManager()))
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(AudioSystem) :
        INSCRIPTION_TABLE_GET_BASE(UniqueProviderSystem<AudioManager>)
    {}
}

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::AudioSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::UniqueProviderSystem<::Atmos::AudioManager>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::AudioSystem, "AudioSystem");
}