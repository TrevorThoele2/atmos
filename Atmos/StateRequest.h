#pragma once

#include <Arca/Signal.h>
#include <Arca/RelicIndex.h>

#include "GameState.h"

#include <variant>

namespace Atmos::State
{
    struct Request
    {
        enum Pop {};

        using Value = std::variant<Arca::RelicIndex<GameState>, Pop>;
        Value value;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::State::Request>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "StateRequest";
    };
}