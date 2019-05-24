
#include "EntityComponent.h"

namespace Atmos
{
    namespace Entity
    {
        Component::Component(ObjectManager& manager, EntityReference owner) : Object(manager), owner(owner)
        {}

        Component::Component(const Component& arg) : Object(arg), owner(arg.owner)
        {}

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(Component) : INSCRIPTION_TABLE_GET_BASE(Object)
        {}

        ObjectTypeDescription Component::TypeDescription() const
        {
            return ObjectTraits<Component>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Entity::Component>::typeName = "EntityComponent";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Entity::Component)
    {

    }
}