#pragma once

#include <Arca/Signal.h>
#include <Arca/Index.h>

#include "GameState.h"

#include <variant>

namespace Atmos::State
{
    struct Request
    {
        enum Pop {};

        using Value = std::variant<Arca::Index<GameState>, Pop>;
        Value value;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::State::Request>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::State::Request";
    };
}