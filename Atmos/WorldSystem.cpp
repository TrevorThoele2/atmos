#include "WorldSystem.h"

namespace Atmos::World
{
    WorldSystem::WorldSystem(ObjectManager& manager) : UnownedProviderSystem(manager)
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::World::WorldSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}