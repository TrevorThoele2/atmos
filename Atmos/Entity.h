#pragma once

#include "Object.h"
#include "ObjectReference.h"
#include "ObjectSerialization.h"

#include "EntityComponent.h"

namespace Atmos
{
    namespace Entity
    {
        class Entity : public Object
        {
        public:
            typedef TypedObjectReference<Component> EntityComponentReference;
            std::vector<EntityComponentReference> componentList;
        public:
            Entity(ObjectManager& manager);
            Entity(const Entity& arg);
            INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(Entity);

            template<class ComponentT>
            TypedObjectReference<ComponentT> RetrieveComponent() const;
            template<class ComponentT>
            bool HasComponent() const;

            ObjectTypeDescription TypeDescription() const override;
        };

        template<class ComponentT>
        TypedObjectReference<ComponentT> Entity::RetrieveComponent() const
        {
            static_assert(std::is_base_of<Component, ComponentT>::value, "ComponentT must be derived from Entity::Component.");

            auto componentTypeName = TypeNameFor<ComponentT>();
            for (auto& loop : componentList)
                if (loop->TypeDescription().typeName == componentTypeName)
                    return loop.Downcast<ComponentT>();

            return TypedObjectReference<ComponentT>();
        }

        template<class ComponentT>
        bool Entity::HasComponent() const
        {
            static_assert(std::is_base_of<Component, ComponentT>::value, "ComponentT must be derived from Entity::Component.");

            return RetrieveComponent<ComponentT>().IsOccupied();
        }
    }

    template<>
    struct ObjectTraits<Entity::Entity> : ObjectTraitsBase<Entity::Entity>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Entity::Entity)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}