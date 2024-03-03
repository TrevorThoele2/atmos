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
    protected:
        void InitializeImplementation() override;
        void WorkImplementation(Stage& stage) override;
    private:
        using Stack = std::vector<GameState*>;
        Stack stack;

        Arca::Batch<Request> requests;

        void Push(GameState& state);
        void Pop();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::State::StateCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::State::StateCurator, BinaryArchive>
    {};
}