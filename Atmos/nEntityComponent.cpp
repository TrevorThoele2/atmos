
#include "nEntityComponent.h"

namespace Atmos
{
    namespace Ent
    {
        nEntityComponent::nEntityComponent(EntityReference owner) : owner(owner)
        {}

        nEntityComponent::nEntityComponent(const ::Inscription::Table<nEntityComponent>& table) : INSCRIPTION_TABLE_GET_BASE(Object)
        {}

        ObjectTypeDescription nEntityComponent::TypeDescription() const
        {
            return ObjectTraits<nEntityComponent>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Ent::nEntityComponent>::typeName = "EntityComponent";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Ent::nEntityComponent)
    {

    }
}