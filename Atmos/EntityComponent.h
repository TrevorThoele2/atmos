#pragma once

#include "Object.h"
#include "ObjectReference.h"

#include "StoredReadonlyProperty.h"

#include "ObjectScribe.h"

namespace Atmos::Entity
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
        Component(const ::Inscription::BinaryTableData<Component>& data);

        ObjectTypeDescription TypeDescription() const override;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Entity::Component> : ObjectTraitsBase<Entity::Component>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Entity::Component, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Entity::Component, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Entity::Component, BinaryArchive> :
        public ObjectScribe<::Atmos::Entity::Component, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}