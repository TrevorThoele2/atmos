#pragma once

#include <optional>

#include "Point3D.h"
#include "Angle2D.h"
#include "Vector3D.h"

namespace Atmos::Spatial
{
    struct Line3D
    {
        Point3D first, second;

        bool operator==(const Line3D& arg) const;
        bool operator!=(const Line3D& arg) const;

        explicit operator Vector3D() const;

        [[nodiscard]] Point3D::Value Length() const;
        [[nodiscard]] Angle2D Angle(const Line3D& other) const;
        [[nodiscard]] Point3D::Value Dot(const Line3D& other) const;
        [[nodiscard]] Vector3D Cross(const Line3D& other) const;
        [[nodiscard]] Line3D TranslateToOrigin() const;

        [[nodiscard]] Vector3D Direction() const;

        // Returns true if the lines intersect, false otherwise
        [[nodiscard]] bool CheckIntersect(const Line3D& other) const;
        // If infinite is false, then it will not return the intersection point unless the segments intersect
        // Will return invalid Optional if the lines overlap
        // Otherwise will return the intersection point if the lines will ever intersect at some point
        [[nodiscard]] std::optional<Point3D> IntersectionPoint(const Line3D& other, bool infinite = false) const;

        [[nodiscard]] Point3D::Value DeltaX() const;
        [[nodiscard]] Point3D::Value DeltaY() const;
        [[nodiscard]] Point3D::Value DeltaZ() const;
    };
}