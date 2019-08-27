#include "EntityAvatarCurator.h"

#include <Arca/Reliquary.h>
#include <Arca/Actualization.h>
#include "DebugStatistics.h"

#include "GeneralComponent.h"

namespace Atmos::Entity
{
    void AvatarCurator::InitializeImplementation()
    {
        currentAvatar = Owner().Find<CurrentAvatar>();

        debugStatistics = Owner().Find<Debug::Statistics>();
    }

    void AvatarCurator::WorkImplementation(Stage& stage)
    {
        Owner().ExecuteOn<Arca::Created>(
            [this](const Arca::Created& signal)
            {
                const auto actualized = Arca::Actualize<AvatarComponent>(signal.handle);
                if (!actualized)
                    return;

                if (currentAvatar->entity)
                    Owner().Destroy(AsHandle(*currentAvatar->entity));

                currentAvatar->entity = Owner().Find<Entity>(signal.handle.ID());
                currentAvatar->component = actualized;
            });

        Owner().ExecuteOn<Arca::Destroying>(
            [this](const Arca::Destroying& signal)
            {
                const auto actualized = Arca::Actualize<AvatarComponent>(signal.handle);
                if (!actualized)
                    return;

                currentAvatar->entity = {};
                currentAvatar->component = {};
            });

        auto& general = *currentAvatar->entity->general;
        debugStatistics->game.playerColumn = general.position.x;
        debugStatistics->game.playerRow = general.position.y;
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Entity::AvatarCurator>::typeName = "AvatarCurator";
}