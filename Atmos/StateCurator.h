#pragma once

#include <Arca/Curator.h>

#include "StateRequest.h"

#include "Serialization.h"

namespace Atmos::State
{
    class Curator final : public Arca::Curator
    {
    public:
        using Arca::Curator::Curator;

        void Handle(const Request& command);
    private:
        using Stack = std::vector<const GameState*>;
        Stack stack;

        void Push(const GameState& state);
        void Pop();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::State::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline TypeName typeName = "Atmos::State::Curator";
        using HandledCommands = HandledCommands<Atmos::State::Request>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::State::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::State::Curator>;
    };
}