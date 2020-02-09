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
    public:
        explicit EntityCurator(Init init);
    private:
        Arca::GlobalIndex<MappedEntities> mappedEntities;
    private:
        Arca::GlobalIndex<Debug::Statistics> debugStatistics;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::EntityCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "EntityCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::EntityCurator, BinaryArchive> final
        : public ArcaNullScribe<::Atmos::Entity::EntityCurator, BinaryArchive>
    {};
}