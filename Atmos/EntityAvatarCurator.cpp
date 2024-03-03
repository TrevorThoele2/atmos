#include "EntityAvatarCurator.h"

#include <Arca/Reliquary.h>

namespace Atmos::Entity
{
    AvatarCurator::AvatarCurator(Init init) :
        Curator(init), currentAvatar(init.owner),
        debugPlayerColumn(
            [this](Debug::Statistics& statistics)
            {
                statistics.game.playerColumn = currentAvatar->entity->position.x;
            },
            MutablePointer()),
        debugPlayerRow(
            [this](Debug::Statistics& statistics)
            {
                statistics.game.playerRow = currentAvatar->entity->position.y;
            },
            MutablePointer())
    {}

    void AvatarCurator::Work()
    {
        debugPlayerColumn.Set();
        debugPlayerRow.Set();
    }

    void AvatarCurator::Handle(const SetAvatar& command)
    {
        auto mutableCurrentAvatar = MutablePointer().Of(currentAvatar);
        mutableCurrentAvatar->entity = command.setAs;
    }
}