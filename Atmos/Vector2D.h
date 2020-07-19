#pragma once

#include "Point2D.h"
#include "Angle2D.h"

namespace Atmos::Spatial
{
    struct Vector2D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;

        bool operator==(const Vector2D& arg) const;
        bool operator!=(const Vector2D& arg) const;

        void Scale(Value value);
        [[nodiscard]] Value Length() const;
        [[nodiscard]] Angle2D Angle(const Vector2D& other) const;
        [[nodiscard]] Value Dot(const Vector2D& other) const;
        [[nodiscard]] Value Cross(const Vector2D& other) const;
    };
}