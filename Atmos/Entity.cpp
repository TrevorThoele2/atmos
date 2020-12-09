#include "Entity.h"

namespace Atmos::Entity
{
    Entity::Entity(
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
    {}
}