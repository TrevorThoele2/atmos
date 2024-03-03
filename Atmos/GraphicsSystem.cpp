#include "GraphicsSystem.h"

#include "NullGraphicsManager.h"

namespace Atmos::Render
{
    GraphicsSystem::GraphicsSystem(ObjectManager& manager) :
        UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullGraphicsManager(manager)))
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::Render::GraphicsSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}