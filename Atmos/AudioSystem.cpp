#include "AudioSystem.h"

#include "NullAudioManager.h"

namespace Atmos::Audio
{
    AudioSystem::AudioSystem(ObjectManager& manager) :
        UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullAudioManager()))
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::Audio::AudioSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::UniqueProviderSystem<::Atmos::Audio::AudioManager>>(object, archive);
    }
}