#pragma once

#include "UnownedProviderSystem.h"
#include "WorldManager.h"

namespace Atmos
{
    class WorldSystem : public UnownedProviderSystem<WorldManager>
    {
    public:
        WorldSystem(ObjectManager& manager);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::WorldSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::WorldSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}
