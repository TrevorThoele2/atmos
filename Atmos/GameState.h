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
    template<>
    class Scribe<::Atmos::State::GameState, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::State::GameState, BinaryArchive>
    {};
}