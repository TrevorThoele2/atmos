#include "AudioSystem.h"

#include "NullAudio.h"

namespace Atmos
{
    AudioSystem::AudioSystem(ObjectManager& manager) :
        UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullAudioManager()))
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::AudioSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::AudioSystem>(object, archive);
    }
}