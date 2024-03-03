#include "Vector3D.h"
#include <cmath>

namespace Atmos::Spatial
{
    bool Vector3D::operator==(const Vector3D& arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool Vector3D::operator!=(const Vector3D& arg) const
    {
        return !(*this == arg);
    }

    void Vector3D::Scale(Value value)
    {
        x *= value;
        y *= value;
        z *= value;
    }

    Vector3D::Value Vector3D::Length() const
    {
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
    }

    Angle2D Vector3D::Angle(const Vector3D& other) const
    {
        // Angle = arccos(dot product / (magnitude1 * magnitude2))
        return std::acos(Dot(other) / (Length() * other.Length()));
    }

    Vector3D::Value Vector3D::Dot(const Vector3D& other) const
    {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    Vector3D Vector3D::Cross(const Vector3D& other) const
    {
        // (y1 * z2 - y2 * z1) - (x1 * z2 - x2 * z1) + (x1 * y2 - x2 * y1)
        return Vector3D
        {
            (y * other.z) - (other.y * z),
            -1 * ((x * other.z) - (other.x * z)),
            (x * other.y) - (other.x * y)
        };
    }
}