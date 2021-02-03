#include "EntityPrototype.h"

namespace Atmos::Entity
{
    Prototype::Prototype(
        Arca::Index<Scripting::Script> constructor,
        Name name,
        Spatial::Grid::Point position)
        :
        constructor(constructor),
        name(name),
        position(position)
    {}
}