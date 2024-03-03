
#include "EntityAvatarSystem.h"

#include "ObjectManager.h"
#include "DebugStatisticsSystem.h"

#include "GeneralComponent.h"

#include "StringUtility.h"

namespace Atmos
{
    namespace Entity
    {
        AvatarSystem::AvatarSystem(ObjectManager& manager) : ObjectSystem(manager)
        {
            avatarBatch = manager.Batch<AvatarComponent>();
            avatarBatch.onCreated.Subscribe(&AvatarSystem::OnAvatarComponentCreated, *this);
            avatarBatch.onBeforeDestroyed.Subscribe(&AvatarSystem::OnAvatarComponentDestroyed, *this);
        }

        AvatarSystem::AvatarSystem(const ::Inscription::Table<AvatarSystem>& table) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
        {}

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
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Entity::AvatarSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::Entity::AvatarSystem, "EntityAvatarSystem");
}