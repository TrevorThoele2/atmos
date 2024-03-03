#include "Entity.h"

#include "DataCore.h"

namespace Atmos::Entity
{
    Entity::Entity(Arca::RelicInit init)
    {
        init.Create<DataCore>();
    }

    Entity::Entity(
        Arca::RelicInit init,
        Name name,
        Name displayName,
        Spatial::Grid::Point position,
        Spatial::Angle2D direction,
        bool isSolid)
        :
        name(name),
        displayName(displayName),
        position(position),
        direction(direction),
        isSolid(isSolid)
    {
        init.Create<DataCore>();
    }
}