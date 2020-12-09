#include "EntityPrototype.h"

namespace Atmos::Entity
{
    Prototype::Prototype(
        Arca::Index<Scripting::Script> constructor,
        Name name,
        Spatial::Grid::Point position,
        Spatial::Angle2D direction)
        :
        constructor(constructor),
        name(name),
        position(position),
        direction(direction)
    {}
}