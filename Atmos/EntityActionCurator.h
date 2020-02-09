#pragma once

#include <Arca/Curator.h>
#include <Arca/ShardBatch.h>
#include <Arca/SignalBatch.h>

#include "ActionComponent.h"

#include "Field.h"
#include "FieldSet.h"
#include "FieldUnset.h"

namespace Atmos::Entity
{
    class ActionCurator final : public Arca::Curator
    {
    public:
        explicit ActionCurator(Init init);

        void Work();
    private:
        Arca::Batch<ActionComponent> actionComponents;

        Arca::Batch<World::FieldSet> fieldSet;
        Arca::Batch<World::FieldUnset> fieldUnset;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::ActionCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::ActionCurator, BinaryArchive>
    {};
}