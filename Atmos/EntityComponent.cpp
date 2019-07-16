
#include "EntityComponent.h"

namespace Atmos::Entity
{
    Component::Component(ObjectManager& manager, EntityReference owner) :
        Object(manager), owner(owner)
    {}

    Component::Component(const Component& arg) :
        Object(arg), owner(arg.owner)
    {}

    Component::Component(const ::Inscription::BinaryTableData<Component>& data) :
        Object(std::get<0>(data.bases))
    {}

    ObjectTypeDescription Component::TypeDescription() const
    {
        return ObjectTraits<Component>::TypeDescription();
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Entity::Component>::typeName = "EntityComponent";
}