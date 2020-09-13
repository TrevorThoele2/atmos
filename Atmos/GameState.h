#pragma once

#include <Arca/ClosedTypedRelic.h>

namespace Atmos::State
{
    class GameState final : public Arca::ClosedTypedRelic<GameState>
    {};
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::State::GameState>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::State::GameState";
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::State::GameState, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::State::GameState>;
    };
}