#pragma once

#include "Object.h"
#include "ObjectReference.h"
#include "ObjectScribe.h"

#include "EntityComponent.h"

namespace Atmos::Entity
{
    class Entity : public Object
    {
    public:
        typedef TypedObjectReference<Component> EntityComponentReference;
        std::vector<EntityComponentReference> componentList;
    public:
        Entity(ObjectManager& manager);
        Entity(const Entity& arg);
        Entity(const ::Inscription::BinaryTableData<Entity>& data);

        template<class ComponentT>
        TypedObjectReference<ComponentT> RetrieveComponent() const;
        template<class ComponentT>
        bool HasComponent() const;

        ObjectTypeDescription TypeDescription() const override;
    };

    template<class ComponentT>
    TypedObjectReference<ComponentT> Entity::RetrieveComponent() const
    {
        static_assert(std::is_base_of<Component, ComponentT>::value,
            "ComponentT must be derived from Entity::Component.");

        auto componentTypeName = TypeNameFor<ComponentT>();
        for (auto& loop : componentList)
            if (loop->TypeDescription().typeName == componentTypeName)
                return loop.Downcast<ComponentT>();

        return TypedObjectReference<ComponentT>();
    }

    template<class ComponentT>
    bool Entity::HasComponent() const
    {
        static_assert(std::is_base_of<Component, ComponentT>::value,
            "ComponentT must be derived from Entity::Component.");

        return RetrieveComponent<ComponentT>().IsOccupied();
    }
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Entity::Entity> :
        public ObjectTraitsBase<Entity::Entity>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Entity::Entity, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Entity::Entity, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Entity::Entity, BinaryArchive> :
        public ObjectScribe<::Atmos::Entity::Entity, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}