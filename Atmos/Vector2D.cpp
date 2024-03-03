
#include "Vector2D.h"

namespace Atmos
{
    Vector2D::Vector2D() : x(0.0f), y(0.0f)
    {}

    Vector2D::Vector2D(ValueT x, ValueT y) : x(x), y(y)
    {}

    bool Vector2D::operator==(const Vector2D &arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Vector2D::operator!=(const Vector2D &arg) const
    {
        return !(*this == arg);
    }

    void Vector2D::Scale(ValueT value)
    {
        x *= value;
        y *= value;
    }

    Vector2D::ValueT Vector2D::Length() const
    {
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    }

    Angle Vector2D::Angle(const Vector2D &other) const
    {
        return ::Atmos::Angle(Angle::RADIANS, std::atan2(y, x));
    }

    Vector2D::ValueT Vector2D::Dot(const Vector2D &other) const
    {
        return (x * other.x) + (y * other.y);
    }

    Vector2D::ValueT Vector2D::Cross(const Vector2D &other) const
    {
        return (x * other.y) - (other.x * y);
    }
}