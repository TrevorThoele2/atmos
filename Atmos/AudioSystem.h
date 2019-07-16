#pragma once

#include "UniqueProviderSystem.h"
#include "AudioManager.h"

namespace Atmos
{
    class AudioSystem : public UniqueProviderSystem<AudioManager>
    {
    public:
        AudioSystem(ObjectManager& manager);
    };

    template<>
    struct ObjectSystemTraits<AudioSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::AudioSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::AudioSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}