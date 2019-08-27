#pragma once

#include <Arca/Curator.h>

#include "PositionedEntities.h"

#include "Entity.h"
#include "GeneralComponent.h"

#include "GridPosition.h"

namespace Atmos::Debug
{
    class Statistics;
}

namespace Atmos::Entity
{
    class PositionCurator final : public Arca::Curator
    {
    protected:
        void InitializeImplementation() override;
    private:
        PositionedEntities* positionedEntities = nullptr;
    private:
        Debug::Statistics* debugStatistics = nullptr;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::PositionCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::PositionCurator, BinaryArchive>
    {};
}