
#include <functional>

#include "GridPosition.h"
#include "GridSize.h"
#include "Math.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(GridPosition)
    {
        scribe(x);
        scribe(y);
        scribe(z);
    }

    GridPosition::GridPosition(ValueT x, ValueT y, ValueT z) : x(x), y(y), z(z)
    {}

    GridPosition::GridPosition(const GridPosition &source, const RelativeGridPosition &offset) : x(source.x + offset.x), y(source.y + offset.y), z(source.z + offset.z)
    {}

    GridPosition::GridPosition(const Position2D &arg, ValueT z) : x(DimensionFromPosition(arg.GetX())), y(DimensionFromPosition(arg.GetY())), z(z)
    {}

    GridPosition::GridPosition(const Position3D &pos) : x(DimensionFromPosition(pos.GetX())), y(DimensionFromPosition(pos.GetY())), z(DimensionFromPosition(pos.GetZ()))
    {}

    bool GridPosition::operator==(const GridPosition &arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool GridPosition::operator!=(const GridPosition &arg) const
    {
        return !(*this == arg);
    }

    void GridPosition::Edit(ValueT x, ValueT y, ValueT z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void GridPosition::Edit(const RelativeGridPosition &offset)
    {
        x += offset.x;
        y += offset.y;
        z += offset.z;
    }

    void GridPosition::Edit(const GridPosition &source, const RelativeGridPosition &offset)
    {
        x = source.x + offset.x;
        y = source.y + offset.y;
        z = source.z + offset.z;
    }

    void GridPosition::Edit(const Position2D &pos, ValueT z)
    {
        x = DimensionFromPosition(pos.GetX());
        y = DimensionFromPosition(pos.GetY());
        this->z = z;
    }

    void GridPosition::Edit(const Position3D &pos)
    {
        x = DimensionFromPosition(pos.GetX());
        y = DimensionFromPosition(pos.GetY());
        z = DimensionFromPosition(pos.GetZ());
    }

    void GridPosition::SetX(ValueT set)
    {
        x = set;
    }

    void GridPosition::SetY(ValueT set)
    {
        y = set;
    }

    void GridPosition::SetZ(ValueT set)
    {
        z = set;
    }

    GridPosition::operator Position2D() const
    {
        return Position2D(DimensionToPosition(x), DimensionToPosition(y));
    }

    GridPosition::operator Position3D() const
    {
        return Position3D(DimensionToPosition(x), DimensionToPosition(y), DimensionToPosition(z));
    }

    GridPosition::ValueT GridPosition::GetX() const
    {
        return x;
    }

    GridPosition::ValueT GridPosition::GetY() const
    {
        return y;
    }

    GridPosition::ValueT GridPosition::GetZ() const
    {
        return z;
    }

    GridPosition GridPosition::FindOffset(const RelativeGridPosition &offset) const
    {
        GridPosition ret;
        ret.Edit(*this, offset);
        return ret;
    }

    GridPosition::ValueT GridPosition::FindXDistance(const GridPosition &destination) const
    {
        return destination.x - x;
    }

    GridPosition::ValueT GridPosition::FindYDistance(const GridPosition &destination) const
    {
        return destination.y - y;
    }

    GridPosition::ValueT GridPosition::FindZDistance(const GridPosition &destination) const
    {
        return destination.z - z;
    }

    unsigned int GridPosition::FindDistance(const GridPosition &destination) const
    {
        return std::abs(FindXDistance(destination)) + std::abs(FindYDistance(destination)) + std::abs(FindZDistance(destination));
    }

    Direction GridPosition::DetermineDirection(const GridPosition &ending) const
    {
        if (ending.x < x)
            return Direction::ValueT::LEFT;
        else if (ending.x > x)
            return Direction::ValueT::RIGHT;
        else if (ending.y < y)
            return Direction::ValueT::UP;
        else if (ending.y > y)
            return Direction::ValueT::DOWN;
        else if (ending.z > z)
            return Direction::ValueT::Z_UP;
        else
            return Direction::ValueT::Z_DOWN;
    }

    bool GridPosition::IsCloser(const GridPosition &first, const GridPosition &second) const
    {
        auto thisDist = FindDistance(first);
        auto argDist = FindDistance(second);

        return argDist >= thisDist;
    }

    GridPosition GridPosition::FindPositionAdjacent(const Direction &dir) const
    {
        GridPosition newPos(*this);

        switch (dir.Get())
        {
        case Direction::ValueT::UP:
            --newPos.y;
            break;
        case Direction::ValueT::DOWN:
            ++newPos.y;
            break;
        case Direction::ValueT::LEFT:
            --newPos.x;
            break;
        case Direction::ValueT::RIGHT:
            ++newPos.x;
            break;
        case Direction::ValueT::Z_UP:
            ++newPos.z;
            break;
        case Direction::ValueT::Z_DOWN:
            --newPos.z;
            break;
        }

        return newPos;
    }

    RelativeGridPosition GridPosition::Difference(const GridPosition &against) const
    {
        return RelativeGridPosition(against.x - x, against.y - y, against.z - z);
    }

    GridPosition GridPosition::FromScreen(const Position2D &position, ValueT z, const Position2D &topLeftScreen)
    {
        return GridPosition(DimensionFromPosition(position.GetX() + topLeftScreen.GetX()), DimensionFromPosition(position.GetY() + topLeftScreen.GetY()), z);
    }

    GridPosition GridPosition::FromScreen(const Position3D &position, const Position2D &topLeftScreen)
    {
        return GridPosition(DimensionFromPosition(position.GetX() + topLeftScreen.GetX()), DimensionFromPosition(position.GetY() + topLeftScreen.GetY()), DimensionFromPosition(position.GetZ()));
    }

    GridPosition::ValueT GridPosition::DimensionFromPosition(Position2D::ValueT dim)
    {
        return static_cast<GridPosition::ValueT>(std::floor(dim / GRID_SIZE<Position3D::ValueT>));
    }

    Position2D::ValueT GridPosition::DimensionToPosition(ValueT dim)
    {
        return static_cast<Position2D::ValueT>((dim * GRID_SIZE<ValueT>) + (GRID_SIZE<ValueT> / 2));
    }

    void RelativeGridPosition::Serialize(::Inscription::Scribe &scribe)
    {
        scribe(x);
        scribe(y);
        scribe(z);
    }

    RelativeGridPosition::RelativeGridPosition(ValueT x, ValueT y, ValueT z) : x(x), y(y), z(z)
    {}

    RelativeGridPosition::RelativeGridPosition(const GridPosition &source, const GridPosition &destination) : x(destination.x - source.x), y(destination.y - source.y), z(destination.z - source.z)
    {}

    bool RelativeGridPosition::operator==(const RelativeGridPosition &arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool RelativeGridPosition::operator!=(const RelativeGridPosition &arg) const
    {
        return !(*this == arg);
    }

    void RelativeGridPosition::Edit(ValueT x, ValueT y, ValueT z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
}