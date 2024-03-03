#pragma once

#include "Angle2D.h"

namespace Atmos::Spatial
{
    struct Vector3D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;
        Value z = 0.0f;

        bool operator==(const Vector3D& arg) const;
        bool operator!=(const Vector3D& arg) const;

        void Scale(Value value);
        [[nodiscard]] Value Length() const;
        [[nodiscard]] Angle2D Angle(const Vector3D& other) const;
        [[nodiscard]] Value Dot(const Vector3D& other) const;
        [[nodiscard]] Vector3D Cross(const Vector3D& other) const;
    };
}