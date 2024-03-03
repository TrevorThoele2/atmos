#pragma once

#include "Position3D.h"
#include "Angle.h"
#include "Vector3D.h"
#include "Optional.h"

namespace Atmos
{
    class Line3D
    {
    public:
        Position3D first, second;

        Line3D() = default;
        Line3D(const Position3D &first, const Position3D &second);
        Line3D(const Line3D &arg) = default;
        Line3D& operator=(const Line3D &arg) = default;
        explicit operator Vector3D() const;
        bool operator==(const Line3D &arg) const;
        bool operator!=(const Line3D &arg) const;

        Position3D::ValueT Length() const;
        Angle Angle(const Line3D &other) const;
        Position3D::ValueT Dot(const Line3D &other) const;
        Vector3D Cross(const Line3D &other) const;
        Line3D TranslateToOrigin() const;

        Vector3D Direction() const;

        // Returns true if the lines intersect, false otherwise
        bool CheckIntersect(const Line3D &other) const;
        // If infinite is false, then it will not return the intersection point unless the segments intersect
        // Will return invalid Optional if the lines overlap
        // Otherwise will return the intersection point if the lines will ever intersect at some point
        Optional<Position3D> IntersectionPoint(const Line3D &other, bool infinite = false) const;

        Position3D::ValueT DeltaX() const;
        Position3D::ValueT DeltaY() const;
        Position3D::ValueT DeltaZ() const;
    };
}