#include "EntityPrototype.h"

namespace Atmos::Entity
{
    Prototype::Prototype(
        Arca::Index<Scripting::Script> constructor,
        Name name,
        Spatial::Grid::Point position,
        bool solid)
        :
        constructor(constructor),
        name(name),
        position(position),
        solid(solid)
    {}
}