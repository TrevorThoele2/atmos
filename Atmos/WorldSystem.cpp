#include "WorldSystem.h"

namespace Atmos
{
    WorldSystem::WorldSystem(ObjectManager& manager) : UnownedProviderSystem(manager)
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::WorldSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}