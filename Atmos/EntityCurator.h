#pragma once

#include <Arca/Curator.h>

#include "Entity.h"
#include "MappedEntities.h"

namespace Atmos::Entity
{
    class EntityCurator final : public Arca::Curator
    {
    public:
        explicit EntityCurator(Init init);
    private:
        Arca::Index<MappedEntities> mappedEntities;
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