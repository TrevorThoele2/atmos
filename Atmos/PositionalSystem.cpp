
#include "PositionalSystem.h"

namespace Atmos
{
    PositionalSystem::PositionalSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        batch = manager.Batch<PositionalObject>();
        batch.onCreated.Subscribe(&PositionalSystem::OnCreated, *this);
    }

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(PositionalSystem) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

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

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::PositionalSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::PositionalSystem, "PositionalSystem");
}