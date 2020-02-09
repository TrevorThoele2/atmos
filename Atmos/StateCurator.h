#pragma once

#include <Arca/Curator.h>
#include <Arca/RelicBatch.h>
#include <Arca/SignalBatch.h>

#include "StateRequest.h"

#include "Serialization.h"

namespace Atmos::State
{
    class StateCurator final : public Arca::Curator
    {
    public:
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
    struct Traits<Atmos::State::StateCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline TypeName typeName = "StateCurator";
        using HandledCommands = HandledCommands<Atmos::State::Request>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::State::StateCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::State::StateCurator, BinaryArchive>
    {};
}