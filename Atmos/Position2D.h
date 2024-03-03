
#pragma once

#include <vector>
#include <set>

#include "Range.h"
#include "Serialization.h"

namespace Atmos
{
    class Position3D;
    class AxisBoundingBox2D;

    class Position2D
    {
    public:
        typedef float ValueT;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT realX, realY;
        ValueT X, Y;
        bool pixelPerfect;

        void Calc();
    public:
        Position2D(ValueT X = 0, ValueT Y = 0, bool pixelPerfect = false);
        Position2D(const Position3D &arg);
        Position2D(const Position2D &arg);
        Position2D& operator=(const Position2D &arg);
        bool operator==(const Position2D &arg) const;
        bool operator!=(const Position2D &arg) const;

        void Set(ValueT X, ValueT Y);
        void SetX(ValueT set);
        void IncrementX(ValueT inc);
        void DecrementX(ValueT dec);
        void SetY(ValueT set);
        void IncrementY(ValueT inc);
        void DecrementY(ValueT dec);
        void SetPixelPerfect(bool set = true);

        ValueT GetX() const;
        ValueT GetY() const;
        bool GetPixelPerfect() const;

        bool Within(ValueT left, ValueT right, ValueT top, ValueT bottom) const;
        bool Within(const AxisBoundingBox2D &box) const;

        static Position2D FromScreen(const Position2D &convert, const Position2D &topLeftScreen);
        static ValueT FindDistance(const Position2D &starting, const Position2D &destination);
        static Position2D FindCenter(const std::vector<Position2D> &container);
        static Position2D FindCenter(const std::set<Position2D> &container);
        static Position2D FindCenter(const Position2D &topLeft, const Position2D &bottomRight);
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::Position2D>
    {
        typedef ::Atmos::Position2D argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<argument_type::ValueT>()(arg.GetX()));
            const result_type second(std::hash<argument_type::ValueT>()(arg.GetY()));
            return first ^ (second << 1);
        }
    };
}