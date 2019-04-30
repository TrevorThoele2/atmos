
#include "MusicSystem.h"

namespace Atmos
{
    MusicSystem::MusicSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    MusicSystem::MusicSystem(const ::Inscription::Table<MusicSystem>& table) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

    void MusicSystem::BeginPlaying(const FileName& fileName)
    {

    }

    void MusicSystem::TerminateCurrent()
    {

    }

    void MusicSystem::ResumeCurrent()
    {

    }

    void MusicSystem::PauseCurrent()
    {

    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::MusicSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::MusicSystem, "MusicSystem");
}