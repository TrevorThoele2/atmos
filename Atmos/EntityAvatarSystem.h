#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "AvatarComponent.h"
#include "Entity.h"

namespace Atmos
{
    namespace Entity
    {
        class AvatarSystem : public ObjectSystem
        {
        private:
            typedef TypedObjectReference<Entity> EntityReference;
            typedef TypedObjectReference<AvatarComponent> AvatarComponentReference;
        public:
            AvatarSystem(ObjectManager& manager);
            AvatarSystem(const ::Inscription::Table<AvatarSystem>& table);

            void Set(EntityReference set);

            EntityReference Avatar();
            AvatarComponentReference Component();
        private:
            void InitializeImpl() override;
        private:
            EntityReference avatar;
        private:
            ObjectBatch<AvatarComponent> avatarBatch;
            void OnAvatarComponentCreated(AvatarComponentReference reference);
            void OnAvatarComponentDestroyed(AvatarComponentReference reference);
        };
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::Entity::AvatarSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}