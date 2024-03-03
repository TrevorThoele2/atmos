#pragma once

#include <Arca/Relic.h>
#include "Entity.h"

namespace Atmos::Entity
{
    class Mapped final
    {
    public:
        using NameToEntity = std::unordered_map<String, Arca::Index<Entity>>;
        NameToEntity nameToEntity;

        using PositionToEntity = std::unordered_map<Spatial::Grid::Point, std::set<Arca::Index<Entity>>>;
        PositionToEntity positionToEntity;

        Mapped() = default;
        explicit Mapped(Mapped&& arg) = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Mapped>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Entity::Mapped"; }
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Entity::Mapped, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Entity::Mapped>;
    };
}