
#include "RenderFragmentSystem.h"

namespace Atmos
{
    RenderFragmentSystem::RenderFragmentSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        renderFragments = manager.Batch<RenderFragment>();
        renderFragments.onCreated.Subscribe(&RenderFragmentSystem::OnCreated, *this);
    }

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(RenderFragmentSystem) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

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
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::RenderFragmentSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::RenderFragmentSystem, "RenderFragmentSystem");
}