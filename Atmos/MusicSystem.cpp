#include "MusicSystem.h"

namespace Atmos
{
    MusicSystem::MusicSystem(ObjectManager& manager) : ObjectSystem(manager)
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
    void Scribe<::Atmos::MusicSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}