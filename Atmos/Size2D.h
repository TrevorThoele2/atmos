#pragma once

#include "ReadonlyProperty.h"

#include "Serialization.h"

namespace Atmos
{
    class Size2D
    {
    public:
        typedef float Value;

        Value baseWidth;
        Value baseHeight;

        typedef float Scaler;

        Scaler widthScaler;
        Scaler heightScaler;

        typedef ReadonlyProperty<Value> ScaledValueProperty;

        const Value scaledWidth = ScaledValueProperty(
            [this]() { return baseWidth * widthScaler; });
        const Value scaledHeight = ScaledValueProperty(
            [this]() { return baseHeight * heightScaler; });
    public:
        Size2D();
        Size2D(Value baseWidth, Value baseHeight);
        Size2D(Value baseWidth, Value baseHeight, Scaler widthScaler, Scaler heightScaler);
        Size2D(const Size2D& arg);

        Size2D& operator=(const Size2D& arg);

        bool operator==(const Size2D& arg) const;
        bool operator!=(const Size2D& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Size2D, BinaryArchive> : public CompositeScribe<::Atmos::Size2D, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}