#pragma once

#include "Direction.h"
#include "Position2D.h"
#include "Position3D.h"

#include "Math.h"

#include "Serialization.h"

namespace Atmos::Grid
{
    class RelativePosition;

    class Position
    {
    public:
        using Value = int;
    public:
        Value x, y, z;
    public:
        Position(Value x = 0, Value y = 0, Value z = 0);
        Position(const Position& source, const RelativePosition& offset);
        Position(const Position2D& pos, Value z);
        Position(const Position3D& pos);
        Position(const Position& arg) = default;

        Position& operator=(const Position& arg) = default;

        bool operator==(const Position &arg) const;
        bool operator!=(const Position &arg) const;

        void Edit(Value x = 0, Value y = 0, Value z = 0);
        void Edit(const RelativePosition& offset);
        void Edit(const Position& source, const RelativePosition& offset);
        void Edit(const Position2D& pos, Value z);
        void Edit(const Position3D& pos);
        void SetX(Value set);
        void SetY(Value set);
        void SetZ(Value set);

        operator Position2D() const;
        operator Position3D() const;

        Value GetX() const;
        Value GetY() const;
        Value GetZ() const;
        Position FindOffset(const RelativePosition& offset) const;

        Position::Value FindXDistance(const Position& destination) const;
        Position::Value FindYDistance(const Position& destination) const;
        Position::Value FindZDistance(const Position& destination) const;
        unsigned int FindDistance(const Position& destination) const;
        Direction DetermineDirection(const Position& ending) const;
        // Returns true if first is closer, false if second
        bool IsCloser(const Position& first, const Position& second) const;
        Position FindPositionAdjacent(const Direction& dir) const;

        RelativePosition Difference(const Position& against) const;

        static Position FromScreen(const Position2D& position, Value z, const Position2D& topLeftScreen);
        static Position FromScreen(const Position3D& position, const Position2D& topLeftScreen);
        static Value DimensionFromPosition(Position3D::Value dim);
        static Position3D::Value DimensionToPosition(Value dim);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Grid::Position, BinaryArchive> :
        public CompositeScribe<::Atmos::Grid::Position, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::Atmos::Grid::RelativePosition, BinaryArchive> :
        public CompositeScribe<::Atmos::Grid::RelativePosition, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::Grid::Position>
    {
        using argument_type = ::Atmos::Grid::Position;
        using result_type = std::size_t;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<argument_type::Value>()(arg.x));
            const result_type second(std::hash<argument_type::Value>()(arg.y));
            const result_type third(std::hash<argument_type::Value>()(arg.z));
            return first ^ (second << 1) ^ (third >> 1);
        }
    };
}

namespace Atmos::Grid
{
    class RelativePosition
    {
    public:
        using Value = Position::Value;
    public:
        Value x, y, z;
    public:
        RelativePosition();
        RelativePosition(Value x, Value y, Value z);
        RelativePosition(const Position &source, const Position &destination);
        bool operator==(const RelativePosition &arg) const;
        bool operator!=(const RelativePosition &arg) const;
        void Edit(Value x = 0, Value y = 0, Value z = 0);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::Grid::RelativePosition>
    {
        using argument_type = ::Atmos::Grid::RelativePosition;
        using result_type = std::size_t;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<argument_type::Value>()(arg.x));
            const result_type second(std::hash<argument_type::Value>()(arg.y));
            const result_type third(std::hash<argument_type::Value>()(arg.z));
            return first ^ (second << 1) ^ (third >> 1);
        }
    };
}

namespace Atmos::Grid
{
    template<class Cont>
    void FindPosSquarePlane(Cont& positions, const Position& topLeft, Position::Value range)
    {
        positions.clear();

        auto pos = topLeft;
        auto realRange = range - 1;
        GridPosition bottomRight(pos.x + realRange, pos.y + realRange, pos.z);

        while (pos != bottomRight)
        {
            positions.emplace(pos);
            pos.x++;

            // Check if the position's column is past the ending position
            if (pos.x > bottomRight.x)
            {
                pos.x = topLeft.x;
                pos.y++;
            }
        }

        // Place in last one (loop will skip over the last one)
        positions.emplace(pos);
    }

    template<class Cont>
    void GetSurroundingPlane(Cont& toFill, const Position& pos)
    {
        // Surrounding tiles will always be 3x3 square
        FindPosSquarePlane(toFill, Position(pos.x - 1, pos.y - 1, pos.z), 3);
    }
}