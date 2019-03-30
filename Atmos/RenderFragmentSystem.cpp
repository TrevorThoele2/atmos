
#include "RenderFragmentSystem.h"

namespace Atmos
{
    RenderFragmentSystem::RenderFragmentSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        renderFragments = manager.Batch<nRenderFragment>();
        renderFragments.onCreated.Subscribe(&RenderFragmentSystem::OnCreated, *this);
    }

    void RenderFragmentSystem::DrawAll()
    {

    }

    void RenderFragmentSystem::OnCreated(Reference object)
    {
        grid.Add(object);

        object->onBoundsChanged.Subscribe(&RenderFragmentSystem::OnObjectBoundsChanged, *this);
    }

    void RenderFragmentSystem::OnObjectBoundsChanged(TypedObjectReference<PositionalObject> object, AxisBoundingBox3D previous)
    {
        grid.InformChanged(object, previous);
    }
}