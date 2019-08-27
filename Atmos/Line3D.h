#pragma once

#include <optional>

#include "Position3D.h"
#include "Angle.h"
#include "Vector3D.h"

namespace Atmos
{
    struct Line3D
    {
        Position3D first, second;

        bool operator==(const Line3D& arg) const;
        bool operator!=(const Line3D& arg) const;

        explicit operator Vector3D() const;

        [[nodiscard]] Position3D::Value Length() const;
        [[nodiscard]] Angle Angle(const Line3D& other) const;
        [[nodiscard]] Position3D::Value Dot(const Line3D& other) const;
        [[nodiscard]] Vector3D Cross(const Line3D& other) const;
        [[nodiscard]] Line3D TranslateToOrigin() const;

        [[nodiscard]] Vector3D Direction() const;

        // Returns true if the lines intersect, false otherwise
        [[nodiscard]] bool CheckIntersect(const Line3D& other) const;
        // If infinite is false, then it will not return the intersection point unless the segments intersect
        // Will return invalid Optional if the lines overlap
        // Otherwise will return the intersection point if the lines will ever intersect at some point
        std::optional<Position3D> IntersectionPoint(const Line3D& other, bool infinite = false) const;

        [[nodiscard]] Position3D::Value DeltaX() const;
        [[nodiscard]] Position3D::Value DeltaY() const;
        [[nodiscard]] Position3D::Value DeltaZ() const;
    };
}