#include "GraphicsSystem.h"

#include "NullGraphics.h"

namespace Atmos::Render
{
    GraphicsSystem::GraphicsSystem(ObjectManager& manager) :
        UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullGraphicsManager(manager)))
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::GraphicsSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}