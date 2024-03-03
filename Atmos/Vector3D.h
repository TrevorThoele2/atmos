#pragma once

#include "Angle.h"

namespace Atmos
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
        [[nodiscard]] Angle Angle(const Vector3D& other) const;
        [[nodiscard]] Value Dot(const Vector3D& other) const;
        [[nodiscard]] Vector3D Cross(const Vector3D& other) const;
    };
}