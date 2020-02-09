#pragma once

#include <Arca/Curator.h>
#include <Arca/RelicBatch.h>

#include "AIComponent.h"

namespace Atmos::Entity
{
    class AICurator final : public Arca::Curator
    {
    public:
        explicit AICurator(Init init);

        void Work();
    private:
        Arca::Batch<AIComponent> aiComponents;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::AICurator, BinaryArchive> :
        public ArcaNullScribe<::Atmos::Entity::AICurator, BinaryArchive>
    {};
}