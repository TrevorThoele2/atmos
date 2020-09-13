#include "Entity.h"

namespace Atmos::Entity
{
    Entity::Entity(Init init) : ClosedTypedRelic(init)
    {}

    Entity::Entity(
        Init init,
        Name name,
        Spatial::Grid::Point position,
        Spatial::Angle2D direction)
        :
        ClosedTypedRelic(init),
        name(name),
        position(position),
        direction(direction)
    {}
}