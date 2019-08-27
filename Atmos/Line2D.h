#pragma once

#include <optional>
#include "Position2D.h"
#include "Angle.h"
#include "Vector2D.h"

namespace Atmos
{
    struct Line2D
    {
        Position2D first, second;

        bool operator==(const Line2D& arg) const;
        bool operator!=(const Line2D& arg) const;

        explicit operator Vector2D() const;

        // Will return an invalid optional if the slope is undefined (vertical line)
        [[nodiscard]] std::optional<Position2D::Value> Slope() const;
        // Will return an invalid optional if the slope is undefined (vertical line)
        [[nodiscard]] std::optional<Position2D::Value> YIntercept() const;
        [[nodiscard]] Position2D::Value Length() const;
        // Returns the angle from the positive X axis
        [[nodiscard]] ::Atmos::Angle Angle() const;
        [[nodiscard]] ::Atmos::Angle Angle(const Line2D& other) const;
        [[nodiscard]] Position2D::Value Dot(const Line2D& other) const;
        // Returns a 2D cross product of the lines
        [[nodiscard]] Position2D::Value Cross(const Line2D& other) const;

        [[nodiscard]] Vector2D Direction() const;

        // Returns true if the lines intersect, false otherwise
        [[nodiscard]] bool CheckIntersect(const Line2D& other) const;
        // If infinite is false, then it will not return the intersection point unless the segments intersect
        // Otherwise will return the intersection point if the lines will ever intersect at some point
        [[nodiscard]] std::optional<Position2D> IntersectionPoint(const Line2D& other, bool infinite = false) const;

        [[nodiscard]] Position2D::Value DeltaX() const;
        [[nodiscard]] Position2D::Value DeltaY() const;
    };
}