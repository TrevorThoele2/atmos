#pragma once

#include "Position2D.h"
#include "Angle.h"

namespace Atmos
{
    class Vector2D
    {
    public:
        typedef float Value;
    public:
        Value x, y;
    public:
        Vector2D();
        Vector2D(Value x, Value y);
        Vector2D(const Vector2D& arg) = default;

        Vector2D& operator=(const Vector2D& arg) = default;

        bool operator==(const Vector2D& arg) const;
        bool operator!=(const Vector2D& arg) const;

        void Scale(Value value);
        Value Length() const;
        Angle Angle(const Vector2D& other) const;
        Value Dot(const Vector2D& other) const;
        // Returns a 2D cross product
        Value Cross(const Vector2D& other) const;
    };
}