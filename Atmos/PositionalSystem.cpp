
#include "PositionalSystem.h"

namespace Atmos
{
    PositionalSystem::PositionalSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        batch = manager.Batch<AxisAlignedObject>();
        batch.onCreated.Subscribe(&PositionalSystem::OnCreated, *this);
    }

    void PositionalSystem::OnCreated(Reference object)
    {
        grid.Add(object);

        object->onBoundsChanged.Subscribe([this, object](AxisAlignedBox3D previous)
        {
            OnObjectBoundsChanged(object, previous);
        });
    }

    void PositionalSystem::OnObjectBoundsChanged(Reference object, AxisAlignedBox3D previous)
    {
        grid.InformChanged(object, previous);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::PositionalSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}