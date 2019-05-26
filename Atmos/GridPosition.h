
#pragma once

#include "Direction.h"
#include "Position2D.h"
#include "Position3D.h"

#include "Math.h"

#include "Serialization.h"

namespace Atmos
{
    class RelativeGridPosition;

    class GridPosition
    {
    public:
        typedef int Value;
    public:
        Value x, y, z;
    public:
        GridPosition(Value x = 0, Value y = 0, Value z = 0);
        GridPosition(const GridPosition& source, const RelativeGridPosition& offset);
        GridPosition(const Position2D& pos, Value z);
        GridPosition(const Position3D& pos);
        GridPosition(const GridPosition& arg) = default;

        GridPosition& operator=(const GridPosition& arg) = default;

        bool operator==(const GridPosition &arg) const;
        bool operator!=(const GridPosition &arg) const;

        void Edit(Value x = 0, Value y = 0, Value z = 0);
        void Edit(const RelativeGridPosition& offset);
        void Edit(const GridPosition& source, const RelativeGridPosition& offset);
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
        GridPosition FindOffset(const RelativeGridPosition& offset) const;

        GridPosition::Value FindXDistance(const GridPosition& destination) const;
        GridPosition::Value FindYDistance(const GridPosition& destination) const;
        GridPosition::Value FindZDistance(const GridPosition& destination) const;
        unsigned int FindDistance(const GridPosition& destination) const;
        Direction DetermineDirection(const GridPosition& ending) const;
        // Returns true if first is closer, false if second
        bool IsCloser(const GridPosition& first, const GridPosition& second) const;
        GridPosition FindPositionAdjacent(const Direction& dir) const;

        RelativeGridPosition Difference(const GridPosition& against) const;

        static GridPosition FromScreen(const Position2D& position, Value z, const Position2D& topLeftScreen);
        static GridPosition FromScreen(const Position3D& position, const Position2D& topLeftScreen);
        static Value DimensionFromPosition(Position3D::Value dim);
        static Position3D::Value DimensionToPosition(Value dim);
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::GridPosition>
    {
        typedef ::Atmos::GridPosition argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<argument_type::Value>()(arg.x));
            const result_type second(std::hash<argument_type::Value>()(arg.y));
            const result_type third(std::hash<argument_type::Value>()(arg.z));
            return first ^ (second << 1) ^ (third >> 1);
        }
    };
}

namespace Atmos
{
    class RelativeGridPosition
    {
    public:
        typedef GridPosition::Value ValueT;
    public:
        ValueT x, y, z;
    public:
        RelativeGridPosition(ValueT x = 0, ValueT y = 0, ValueT z = 0);
        RelativeGridPosition(const GridPosition &source, const GridPosition &destination);
        bool operator==(const RelativeGridPosition &arg) const;
        bool operator!=(const RelativeGridPosition &arg) const;
        void Edit(ValueT x = 0, ValueT y = 0, ValueT z = 0);
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::RelativeGridPosition>
    {
        typedef ::Atmos::RelativeGridPosition argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<argument_type::ValueT>()(arg.x));
            const result_type second(std::hash<argument_type::ValueT>()(arg.y));
            const result_type third(std::hash<argument_type::ValueT>()(arg.z));
            return first ^ (second << 1) ^ (third >> 1);
        }
    };
}

namespace Atmos
{
    template<class Cont>
    void FindPosSquarePlane(Cont& positions, const GridPosition& topLeft, GridPosition::Value range)
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
    void GetSurroundingPlane(Cont& toFill, const GridPosition& pos)
    {
        // Surrounding tiles will always be 3x3 square
        FindPosSquarePlane(toFill, GridPosition(pos.x - 1, pos.y - 1, pos.z), 3);
    }
}