#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "AvatarComponent.h"
#include "nEntity.h"

namespace Atmos
{
    namespace Ent
    {
        class nEntityAvatarSystem : public ObjectSystem
        {
        private:
            typedef TypedObjectReference<nEntity> EntityReference;
            typedef TypedObjectReference<nAvatarComponent> AvatarComponentReference;
        public:
            nEntityAvatarSystem(ObjectManager& manager);

            void Set(EntityReference set);

            EntityReference Avatar();
            AvatarComponentReference Component();
        private:
            EntityReference avatar;
        private:
            ObjectBatch<nAvatarComponent> avatarBatch;
            void OnAvatarComponentCreated(AvatarComponentReference reference);
            void OnAvatarComponentDestroyed(AvatarComponentReference reference);
        };
    }
}