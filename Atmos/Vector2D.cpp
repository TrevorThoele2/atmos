#include "Vector2D.h"

namespace Atmos::Spatial
{
    bool Vector2D::operator==(const Vector2D& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Vector2D::operator!=(const Vector2D& arg) const
    {
        return !(*this == arg);
    }

    void Vector2D::Scale(Value value)
    {
        x *= value;
        y *= value;
    }

    Vector2D::Value Vector2D::Length() const
    {
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    }

    Angle2D Vector2D::Angle(const Vector2D& other) const
    {
        return Spatial::Angle2D(std::atan2(y, x));
    }

    Vector2D::Value Vector2D::Dot(const Vector2D& other) const
    {
        return (x * other.x) + (y * other.y);
    }

    Vector2D::Value Vector2D::Cross(const Vector2D& other) const
    {
        return (x * other.y) - (other.x * y);
    }
}