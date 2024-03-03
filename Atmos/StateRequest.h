#pragma once

#include <Arca/SignalTraits.h>

#include <variant>

namespace Atmos::State
{
    class State;

    struct Request
    {
        enum Pop {};

        using Value = std::variant<State*, Pop>;
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