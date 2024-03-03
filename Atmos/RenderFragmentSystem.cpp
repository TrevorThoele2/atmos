
#include "RenderFragmentSystem.h"

namespace Atmos
{
    RenderFragmentSystem::RenderFragmentSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        renderFragments = manager.Batch<RenderFragment>();
        renderFragments.onCreated.Subscribe(&RenderFragmentSystem::OnCreated, *this);
    }

    RenderFragmentSystem::RenderFragmentSystem(const ::Inscription::Table<RenderFragmentSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

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

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::RenderFragmentSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::RenderFragmentSystem, "RenderFragmentSystem");
}