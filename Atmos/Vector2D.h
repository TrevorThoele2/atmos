#pragma once

#include "Position2D.h"
#include "Angle.h"

namespace Atmos
{
    class Vector2D
    {
    public:
        typedef float ValueT;
    public:
        ValueT x, y;

        Vector2D();
        Vector2D(ValueT x, ValueT y);
        Vector2D(const Vector2D &arg) = default;
        Vector2D& operator=(const Vector2D &arg) = default;

        bool operator==(const Vector2D &arg) const;
        bool operator!=(const Vector2D &arg) const;

        void Scale(ValueT value);
        ValueT Length() const;
        Angle Angle(const Vector2D &other) const;
        ValueT Dot(const Vector2D &other) const;
        // Returns a 2D cross product
        ValueT Cross(const Vector2D &other) const;
    };
}