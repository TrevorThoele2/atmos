#pragma once

#include <vector>
#include <set>

#include "Serialization.h"

namespace Atmos
{
    class Position2D;
    class AxisBoundingBox3D;
    class Position3D
    {
    public:
        typedef float ValueT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT realX, realY, realZ;
        ValueT X, Y, Z;
        bool pixelPerfect;

        void Calc();
    public:
        Position3D(ValueT X = 0, ValueT Y = 0, ValueT Z = 0, bool pixelPerfect = false);
        Position3D(const Position2D &pos, ValueT Z);
        Position3D(const Position3D &arg);
        Position3D& operator=(const Position3D &arg);
        bool operator==(const Position3D &arg) const;
        bool operator!=(const Position3D &arg) const;
        explicit operator Position2D() const;

        void Set(ValueT X, ValueT Y, ValueT Z);
        void SetX(ValueT set);
        void IncrementX(ValueT inc);
        void DecrementX(ValueT dec);
        void SetY(ValueT set);
        void IncrementY(ValueT inc);
        void DecrementY(ValueT dec);
        void SetZ(ValueT set);
        void IncrementZ(ValueT inc);
        void DecrementZ(ValueT dec);
        void SetPixelPerfect(bool set = true);

        ValueT GetX() const;
        ValueT GetY() const;
        ValueT GetZ() const;
        bool GetPixelPerfect() const;

        bool Within(ValueT left, ValueT right, ValueT top, ValueT bottom, ValueT nearZ, ValueT farZ) const;
        bool Within(const AxisBoundingBox3D &box) const;

        static Position3D FromScreen(const Position3D &convert, const Position3D &topLeftScreen, ValueT z);
        static ValueT FindDistance(const Position3D &starting, const Position3D &destination);
        static Position3D FindCenter(const std::vector<Position3D> &container);
        static Position3D FindCenter(const Position3D &nearTopLeft, const Position3D &nearBottomRight, const Position3D &farTopLeft);
    };
}