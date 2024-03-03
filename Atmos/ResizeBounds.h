#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Size2D.h"

namespace Atmos
{
    struct ResizeBounds
    {
        Arca::RelicID id = 0;
        Size2D toSize;

        ResizeBounds(Arca::RelicID id, const Size2D& toSize) : id(id), toSize(toSize)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::ResizeBounds>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "ResizeBounds";
    };
}