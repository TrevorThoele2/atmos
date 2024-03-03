
#include "EntityAvatarSystem.h"

#include "ObjectManager.h"
#include "DebugStatisticsSystem.h"

#include "GeneralComponent.h"

#include "StringUtility.h"

namespace Atmos::Entity
{
    AvatarSystem::AvatarSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        avatarBatch = manager.Batch<AvatarComponent>();
        avatarBatch.onCreated.Subscribe(&AvatarSystem::OnAvatarComponentCreated, *this);
        avatarBatch.onBeforeDestroyed.Subscribe(&AvatarSystem::OnAvatarComponentDestroyed, *this);
    }

    void AvatarSystem::Set(EntityReference set)
    {
        avatar = set;
    }

    AvatarSystem::EntityReference AvatarSystem::Avatar()
    {
        return avatar;
    }

    AvatarSystem::AvatarComponentReference AvatarSystem::Component()
    {
        if (avatar.IsOccupied())
            return avatar->RetrieveComponent<AvatarComponent>();

        return AvatarComponentReference();
    }

    void AvatarSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->gamePage.playerColumn.retrievalFunction = [this]() -> String
        {
            return ToString(Avatar()->RetrieveComponent<GeneralComponent>()->position.x);
        };

        debugStatistics->gamePage.playerRow.retrievalFunction = [this]() -> String
        {
            return ToString(Avatar()->RetrieveComponent<GeneralComponent>()->position.y);
        };
    }

    void AvatarSystem::OnAvatarComponentCreated(AvatarComponentReference reference)
    {
        if (avatar.IsOccupied())
            Manager()->DestroyObject(avatar);

        Set(reference->owner);
    }

    void AvatarSystem::OnAvatarComponentDestroyed(AvatarComponentReference reference)
    {
        Set(EntityReference());
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Entity::AvatarSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}