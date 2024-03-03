#pragma once

#include "EntityComponent.h"

namespace Atmos
{
    namespace Entity
    {
        class AvatarComponent : public Component
        {
        public:
            typedef int Currency;
        public:
            Currency currency;
        public:
            AvatarComponent(ObjectManager& manager, EntityReference owner);
            AvatarComponent(const AvatarComponent& arg) = default;
            AvatarComponent(const ::Inscription::Table<AvatarComponent>& table);

            ObjectTypeDescription TypeDescription() const override;
        private:
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Entity::AvatarComponent> : ObjectTraitsBase<Entity::AvatarComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Entity::AvatarComponent)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}