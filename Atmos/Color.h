
#pragma once

#include "Serialization.h"

namespace Atmos
{
    class Color
    {
    public:
        typedef unsigned char Value;
    public:
        Value alpha, red, green, blue;

        Color();
        Color(Value alpha, Value red, Value green, Value blue);
        Color(const Color& arg);

        Color& operator=(const Color& arg);
        bool operator==(const Color& arg) const;
        bool operator!=(const Color& arg) const;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}