
#include "AvatarSystem.h"

#include "ObjectManager.h"

namespace Atmos
{
    namespace Ent
    {
        nEntityAvatarSystem::nEntityAvatarSystem(ObjectManager& manager) : ObjectSystem(manager)
        {
            avatarBatch = manager.Batch<nAvatarComponent>();
            avatarBatch.onCreated.Subscribe(&nEntityAvatarSystem::OnAvatarComponentCreated, *this);
            avatarBatch.onBeforeDestroyed.Subscribe(&nEntityAvatarSystem::OnAvatarComponentDestroyed, *this);
        }

        void nEntityAvatarSystem::Set(EntityReference set)
        {
            avatar = set;
        }

        nEntityAvatarSystem::EntityReference nEntityAvatarSystem::Avatar()
        {
            return avatar;
        }

        nEntityAvatarSystem::AvatarComponentReference nEntityAvatarSystem::Component()
        {
            if (avatar.IsOccupied())
                return avatar->Component<nAvatarComponent>();

            return AvatarComponentReference();
        }

        void nEntityAvatarSystem::OnAvatarComponentCreated(AvatarComponentReference reference)
        {
            if (avatar.IsOccupied())
                Manager()->DestroyObject(avatar);

            Set(reference->owner);
        }

        void nEntityAvatarSystem::OnAvatarComponentDestroyed(AvatarComponentReference reference)
        {
            Set(EntityReference());
        }
    }
}