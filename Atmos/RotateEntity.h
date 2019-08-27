#pragma once

#include <Arca/SignalTraits.h>
#include "Entity.h"
#include "Direction.h"

namespace Atmos::Entity
{
    struct RotateEntity
    {
        Entity* entity = nullptr;
        Direction toFace;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::RotateEntity>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}