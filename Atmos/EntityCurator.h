#pragma once

#include <Arca/Curator.h>

#include "Entity.h"
#include "MappedEntities.h"

namespace Atmos::Debug
{
    class Statistics;
}

namespace Atmos::Entity
{
    class EntityCurator final : public Arca::Curator
    {
    protected:
        void InitializeImplementation() override;
    private:
        MappedEntities* mappedEntities = nullptr;
    private:
        Debug::Statistics* debugStatistics = nullptr;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::EntityCurator, BinaryArchive> final
        : public ArcaNullScribe<::Atmos::Entity::EntityCurator, BinaryArchive>
    {};
}