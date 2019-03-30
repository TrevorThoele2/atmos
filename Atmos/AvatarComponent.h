#pragma once

#include "nEntityComponent.h"

#include "Currency.h"

namespace Atmos
{
    namespace Ent
    {
        class nAvatarComponent : public nEntityComponent
        {
        public:
            Currency currency;
        public:
            nAvatarComponent(EntityReference owner);
            nAvatarComponent(const nAvatarComponent& arg) = default;
            nAvatarComponent(const ::Inscription::Table<nAvatarComponent>& table);

            ObjectTypeDescription TypeDescription() const override;
        private:
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Ent::nAvatarComponent> : ObjectTraitsBase<Ent::nAvatarComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Ent::nAvatarComponent)
    {
    public:
        static void AddMembers(TableT& table);
    };
}