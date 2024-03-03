#pragma once

#include "UniqueProviderSystem.h"
#include "AudioManager.h"

namespace Atmos::Audio
{
    class AudioSystem : public UniqueProviderSystem<AudioManager>
    {
    public:
        AudioSystem(ObjectManager& manager);
    };
}

namespace Atmos
{
    template<>
    struct ObjectSystemTraits<Audio::AudioSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::AudioSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Audio::AudioSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}