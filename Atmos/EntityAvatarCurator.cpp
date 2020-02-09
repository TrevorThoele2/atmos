#include "EntityAvatarCurator.h"

#include <Arca/Reliquary.h>
#include <Arca/Actualization.h>
#include "DebugStatistics.h"

#include "GeneralComponent.h"

namespace Atmos::Entity
{
    AvatarCurator::AvatarCurator(Init init) :
        Curator(init), currentAvatar(init.owner), debugStatistics(init.owner)
    {}

    void AvatarCurator::Work()
    {
        Owner().ExecuteOn<Arca::CreatedKnown<AvatarComponent>>(
            [this](const Arca::CreatedKnown<AvatarComponent>& signal)
            {
                if (currentAvatar->entity)
                    Owner().Destroy(AsHandle(*currentAvatar->entity));

                auto currentAvatarData = Data(currentAvatar);
                currentAvatarData->entity = Arca::RelicIndex<Entity>(signal.index.ID(), Owner());
                currentAvatarData->component = signal.index;
            });

        Owner().ExecuteOn<Arca::DestroyingKnown<AvatarComponent>>(
            [this](const Arca::DestroyingKnown<AvatarComponent>&)
            {
                auto currentAvatarData = Data(currentAvatar);
                currentAvatarData->entity = {};
                currentAvatarData->component = {};
            });

        auto& general = *currentAvatar->entity->general;
        const auto debugStatisticsData = Data(debugStatistics);
        debugStatisticsData->game.playerColumn = general.position.x;
        debugStatisticsData->game.playerRow = general.position.y;
    }
}