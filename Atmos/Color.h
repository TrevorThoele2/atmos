
#pragma once

#include "Serialization.h"

namespace Atmos
{
    class Color
    {
    public:
        typedef unsigned char ValueT;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        static const ValueT defaultValue = 255;
    public:
        ValueT alpha, red, green, blue;

        Color(ValueT alpha = defaultValue, ValueT red = defaultValue, ValueT green = defaultValue, ValueT blue = defaultValue);
        Color(const Color &arg);

        Color& operator=(const Color &arg);
        bool operator==(const Color &arg) const;
        bool operator!=(const Color &arg) const;

        void Edit(ValueT red, ValueT green, ValueT blue);
        void Edit(ValueT alpha, ValueT red, ValueT green, ValueT blue);

        void SetAlpha(ValueT setTo);
        void SetRed(ValueT setTo);
        void SetGreen(ValueT setTo);
        void SetBlue(ValueT setTo);

        ValueT GetAlpha() const;
        ValueT GetRed() const;
        ValueT GetGreen() const;
        ValueT GetBlue() const;
    };
}