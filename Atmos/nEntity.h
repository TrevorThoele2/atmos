#pragma once

#include "Object.h"
#include "ObjectReference.h"
#include "ObjectSerialization.h"

#include "nEntityComponent.h"

namespace Atmos
{
    namespace Ent
    {
        class nEntity : public Object
        {
        public:
            typedef TypedObjectReference<nEntityComponent> EntityComponentReference;
            std::vector<EntityComponentReference> componentList;
        public:
            nEntity() = default;
            nEntity(const nEntity& arg) = default;
            nEntity(const ::Inscription::Table<nEntity>& table);

            template<class ComponentT>
            TypedObjectReference<ComponentT> Component() const;
            template<class ComponentT>
            bool HasComponent() const;

            ObjectTypeDescription TypeDescription() const override;
        };

        template<class ComponentT>
        TypedObjectReference<ComponentT> nEntity::Component() const
        {
            static_assert(std::is_base_of<nEntityComponent, ComponentT>::value, "ComponentT must be derived from EntityComponent.");

            auto componentTypeName = TypeNameFor<ComponentT>();
            for (auto& loop : componentList)
                if (loop->TypeDescription().typeName == componentTypeName)
                    return loop.Downcast<ComponentT>();

            return TypedObjectReference<ComponentT>();
        }

        template<class ComponentT>
        bool nEntity::HasComponent() const
        {
            static_assert(std::is_base_of<nEntityComponent, ComponentT>::value, "ComponentT must be derived from EntityComponent.");

            return Component<ComponentT>().IsOccupied();
        }
    }

    template<>
    struct ObjectTraits<Ent::nEntity> : ObjectTraitsBase<Ent::nEntity>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Ent::nEntity)
    {
    public:
        static void AddMembers(TableT& table);
    };
}