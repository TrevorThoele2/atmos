#pragma once

#include <optional>
#include "Point2D.h"
#include "Angle2D.h"
#include "Vector2D.h"

namespace Atmos::Spatial
{
    struct Line2D
    {
        Point2D first, second;

        bool operator==(const Line2D& arg) const;
        bool operator!=(const Line2D& arg) const;

        explicit operator Vector2D() const;

        // Will return an invalid optional if the slope is undefined (vertical line)
        [[nodiscard]] std::optional<Point2D::Value> Slope() const;
        // Will return an invalid optional if the slope is undefined (vertical line)
        [[nodiscard]] std::optional<Point2D::Value> YIntercept() const;
        [[nodiscard]] Point2D::Value Length() const;
        // Returns the angle from the positive X axis
        [[nodiscard]] Spatial::Angle2D Angle() const;
        [[nodiscard]] Spatial::Angle2D Angle(const Line2D& other) const;
        [[nodiscard]] Point2D::Value Dot(const Line2D& other) const;
        // Returns a 2D cross product of the lines
        [[nodiscard]] Point2D::Value Cross(const Line2D& other) const;

        [[nodiscard]] Vector2D Direction() const;

        // Returns true if the lines intersect, false otherwise
        [[nodiscard]] bool CheckIntersect(const Line2D& other) const;
        // If infinite is false, then it will not return the intersection point unless the segments intersect
        // Otherwise will return the intersection point if the lines will ever intersect at some point
        [[nodiscard]] std::optional<Point2D> IntersectionPoint(const Line2D& other, bool infinite = false) const;

        [[nodiscard]] Point2D::Value DeltaX() const;
        [[nodiscard]] Point2D::Value DeltaY() const;
    };
}