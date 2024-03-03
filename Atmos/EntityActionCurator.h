#pragma once

#include <Arca/Curator.h>
#include <Arca/ShardBatch.h>

#include "ActionComponent.h"

#include "Field.h"

namespace Atmos::Entity
{
    class ActionCurator final : public Arca::Curator
    {
    public:
        explicit ActionCurator(Init init);

        void Work();
    private:
        Arca::Batch<ActionComponent> actionComponents;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::ActionCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::ActionCurator, BinaryArchive>
    {};
}