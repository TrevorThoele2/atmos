#include "MusicSystem.h"

namespace Atmos::Audio
{
    MusicSystem::MusicSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    void MusicSystem::BeginPlaying(const File::Name& fileName)
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
    void Scribe<::Atmos::Audio::MusicSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}