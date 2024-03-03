#include "Line3D.h"

namespace Atmos::Spatial
{
    bool Line3D::operator==(const Line3D& arg) const
    {
        return first == arg.first && second == arg.second;
    }

    bool Line3D::operator!=(const Line3D& arg) const
    {
        return !(*this == arg);
    }

    Line3D::operator Vector3D() const
    {
        return Vector3D
        {
            second.x - first.x,
            second.y - first.y,
            second.z - first.z
        };
    }

    Point3D::Value Line3D::Length() const
    {
        return std::sqrt(
            std::pow(second.x - first.x, 2) +
            std::pow(second.y - first.y, 2) +
            std::pow(second.z - first.z, 2));
    }

    Angle Line3D::Angle(const Line3D& other) const
    {
        // Angle = arccos(dot product / (magnitude1 * magnitude2))
        return std::acos(Dot(other) / ( Length() * other.Length()));
    }

    Point3D::Value Line3D::Dot(const Line3D& other) const
    {
        const Line3D line1(TranslateToOrigin());
        const Line3D line2(other.TranslateToOrigin());
        return (line1.second.x * line2.second.x) + (line1.second.y * line2.second.y) + (line1.second.z * line2.second.z);
    }

    Vector3D Line3D::Cross(const Line3D& other) const
    {
        // (y1 * z2 - y2 * z1) - (x1 * z2 - x2 * z1) + (x1 * y2 - x2 * y1)
        return Vector3D(*this).Cross(Vector3D(other));
    }

    Line3D Line3D::TranslateToOrigin() const
    {
        return Line3D{ Point3D{0, 0, 0}, Point3D{second.x - first.x, second.y - first.y, second.z - first.z} };
    }

    Vector3D Line3D::Direction() const
    {
        return Vector3D{ second.x - first.x, second.y - first.y, second.z - first.z };
    }

    bool Line3D::CheckIntersect(const Line3D& other) const
    {
        return true;
    }

    std::optional<Point3D> Line3D::IntersectionPoint(const Line3D& other, bool infinite) const
    {
        // a(V1 x V2) = (P2-P1) x V2

        // Check if lines overlap completely
        if (first == other.first && second == other.second)
            return {};
        else if (first == other.first || first == other.second)
            return { first };
        else if (second == other.first || second == other.second)
            return { second };

        const Vector3D vector1(Direction());
        const Vector3D vector2(other.Direction());
        // Check parallelity
        if (vector1 == vector2)
            return {};

        // V1 x V2
        const auto cross1 = vector1.Cross(vector2);
        // (P2 - P1) x V2
        const auto cross2 = Vector3D
        {
            other.first.x - first.x, other.first.y - first.y, other.first.z - first.z
        }.Cross(vector2);

        Vector3D::Value a = 0;
        if (cross1.x != 0)
            a = cross2.x / cross1.x;
        else if (cross1.y != 0)
            a = cross2.y / cross1.y;
        else
            a = cross2.z / cross1.z;

        // P1 + aV1
        return { Point3D{ first.x + (a * vector1.x), first.y + (a * vector1.y), first.z + (a * vector1.z) } };
    }

    Point3D::Value Line3D::DeltaX() const
    {
        return std::abs(first.x - second.x);
    }

    Point3D::Value Line3D::DeltaY() const
    {
        return std::abs(first.y - second.y);
    }

    Point3D::Value Line3D::DeltaZ() const
    {
        return std::abs(first.z - second.z);
    }
}