
#pragma once

#include "Direction.h"
#include "Position2D.h"
#include "Position3D.h"

#include "Math.h"

#include "Serialization.h"

namespace Atmos
{
    struct RelativeGridPosition;
    class GridPosition
    {
    public:
        typedef int ValueT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        ValueT x, y, z;

        GridPosition(ValueT x = 0, ValueT y = 0, ValueT z = 0);
        GridPosition(const GridPosition &source, const RelativeGridPosition &offset);
        GridPosition(const Position2D &pos, ValueT z);
        GridPosition(const Position3D &pos);
        GridPosition(const GridPosition &arg) = default;
        GridPosition& operator=(const GridPosition &arg) = default;
        bool operator==(const GridPosition &arg) const;
        bool operator!=(const GridPosition &arg) const;
        void Edit(ValueT x = 0, ValueT y = 0, ValueT z = 0);
        void Edit(const RelativeGridPosition &offset);
        void Edit(const GridPosition &source, const RelativeGridPosition &offset);
        void Edit(const Position2D &pos, ValueT z);
        void Edit(const Position3D &pos);
        void SetX(ValueT set);
        void SetY(ValueT set);
        void SetZ(ValueT set);
        operator Position2D() const;
        operator Position3D() const;

        ValueT GetX() const;
        ValueT GetY() const;
        ValueT GetZ() const;
        GridPosition FindOffset(const RelativeGridPosition &offset) const;

        GridPosition::ValueT FindXDistance(const GridPosition &destination) const;
        GridPosition::ValueT FindYDistance(const GridPosition &destination) const;
        GridPosition::ValueT FindZDistance(const GridPosition &destination) const;
        unsigned int FindDistance(const GridPosition &destination) const;
        Direction DetermineDirection(const GridPosition &ending) const;
        // Returns true if first is closer, false if second
        bool IsCloser(const GridPosition &first, const GridPosition &second) const;
        GridPosition FindPositionAdjacent(const Direction &dir) const;

        RelativeGridPosition Difference(const GridPosition &against) const;

        static GridPosition FromScreen(const Position2D &position, ValueT z, const Position2D &topLeftScreen);
        static GridPosition FromScreen(const Position3D &position, const Position2D &topLeftScreen);
        static ValueT DimensionFromPosition(Position3D::ValueT dim);
        static Position3D::ValueT DimensionToPosition(ValueT dim);
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
            const result_type first(std::hash<argument_type::ValueT>()(arg.x));
            const result_type second(std::hash<argument_type::ValueT>()(arg.y));
            const result_type third(std::hash<argument_type::ValueT>()(arg.z));
            return first ^ (second << 1) ^ (third >> 1);
        }
    };
}

namespace Atmos
{
    struct RelativeGridPosition
    {
    public:
        typedef GridPosition::ValueT ValueT;
    private:
        void Serialize(::Inscription::Scribe &scribe);
        INSCRIPTION_ACCESS;
    public:
        ValueT x, y, z;

        RelativeGridPosition(ValueT x = 0, ValueT y = 0, ValueT z = 0);
        RelativeGridPosition(const GridPosition &source, const GridPosition &destination);
        bool operator==(const RelativeGridPosition &arg) const;
        bool operator!=(const RelativeGridPosition &arg) const;
        void Edit(ValueT x = 0, ValueT y = 0, ValueT z = 0);
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
    void FindPosSquarePlane(Cont &positions, const GridPosition &topLeft, GridPosition::ValueT range)
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
    void GetSurroundingPlane(Cont &toFill, const GridPosition &pos)
    {
        // Surrounding tiles will always be 3x3 square
        FindPosSquarePlane(toFill, GridPosition(pos.x - 1, pos.y - 1, pos.z), 3);
    }

    template<class Itr>
    Position3D FindCenterTile(Itr begin, Itr end)
    {
        auto nearTopLeft = begin;
        auto nearBottomRight = begin;
        auto farTopLeft = begin;

        for (auto loop = begin; loop != end; ++loop)
        {
            // Check col
            if (loop->x < nearTopLeft->x)
                nearTopLeft->x = loop->x;
            else if (loop->x > nearBottomRight->x)
                nearBottomRight->x = loop->x;

            // Check row
            if (loop->y < nearTopLeft.y)
                nearTopLeft->y = loop->y;
            else if (loop->y > nearBottomRight->y)
                nearBottomRight->y = loop->y;

            // Check Z
            if (loop->z < nearTopLeft.z)
                nearTopLeft->z = loop->z;
            else if (loop->z > farTopLeft->z)
                farTopLeft->z = loop->z;
        }

        return Position(FindMidpoint(nearTopLeft.x, nearBottomRight.x) * TILE_WIDTH, FindMidpoint(nearTopLeft.y, nearBottomRight.y) * TILE_WIDTH, FindMidpoint(nearTopLeft.z, farTopLeft.z));
    }
}