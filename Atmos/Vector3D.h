#pragma once

#include "Angle.h"

namespace Atmos
{
    class Vector3D
    {
    public:
        typedef float Value;
    public:
        Value x, y, z;
    public:
        Vector3D();
        Vector3D(Value x, Value y, Value z);
        Vector3D(const Vector3D& arg) = default;

        Vector3D& operator=(const Vector3D& arg) = default;

        bool operator==(const Vector3D& arg) const;
        bool operator!=(const Vector3D& arg) const;

        void Scale(Value value);
        Value Length() const;
        Angle Angle(const Vector3D& other) const;
        Value Dot(const Vector3D& other) const;
        Vector3D Cross(const Vector3D& other) const;
    };
}