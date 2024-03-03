
#include "Acumen.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Acumen)
    {
        scribe(value);
    }

    Acumen::Acumen() : value(0)
    {}

    Acumen::Acumen(ValueT value) : value(value)
    {}

    Acumen& Acumen::operator=(ValueT value)
    {
        this->value = value;
        return *this;
    }

    bool Acumen::operator==(const Acumen &arg) const
    {
        return value == arg.value;
    }

    bool Acumen::operator!=(const Acumen &arg) const
    {
        return !(*this == arg);
    }

    Acumen Acumen::operator+(const Acumen &arg) const
    {
        return Acumen(value + arg.value);
    }

    Acumen Acumen::operator+(ValueT value) const
    {
        return Acumen(this->value + value);
    }

    Acumen Acumen::operator-(const Acumen &arg) const
    {
        return Acumen(value - arg.value);
    }

    Acumen Acumen::operator-(ValueT value) const
    {
        return Acumen(this->value - value);
    }

    Acumen& Acumen::operator+=(const Acumen &arg)
    {
        value += arg.value;
        return *this;
    }

    Acumen& Acumen::operator+=(ValueT value)
    {
        this->value += value;
        return *this;
    }

    Acumen& Acumen::operator-=(const Acumen &arg)
    {
        value -= arg.value;
        return *this;
    }

    Acumen& Acumen::operator-=(ValueT value)
    {
        this->value -= value;
        return *this;
    }

    Acumen::operator ValueT() const
    {
        return value;
    }

    void Acumen::Set(ValueT set)
    {
        value = set;
    }

    Acumen::ValueT Acumen::Get() const
    {
        return value;
    }
}