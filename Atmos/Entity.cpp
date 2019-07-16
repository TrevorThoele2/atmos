#include "Entity.h"

namespace Atmos::Entity
{
    Entity::Entity(ObjectManager& manager) : Object(manager)
    {}

    Entity::Entity(const Entity& arg) : Object(arg)
    {}

    Entity::Entity(const ::Inscription::BinaryTableData<Entity>& data) :
        Object(std::get<0>(data.bases))
    {}

    ObjectTypeDescription Entity::TypeDescription() const
    {
        return ObjectTraits<Entity>::TypeDescription();
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Entity::Entity>::typeName = "Entity";
}