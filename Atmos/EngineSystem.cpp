
#include "EngineSystem.h"

namespace Atmos
{
    EngineSystem::EngineSystem(ObjectManager& manager) : UnownedProviderSystem(manager)
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::EngineSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}