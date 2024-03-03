#pragma once

#include "Position2D.h"
#include "Angle.h"
#include "Vector2D.h"
#include "Optional.h"

namespace Atmos
{
    class Line2D
    {
    public:
        Position2D first, second;

        Line2D() = default;
        Line2D(const Position2D &first, const Position2D &second);
        Line2D(const Line2D &arg) = default;
        Line2D& operator=(const Line2D &arg) = default;
        explicit operator Vector2D() const;
        bool operator==(const Line2D &arg) const;
        bool operator!=(const Line2D &arg) const;

        // Will return an invalid optional if the slope is undefined (vertical line)
        Optional<Position2D::ValueT> Slope() const;
        // Will return an invalid optional if the slope is undefined (vertical line)
        Optional<Position2D::ValueT> YIntercept() const;
        Position2D::ValueT Length() const;
        // Returns the angle from the positive X axis
        ::Atmos::Angle Angle() const;
        ::Atmos::Angle Angle(const Line2D &other) const;
        Position2D::ValueT Dot(const Line2D &other) const;
        // Returns a 2D cross product of the lines
        Position2D::ValueT Cross(const Line2D &other) const;

        Vector2D Direction() const;

        // Returns true if the lines intersect, false otherwise
        bool CheckIntersect(const Line2D &other) const;
        // If infinite is false, then it will not return the intersection point unless the segments intersect
        // Otherwise will return the intersection point if the lines will ever intersect at some point
        Optional<Position2D> IntersectionPoint(const Line2D &other, bool infinite = false) const;

        Position2D::ValueT DeltaX() const;
        Position2D::ValueT DeltaY() const;
    };
}