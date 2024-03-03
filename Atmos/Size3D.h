#pragma once

#include "Angle.h"

#include "Join.h"

#include "Serialization.h"

namespace Atmos
{
    class Size3D
    {
    public: typedef float ValueT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT width, height, depth, realWidth, realHeight, realDepth;
        ValueT xScaler, yScaler, zScaler;
        Angle xRotation, yRotation, zRotation;

        void Calc();
    public:
        Size3D(ValueT width = 0.0f, ValueT height = 0.0f, ValueT depth = 0.0f, ValueT xScaler = 1.0f, ValueT yScaler = 1.0f, ValueT zScaler = 1.0f, const Angle &xRotation = Angle(), const Angle &yRotation = Angle(), const Angle &zRotation = Angle());
        Size3D(const Size3D &arg);
        Size3D& operator=(const Size3D &arg);
        bool operator==(const Size3D &arg) const;
        bool operator!=(const Size3D &arg) const;

        void SetWidth(ValueT setTo);
        void SetHeight(ValueT setTo);
        void SetDepth(ValueT setTo);

        ValueT GetWidth() const;
        ValueT GetHeight() const;
        ValueT GetDepth() const;
        ValueT GetScaledWidth() const;
        ValueT GetScaledHeight() const;
        ValueT GetScaledDepth() const;

        void SetXScaler(ValueT setTo);
        void SetYScaler(ValueT setTo);
        void SetZScaler(ValueT setTo);

        Join3<ValueT> GetScalers() const;
        ValueT GetXScaler() const;
        ValueT GetYScaler() const;
        ValueT GetZScaler() const;

        void SetXRotation(const Angle &setTo);
        void SetYRotation(const Angle &setTo);
        void SetZRotation(const Angle &setTo);
        const Angle& GetXRotation() const;
        const Angle& GetYRotation() const;
        const Angle& GetZRotation() const;
    };
}