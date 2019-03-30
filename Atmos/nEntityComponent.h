#pragma once

#include "Object.h"
#include "ObjectReference.h"

#include "StoredReadonlyProperty.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Ent
    {
        class nEntity;

        class nEntityComponent : public Object
        {
        public:
            typedef TypedObjectReference<nEntity> EntityReference;
            EntityReference owner;
        public:
            nEntityComponent(EntityReference owner);
            nEntityComponent(const nEntityComponent& arg) = default;
            nEntityComponent(const ::Inscription::Table<nEntityComponent>& table);
            ObjectTypeDescription TypeDescription() const override;
        };
    }

    template<>
    struct ObjectTraits<Ent::nEntityComponent> : ObjectTraitsBase<Ent::nEntityComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Ent::nEntityComponent)
    {
    public:
        static void AddMembers(TableT& table);
    };
}