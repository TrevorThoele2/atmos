
#include "Angle.h"
#include "MathUtility.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Angle)
    {
        scribe(type);
        scribe(value);
    }

    Angle::ValueT Angle::GetConvertToRadians() const
    {
        if (type == RADIANS)
            return value;

        return (value * PI<float>) / 180.0f;
    }

    Angle::ValueT Angle::GetConvertToDegrees() const
    {
        if (type == DEGREES)
            return value;

        return (value * 180.0f) / PI<float>;
    }

    Angle::ValueT Angle::ConditionalConvert(const Angle &other) const
    {
        if (type == other.type)
            return other.value;
        else
            return (type == DEGREES) ? other.GetConvertToDegrees() : other.GetConvertToRadians();
    }

    Angle::Angle() : type(RADIANS), value(0.0f)
    {}

    Angle::Angle(Type type, ValueT value) : type(type), value(value)
    {}

    Angle::Angle(const Angle &arg) : type(arg.type), value(arg.value)
    {}

    Angle& Angle::operator=(const Angle &arg)
    {
        value = ConditionalConvert(arg);
        type = arg.type;
        return *this;
    }

    Angle Angle::operator+(const Angle &arg) const
    {
        return Angle(type, value + ConditionalConvert(arg));
    }

    Angle& Angle::operator+=(const Angle &arg)
    {
        value += ConditionalConvert(arg);
        return *this;
    }

    Angle Angle::operator-(const Angle &arg) const
    {
        return Angle(type, value - ConditionalConvert(arg));
    }

    Angle& Angle::operator-=(const Angle &arg)
    {
        value -= ConditionalConvert(arg);
        return *this;
    }

    Angle Angle::operator*(const Angle &arg) const
    {
        return Angle(type, value * ConditionalConvert(arg));
    }

    Angle& Angle::operator*=(const Angle &arg)
    {
        value *= ConditionalConvert(arg);
        return *this;
    }

    Angle Angle::operator/(const Angle &arg) const
    {
        return Angle(type, value / ConditionalConvert(arg));
    }

    Angle& Angle::operator/=(const Angle &arg)
    {
        value /= ConditionalConvert(arg);
        return *this;
    }

    bool Angle::operator==(const Angle &arg) const
    {
        return type == arg.type && value == arg.value;
    }

    bool Angle::operator!=(const Angle &arg) const
    {
        return !(*this == arg);
    }

    bool Angle::operator<(const Angle &arg) const
    {
        return (type == arg.type) ? value < arg.value : false;
    }

    bool Angle::operator<=(const Angle &arg) const
    {
        return (type == arg.type) ? value <= arg.value : false;
    }

    bool Angle::operator>(const Angle &arg) const
    {
        return (type == arg.type) ? value > arg.value : false;
    }

    bool Angle::operator>=(const Angle &arg) const
    {
        return (type == arg.type) ? value >= arg.value : false;
    }

    void Angle::ConvertToRadians()
    {
        value = GetConvertToRadians();
        type = RADIANS;
    }

    void Angle::ConvertToDegrees()
    {
        value = GetConvertToDegrees();
        type = DEGREES;
    }

    Angle::ValueT Angle::AsRadians() const
    {
        return GetConvertToRadians();
    }

    Angle::ValueT Angle::AsDegrees() const
    {
        return GetConvertToDegrees();
    }

    bool Angle::IsRadians() const
    {
        return type == RADIANS;
    }

    bool Angle::IsDegrees() const
    {
        return type == DEGREES;
    }
}