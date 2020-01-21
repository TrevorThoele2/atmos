#pragma once

#include <Arca/Signal.h>
#include "Entity.h"
#include "TimeValue.h"

namespace Atmos::Entity
{
    struct MoveEntity
    {
        Arca::RelicIndex<Entity> entity;
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
        static inline const TypeName typeName = "MoveEntity";
    };
}