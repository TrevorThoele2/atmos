#include "RenderFragmentSystem.h"

namespace Atmos::Render
{
    RenderFragmentSystem::RenderFragmentSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        renderFragments = manager.Batch<RenderFragment>();
        renderFragments.onCreated.Subscribe(&RenderFragmentSystem::OnCreated, *this);
    }

    void RenderFragmentSystem::DrawAll()
    {

    }

    void RenderFragmentSystem::OnCreated(Reference object)
    {
        grid.Add(object);

        object->onBoundsChanged.Subscribe([this, object](AxisAlignedBox3D previous)
        {
            OnObjectBoundsChanged(object, previous);
        });
    }

    void RenderFragmentSystem::OnObjectBoundsChanged(TypedObjectReference<AxisAlignedObject> object, AxisAlignedBox3D previous)
    {
        grid.InformChanged(object, previous);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Render::RenderFragmentSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}