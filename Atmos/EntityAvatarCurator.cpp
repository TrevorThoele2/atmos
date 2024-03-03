#include "EntityAvatarCurator.h"

#include <Arca/Reliquary.h>
#include <Arca/Actualization.h>

#include "GeneralComponent.h"

namespace Atmos::Entity
{
    AvatarCurator::AvatarCurator(Init init) :
        Curator(init), currentAvatar(init.owner),
        debugPlayerColumn(
            [this](Debug::Statistics& statistics)
            {
                statistics.game.playerColumn = currentAvatar->entity->general->position.x;
            },
            MutablePointer()),
        debugPlayerRow(
            [this](Debug::Statistics& statistics)
            {
                statistics.game.playerRow = currentAvatar->entity->general->position.y;
            },
            MutablePointer())
    {}

    void AvatarCurator::Work()
    {
        Owner().ExecuteOn<Arca::CreatedKnown<AvatarComponent>>(
            [this](const Arca::CreatedKnown<AvatarComponent>& signal)
            {
                if (currentAvatar->entity)
                    Owner().Destroy(AsHandle(*currentAvatar->entity));

                auto currentAvatarData = MutablePointer().Of(currentAvatar);
                currentAvatarData->entity = Arca::Index<Entity>(signal.reference.ID(), Owner());
                currentAvatarData->component = signal.reference;
            });

        Owner().ExecuteOn<Arca::DestroyingKnown<AvatarComponent>>(
            [this](const Arca::DestroyingKnown<AvatarComponent>&)
            {
                auto currentAvatarData = MutablePointer().Of(currentAvatar);
                currentAvatarData->entity = {};
                currentAvatarData->component = {};
            });

        debugPlayerColumn.Set();
        debugPlayerRow.Set();
    }
}