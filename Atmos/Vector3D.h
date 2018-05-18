#pragma once

#include "Angle.h"

namespace Atmos
{
    class Vector3D
    {
    public:
        typedef float ValueT;
    public:
        float x, y, z;

        Vector3D();
        Vector3D(ValueT x, ValueT y, ValueT z);
        Vector3D(const Vector3D &arg) = default;
        Vector3D& operator=(const Vector3D &arg) = default;

        bool operator==(const Vector3D &arg) const;
        bool operator!=(const Vector3D &arg) const;

        void Scale(ValueT value);
        ValueT Length() const;
        Angle Angle(const Vector3D &other) const;
        ValueT Dot(const Vector3D &other) const;
        Vector3D Cross(const Vector3D &other) const;
    };
}