#pragma once

#include "Object.h"
#include "ObjectReference.h"

#include "StoredReadonlyProperty.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Entity
    {
        class Entity;

        class Component : public Object
        {
        public:
            typedef TypedObjectReference<Entity> EntityReference;
            EntityReference owner;
        public:
            Component(ObjectManager& manager, EntityReference owner);
            Component(const Component& arg);
            Component(const ::Inscription::Table<Component>& table);
            ObjectTypeDescription TypeDescription() const override;
        };
    }

    template<>
    struct ObjectTraits<Entity::Component> : ObjectTraitsBase<Entity::Component>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Entity::Component)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}