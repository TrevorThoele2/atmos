#pragma once

#include <Arca/SignalTraits.h>
#include "Entity.h"
#include "TimeValue.h"

namespace Atmos::Entity
{
    struct MoveEntity
    {
        Entity* entity = nullptr;
        Grid::Position toPosition;
        Time::Value timeTaken;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::MoveEntity>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}