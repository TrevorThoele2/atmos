
#include "Entity.h"

namespace Atmos
{
    namespace Entity
    {
        Entity::Entity(ObjectManager& manager) : Object(manager)
        {}

        Entity::Entity(const Entity& arg) : Object(arg)
        {}

        Entity::Entity(const ::Inscription::Table<Entity>& table) : INSCRIPTION_TABLE_GET_BASE(Object)
        {}

        ObjectTypeDescription Entity::TypeDescription() const
        {
            return ObjectTraits<Entity>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Entity::Entity>::typeName = "Entity";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Entity::Entity)
    {

    }
}