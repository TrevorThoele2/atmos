
#pragma once

#include "Angle.h"

#include "Serialization.h"

namespace Atmos
{
    class Size2D
    {
    public:
        typedef float ValueT;
    public:
        struct Scalers
        {
            ValueT x;
            ValueT y;
        };
    public:
        Size2D(ValueT width = 0.0f, ValueT height = 0.0f, ValueT xScaler = 1.0f, ValueT yScaler = 1.0f, const Angle& rotation = Angle());
        Size2D(const Size2D &arg);

        Size2D& operator=(const Size2D& arg);

        bool operator==(const Size2D& arg) const;
        bool operator!=(const Size2D& arg) const;

        void SetWidth(ValueT setTo);
        void SetHeight(ValueT setTo);

        ValueT GetWidth() const;
        ValueT GetHeight() const;
        ValueT GetScaledWidth() const;
        ValueT GetScaledHeight() const;

        void SetXScaler(ValueT setTo);
        void SetYScaler(ValueT setTo);

        Scalers GetScalers() const;
        ValueT GetXScaler() const;
        ValueT GetYScaler() const;

        void SetRotation(const Angle& setTo);
        const Angle& GetRotation() const;
    private:
        ValueT width, height, realWidth, realHeight;
        ValueT xScaler, yScaler;
        Angle rotation;

        void Calculate();
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}