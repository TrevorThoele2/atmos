
#include "PositionalSystem.h"

namespace Atmos
{
    PositionalSystem::PositionalSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        batch = manager.Batch<PositionalObject>();
        batch.onCreated.Subscribe(&PositionalSystem::OnCreated, *this);
    }

    void PositionalSystem::OnCreated(Reference object)
    {
        grid.Add(object);

        object->onBoundsChanged.Subscribe(&PositionalSystem::OnObjectBoundsChanged, *this);
    }

    void PositionalSystem::OnObjectBoundsChanged(Reference object, AxisBoundingBox3D previous)
    {
        grid.InformChanged(object, previous);
    }
}