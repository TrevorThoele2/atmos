#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

namespace Atmos::State
{
    class GameState final : public Arca::ClosedTypedRelicAutomation<GameState>
    {
    public:
        GameState() = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::State::GameState>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::State::GameState, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::State::GameState, BinaryArchive>
    {};
}