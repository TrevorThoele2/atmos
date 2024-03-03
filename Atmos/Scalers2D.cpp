#include "Scalers2D.h"

namespace Atmos
{
    Scalers2D::Scalers2D() : x(1.0f), y(1.0f)
    {}

    Scalers2D::Scalers2D(Value x, Value y) : x(x), y(y)
    {}

    Scalers2D& Scalers2D::operator=(const Scalers2D& arg)
    {
        x = arg.x;
        y = arg.y;
        return *this;
    }

    bool Scalers2D::operator==(const Scalers2D& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Scalers2D::operator!=(const Scalers2D& arg) const
    {
        return (*this == arg);
    }
}