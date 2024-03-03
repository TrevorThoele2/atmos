
#include "Line3D.h"

namespace Atmos
{
    Line3D::Line3D(const Position3D &first, const Position3D &second) : first(first), second(second)
    {}

    Line3D::operator Vector3D() const
    {
        return Vector3D(second.GetX() - first.GetX(), second.GetY() - first.GetY(), second.GetZ() - first.GetZ());
    }

    bool Line3D::operator==(const Line3D &arg) const
    {
        return first == arg.first && second == arg.second;
    }

    bool Line3D::operator!=(const Line3D &arg) const
    {
        return !(*this == arg);
    }

    Position3D::ValueT Line3D::Length() const
    {
        return std::sqrt(std::pow(second.GetX() - first.GetX(), 2) + std::pow(second.GetY() - first.GetY(), 2) + std::pow(second.GetZ() - first.GetZ(), 2));
    }

    Angle Line3D::Angle(const Line3D &other) const
    {
        // Angle = arccos(dot product / (magnitude1 * magnitude2))
        return ::Atmos::Angle(Radians(std::acos(Dot(other) / ( Length() * other.Length()))));
    }

    Position3D::ValueT Line3D::Dot(const Line3D &other) const
    {
        const Line3D line1(TranslateToOrigin());
        const Line3D line2(other.TranslateToOrigin());
        return (line1.second.GetX() * line2.second.GetX()) + (line1.second.GetY() * line2.second.GetY()) + (line1.second.GetZ() * line2.second.GetZ());
    }

    Vector3D Line3D::Cross(const Line3D &other) const
    {
        // (y1 * z2 - y2 * z1) - (x1 * z2 - x2 * z1) + (x1 * y2 - x2 * y1)
        return Vector3D(*this).Cross(Vector3D(other));
    }

    Line3D Line3D::TranslateToOrigin() const
    {
        return Line3D(Position3D(0, 0, 0), Position3D(second.GetX() - first.GetX(), second.GetY() - first.GetY(), second.GetZ() - first.GetZ()));
    }

    Vector3D Line3D::Direction() const
    {
        return Vector3D(second.GetX() - first.GetX(), second.GetY() - first.GetY(), second.GetZ() - first.GetZ());
    }

    bool Line3D::CheckIntersect(const Line3D &other) const
    {
        return true;
    }

    Optional<Position3D> Line3D::IntersectionPoint(const Line3D &other, bool infinite) const
    {
        typedef Optional<Position3D> RetT;
        // a(V1 x V2) = (P2-P1) x V2

        // Check if lines overlap completely
        if (first == other.first && second == other.second)
            return RetT();
        else if (first == other.first || first == other.second)
            return RetT(first);
        else if (second == other.first || second == other.second)
            return RetT(second);

        const Vector3D vector1(Direction());
        const Vector3D vector2(other.Direction());
        // Check parallelity
        if (vector1 == vector2)
            return RetT();

        // V1 x V2
        auto cross1 = vector1.Cross(vector2);
        // (P2 - P1) x V2
        auto cross2 = Vector3D(other.first.GetX() - first.GetX(), other.first.GetY() - first.GetY(), other.first.GetZ() - first.GetZ()).Cross(vector2);

        Vector3D::ValueT a = 0;
        if (cross1.x != 0)
            a = cross2.x / cross1.x;
        else if (cross1.y != 0)
            a = cross2.y / cross1.y;
        else
            a = cross2.z / cross1.z;

        // P1 + aV1
        return RetT(Position3D(first.GetX() + (a * vector1.x), first.GetY() + (a * vector1.y), first.GetZ() + (a * vector1.z)));
    }

    Position3D::ValueT Line3D::DeltaX() const
    {
        return std::abs(first.GetX() - second.GetX());
    }

    Position3D::ValueT Line3D::DeltaY() const
    {
        return std::abs(first.GetY() - second.GetY());
    }

    Position3D::ValueT Line3D::DeltaZ() const
    {
        return std::abs(first.GetZ() - second.GetZ());
    }
}