#pragma once

#include "UnownedProviderSystem.h"
#include "WorldManager.h"

namespace Atmos::World
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
    class Scribe<::Atmos::World::WorldSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::World::WorldSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}
