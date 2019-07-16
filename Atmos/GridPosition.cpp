#include <functional>

#include "GridPosition.h"
#include "GridSize.h"
#include "Math.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    GridPosition::GridPosition(Value x, Value y, Value z) :
        x(x), y(y), z(z)
    {}

    GridPosition::GridPosition(const GridPosition& source, const RelativeGridPosition& offset) :
        x(source.x + offset.x), y(source.y + offset.y), z(source.z + offset.z)
    {}

    GridPosition::GridPosition(const Position2D& arg, Value z) :
        x(DimensionFromPosition(arg.x)), y(DimensionFromPosition(arg.y)), z(z)
    {}

    GridPosition::GridPosition(const Position3D& pos) :
        x(DimensionFromPosition(pos.x)), y(DimensionFromPosition(pos.y)), z(DimensionFromPosition(pos.z))
    {}

    bool GridPosition::operator==(const GridPosition& arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool GridPosition::operator!=(const GridPosition& arg) const
    {
        return !(*this == arg);
    }

    void GridPosition::Edit(Value x, Value y, Value z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void GridPosition::Edit(const RelativeGridPosition& offset)
    {
        x += offset.x;
        y += offset.y;
        z += offset.z;
    }

    void GridPosition::Edit(const GridPosition& source, const RelativeGridPosition& offset)
    {
        x = source.x + offset.x;
        y = source.y + offset.y;
        z = source.z + offset.z;
    }

    void GridPosition::Edit(const Position2D& pos, Value z)
    {
        x = DimensionFromPosition(pos.x);
        y = DimensionFromPosition(pos.y);
        this->z = z;
    }

    void GridPosition::Edit(const Position3D& pos)
    {
        x = DimensionFromPosition(pos.x);
        y = DimensionFromPosition(pos.y);
        z = DimensionFromPosition(pos.z);
    }

    void GridPosition::SetX(Value set)
    {
        x = set;
    }

    void GridPosition::SetY(Value set)
    {
        y = set;
    }

    void GridPosition::SetZ(Value set)
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

    GridPosition::Value GridPosition::GetX() const
    {
        return x;
    }

    GridPosition::Value GridPosition::GetY() const
    {
        return y;
    }

    GridPosition::Value GridPosition::GetZ() const
    {
        return z;
    }

    GridPosition GridPosition::FindOffset(const RelativeGridPosition& offset) const
    {
        GridPosition ret;
        ret.Edit(*this, offset);
        return ret;
    }

    GridPosition::Value GridPosition::FindXDistance(const GridPosition& destination) const
    {
        return destination.x - x;
    }

    GridPosition::Value GridPosition::FindYDistance(const GridPosition& destination) const
    {
        return destination.y - y;
    }

    GridPosition::Value GridPosition::FindZDistance(const GridPosition& destination) const
    {
        return destination.z - z;
    }

    unsigned int GridPosition::FindDistance(const GridPosition& destination) const
    {
        return std::abs(FindXDistance(destination)) + std::abs(FindYDistance(destination)) + std::abs(FindZDistance(destination));
    }

    Direction GridPosition::DetermineDirection(const GridPosition& ending) const
    {
        if (ending.x < x)
            return Direction::Value::LEFT;
        else if (ending.x > x)
            return Direction::Value::RIGHT;
        else if (ending.y < y)
            return Direction::Value::UP;
        else if (ending.y > y)
            return Direction::Value::DOWN;
        else if (ending.z > z)
            return Direction::Value::Z_UP;
        else
            return Direction::Value::Z_DOWN;
    }

    bool GridPosition::IsCloser(const GridPosition& first, const GridPosition& second) const
    {
        auto thisDist = FindDistance(first);
        auto argDist = FindDistance(second);

        return argDist >= thisDist;
    }

    GridPosition GridPosition::FindPositionAdjacent(const Direction& dir) const
    {
        GridPosition newPos(*this);

        switch (dir.Get())
        {
        case Direction::Value::UP:
            --newPos.y;
            break;
        case Direction::Value::DOWN:
            ++newPos.y;
            break;
        case Direction::Value::LEFT:
            --newPos.x;
            break;
        case Direction::Value::RIGHT:
            ++newPos.x;
            break;
        case Direction::Value::Z_UP:
            ++newPos.z;
            break;
        case Direction::Value::Z_DOWN:
            --newPos.z;
            break;
        }

        return newPos;
    }

    RelativeGridPosition GridPosition::Difference(const GridPosition& against) const
    {
        return RelativeGridPosition(against.x - x, against.y - y, against.z - z);
    }

    GridPosition GridPosition::FromScreen(const Position2D& position, Value z, const Position2D& topLeftScreen)
    {
        return GridPosition(
            DimensionFromPosition(position.x + topLeftScreen.x),
            DimensionFromPosition(position.y + topLeftScreen.y),
            z);
    }

    GridPosition GridPosition::FromScreen(const Position3D& position, const Position2D& topLeftScreen)
    {
        return GridPosition(
            DimensionFromPosition(position.x + topLeftScreen.x),
            DimensionFromPosition(position.y + topLeftScreen.y),
            DimensionFromPosition(position.z));
    }

    GridPosition::Value GridPosition::DimensionFromPosition(Position2D::Value dim)
    {
        return static_cast<GridPosition::Value>(std::floor(dim / GRID_SIZE<Position3D::Value>));
    }

    Position2D::Value GridPosition::DimensionToPosition(Value dim)
    {
        return static_cast<Position2D::Value>((dim * GRID_SIZE<Value>) + (GRID_SIZE<Value> / 2));
    }

    RelativeGridPosition::RelativeGridPosition(ValueT x, ValueT y, ValueT z) :
        x(x), y(y), z(z)
    {}

    RelativeGridPosition::RelativeGridPosition(const GridPosition& source, const GridPosition& destination) :
        x(destination.x - source.x), y(destination.y - source.y), z(destination.z - source.z)
    {}

    bool RelativeGridPosition::operator==(const RelativeGridPosition& arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool RelativeGridPosition::operator!=(const RelativeGridPosition& arg) const
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

namespace Inscription
{
    void Scribe<::Atmos::GridPosition, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
        archive(object.z);
    }

    void Scribe<::Atmos::RelativeGridPosition, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
        archive(object.z);
    }
}