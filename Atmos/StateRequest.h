#pragma once

#include <Arca/Signal.h>
#include <Arca/LocalPtr.h>

#include "GameState.h"

#include <variant>

namespace Atmos::State
{
    struct Request
    {
        enum Pop {};

        using Value = std::variant<Arca::LocalPtr<GameState>, Pop>;
        Value value;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::State::Request>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}