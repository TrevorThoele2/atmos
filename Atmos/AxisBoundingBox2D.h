
#pragma once

#include "Position2D.h"
#include "Size2D.h"

#include "Serialization.h"

namespace Atmos
{
    class AxisBoundingBox2D
    {
    public:
        typedef float Coordinate;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        Coordinate left, right, top, bottom;
        Coordinate realLeft, realRight, realTop, realBottom;
        bool pixelPerfect;

        Position2D center;

        void Calc();
    public:
        AxisBoundingBox2D(bool pixelPerfect = true);
        AxisBoundingBox2D(Coordinate left, Coordinate top, Coordinate right, Coordinate bottom, bool pixelPerfect = true);
        bool operator==(const AxisBoundingBox2D &arg) const;
        bool operator!=(const AxisBoundingBox2D &arg) const;

        void SetLeft(Coordinate setTo);
        void SetRight(Coordinate setTo);
        void SetTop(Coordinate setTo);
        void SetBottom(Coordinate setTo);
        void Set(Coordinate top, Coordinate bottom, Coordinate left, Coordinate right);

        void IncLeft(Coordinate toInc);
        void IncRight(Coordinate toInc);
        void IncTop(Coordinate toInc);
        void IncBottom(Coordinate toInc);

        Coordinate GetLeft() const;
        Coordinate GetRight() const;
        Coordinate GetTop() const;
        Coordinate GetBottom() const;
        const Position2D& GetCenter() const;
        Coordinate GetWidth() const;
        Coordinate GetHeight() const;
        Size2D GetSize() const;

        void SetPixelPerfect(bool setTo);
        // Returns true if the position is inside the AABB
        bool IsHit(const Position2D &check) const;
        // Returns true if the position is inside the AABB
        bool Within(const Position2D &check) const;
        // Returns true if this AABB is completely enclosed in the box
        bool Within(const AxisBoundingBox2D &box) const;
        // Returns true if the box overlaps any part of this AABB
        bool Overlapping(const AxisBoundingBox2D &box) const;
    };
}