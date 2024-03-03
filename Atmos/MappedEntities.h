#pragma once

#include "ArcaRelicIncludes.h"
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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Entity::Mapped";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Entity::Mapped, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Entity::Mapped>;
    };
}