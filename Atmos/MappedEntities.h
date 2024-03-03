#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "Entity.h"

namespace Atmos::Entity
{
    class MappedEntities final : public Arca::ClosedTypedRelic<MappedEntities>
    {
    public:
        using NameToEntity = std::unordered_map<String, Arca::Index<Entity>>;
        NameToEntity nameToEntity;

        using PositionToEntity = std::unordered_map<Spatial::Grid::Point, Arca::Index<Entity>>;
        PositionToEntity positionToEntity;

        explicit MappedEntities(Init init);
        explicit MappedEntities(MappedEntities&& arg) = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::MappedEntities>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Entity::MappedEntities";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Entity::MappedEntities, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Entity::MappedEntities>;
    };
}