#pragma once

#include "ReadonlyProperty.h"

#include "Serialization.h"

namespace Atmos
{
    class Size3D
    {
    public:
        typedef float Value;

        Value baseWidth;
        Value baseHeight;
        Value baseDepth;

        typedef float Scaler;

        Scaler widthScaler;
        Scaler heightScaler;
        Scaler depthScaler;

        typedef ReadonlyProperty<Value> ScaledValueProperty;

        const Value scaledWidth = ScaledValueProperty(
            [this]() { return baseWidth * widthScaler; });
        const Value scaledHeight = ScaledValueProperty(
            [this]() { return baseHeight * heightScaler; });
        const Value scaledDepth = ScaledValueProperty(
            [this]() { return baseDepth * depthScaler; });
    public:
        Size3D();
        Size3D(Value baseWidth, Value baseHeight, Value baseDepth);
        Size3D(Value baseWidth, Value baseHeight, Value baseDepth, Scaler widthScaler, Scaler heightScaler, Scaler depthScaler);
        Size3D(const Size3D& arg);

        Size3D& operator=(const Size3D& arg);

        bool operator==(const Size3D& arg) const;
        bool operator!=(const Size3D& arg) const;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}