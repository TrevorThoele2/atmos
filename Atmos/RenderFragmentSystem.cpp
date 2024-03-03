#include "RenderFragmentSystem.h"

namespace Atmos::Render
{
    FragmentSystem::FragmentSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        renderFragments = manager.Batch<Fragment>();
        renderFragments.onCreated.Subscribe(&FragmentSystem::OnCreated, *this);
    }

    void FragmentSystem::DrawAll()
    {

    }

    void FragmentSystem::OnCreated(Reference object)
    {
        grid.Add(object);

        object->onBoundsChanged.Subscribe([this, object](AxisAlignedBox3D previous)
        {
            OnObjectBoundsChanged(object, previous);
        });
    }

    void FragmentSystem::OnObjectBoundsChanged(TypedObjectReference<AxisAlignedObject> object, AxisAlignedBox3D previous)
    {
        grid.InformChanged(object, previous);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::FragmentSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}