#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "Entity.h"

namespace Atmos::Entity
{
    class PositionedEntities final : public Arca::ClosedTypedRelic<PositionedEntities>
    {
    public:
        [[nodiscard]] bool CanMove(const Entity& entity) const;
        [[nodiscard]] bool CanMove(const Entity& entity, const Grid::Position& to) const;
        [[nodiscard]] bool CanMove(const Entity& entity, const Grid::RelativePosition& to) const;
        [[nodiscard]] bool CanMove(const Entity& entity, const Direction& direction) const;

        [[nodiscard]] bool IsMoving(const Entity& entity) const;

        [[nodiscard]] size_t MovingSize() const;
    private:
        using Map = std::unordered_map<Grid::Position, Entity*>;
        Map map;

        using MovingEntities = std::vector<Entity*>;
        MovingEntities movingEntities;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::PositionedEntities>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Entity::PositionedEntities";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::PositionedEntities, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Entity::PositionedEntities, BinaryArchive>
    {};
}