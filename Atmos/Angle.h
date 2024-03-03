#pragma once

#include "Serialization.h"

namespace Atmos
{
    class Angle
    {
    public:
        typedef float ValueT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT GetConvertToRadians() const;
        ValueT GetConvertToDegrees() const;
        ValueT ConditionalConvert(const Angle &other) const;
    public:
        enum Type
        {
            DEGREES,
            RADIANS
        };

        Type type;
    public:
        ValueT value;

        Angle();
        Angle(Type type, ValueT value);
        Angle(const Angle &arg);
        Angle& operator=(const Angle &arg);
        Angle operator+(const Angle &arg) const;
        Angle& operator+=(const Angle &arg);
        Angle operator-(const Angle &arg) const;
        Angle& operator-=(const Angle &arg);
        Angle operator*(const Angle &arg) const;
        Angle& operator*=(const Angle &arg);
        Angle operator/(const Angle &arg) const;
        Angle& operator/=(const Angle &arg);
        bool operator==(const Angle &arg) const;
        bool operator!=(const Angle &arg) const;
        bool operator<(const Angle &arg) const;
        bool operator<=(const Angle &arg) const;
        bool operator>(const Angle &arg) const;
        bool operator>=(const Angle &arg) const;

        void ConvertToRadians();
        void ConvertToDegrees();

        ValueT AsRadians() const;
        ValueT AsDegrees() const;

        bool IsRadians() const;
        bool IsDegrees() const;
    };
}