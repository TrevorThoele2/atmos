#include "PositionedEntities.h"

namespace Atmos::Entity
{
    bool PositionedEntities::CanMove(const Entity& entity) const
    {
        return true;
    }

    bool PositionedEntities::CanMove(const Entity& entity, const Spatial::Grid::Point& to) const
    {
        return true;
    }

    bool PositionedEntities::CanMove(const Entity& entity, const Spatial::Direction& direction) const
    {
        return true;
    }

    bool PositionedEntities::IsMoving(const Entity& entity) const
    {
        return false;
    }

    size_t PositionedEntities::MovingSize() const
    {
        return movingEntities.size();
    }
}