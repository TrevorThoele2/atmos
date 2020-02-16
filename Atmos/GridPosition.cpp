#include "GridPosition.h"

#include "GridCellSize.h"

#include <Inscription/Scribe.h>

namespace Atmos::Grid
{
    Position::Position(Value x, Value y, Value z) :
        x(x), y(y), z(z)
    {}

    Position::Position(const Position& source, const RelativePosition& offset) :
        x(source.x + offset.x), y(source.y + offset.y), z(source.z + offset.z)
    {}

    Position::Position(const Position2D& position, Value z) :
        x(DimensionFromPosition(position.x)), y(DimensionFromPosition(position.y)), z(z)
    {}

    Position::Position(const Position3D& pos) :
        x(DimensionFromPosition(pos.x)), y(DimensionFromPosition(pos.y)), z(DimensionFromPosition(pos.z))
    {}

    bool Position::operator==(const Position& arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool Position::operator!=(const Position& arg) const
    {
        return !(*this == arg);
    }

    void Position::Edit(Value x, Value y, Value z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void Position::Edit(const RelativePosition& offset)
    {
        x += offset.x;
        y += offset.y;
        z += offset.z;
    }

    void Position::Edit(const Position& source, const RelativePosition& offset)
    {
        x = source.x + offset.x;
        y = source.y + offset.y;
        z = source.z + offset.z;
    }

    void Position::Edit(const Position2D& pos, Value z)
    {
        x = DimensionFromPosition(pos.x);
        y = DimensionFromPosition(pos.y);
        this->z = z;
    }

    void Position::Edit(const Position3D& pos)
    {
        x = DimensionFromPosition(pos.x);
        y = DimensionFromPosition(pos.y);
        z = DimensionFromPosition(pos.z);
    }

    void Position::SetX(Value set)
    {
        x = set;
    }

    void Position::SetY(Value set)
    {
        y = set;
    }

    void Position::SetZ(Value set)
    {
        z = set;
    }

    Position::operator Position2D() const
    {
        return Position2D{ DimensionToPosition(x), DimensionToPosition(y) };
    }

    Position::operator Position3D() const
    {
        return Position3D{ DimensionToPosition(x), DimensionToPosition(y), DimensionToPosition(z) };
    }

    Position::Value Position::GetX() const
    {
        return x;
    }

    Position::Value Position::GetY() const
    {
        return y;
    }

    Position::Value Position::GetZ() const
    {
        return z;
    }

    Position Position::FindOffset(const RelativePosition& offset) const
    {
        Position ret;
        ret.Edit(*this, offset);
        return ret;
    }

    Position::Value Position::FindXDistance(const Position& destination) const
    {
        return destination.x - x;
    }

    Position::Value Position::FindYDistance(const Position& destination) const
    {
        return destination.y - y;
    }

    Position::Value Position::FindZDistance(const Position& destination) const
    {
        return destination.z - z;
    }

    unsigned int Position::FindDistance(const Position& destination) const
    {
        return
            std::abs(FindXDistance(destination)) +
            std::abs(FindYDistance(destination)) +
            std::abs(FindZDistance(destination));
    }

    Direction Position::DetermineDirection(const Position& ending) const
    {
        if (ending.x < x)
            return Direction::Value::Left;
        else if (ending.x > x)
            return Direction::Value::Right;
        else if (ending.y < y)
            return Direction::Value::Up;
        else if (ending.y > y)
            return Direction::Value::Down;
        else if (ending.z > z)
            return Direction::Value::ZUp;
        else
            return Direction::Value::ZDown;
    }

    bool Position::IsCloser(const Position& first, const Position& second) const
    {
        const auto thisDist = FindDistance(first);
        const auto argDist = FindDistance(second);

        return argDist >= thisDist;
    }

    Position Position::FindPositionAdjacent(const Direction& dir) const
    {
        Position newPos(*this);

        switch (dir.Get())
        {
        case Direction::Value::Up:
            --newPos.y;
            break;
        case Direction::Value::Down:
            ++newPos.y;
            break;
        case Direction::Value::Left:
            --newPos.x;
            break;
        case Direction::Value::Right:
            ++newPos.x;
            break;
        case Direction::Value::ZUp:
            ++newPos.z;
            break;
        case Direction::Value::ZDown:
            --newPos.z;
            break;
        }

        return newPos;
    }

    RelativePosition Position::Difference(const Position& against) const
    {
        return RelativePosition(against.x - x, against.y - y, against.z - z);
    }

    Position Position::FromScreen(const Position2D& position, Value z, const Position2D& topLeftScreen)
    {
        return Position(
            DimensionFromPosition(position.x + topLeftScreen.x),
            DimensionFromPosition(position.y + topLeftScreen.y),
            z);
    }

    Position Position::FromScreen(const Position3D& position, const Position2D& topLeftScreen)
    {
        return Position(
            DimensionFromPosition(position.x + topLeftScreen.x),
            DimensionFromPosition(position.y + topLeftScreen.y),
            DimensionFromPosition(position.z));
    }

    Position::Value Position::DimensionFromPosition(Position2D::Value dim)
    {
        return static_cast<Value>(std::floor(dim / CellSize<Position3D::Value>));
    }

    Position2D::Value Position::DimensionToPosition(Value dim)
    {
        return static_cast<Position2D::Value>((dim * CellSize<Value>) + (CellSize<Value> / 2));
    }

    RelativePosition::RelativePosition() :
        x(0), y(0), z(0)
    {}

    RelativePosition::RelativePosition(Value x, Value y, Value z) :
        x(x), y(y), z(z)
    {}

    RelativePosition::RelativePosition(const Position& source, const Position& destination) :
        x(destination.x - source.x), y(destination.y - source.y), z(destination.z - source.z)
    {}

    bool RelativePosition::operator==(const RelativePosition& arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool RelativePosition::operator!=(const RelativePosition& arg) const
    {
        return !(*this == arg);
    }

    void RelativePosition::Edit(Value x, Value y, Value z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Grid::Position, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
        archive(object.z);
    }

    void Scribe<::Atmos::Grid::RelativePosition, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
        archive(object.z);
    }
}