#pragma once

#include "Serialization.h"

namespace Atmos
{
    template<class Mixin>
    class AngleUnits
    {
    public:
        typedef float ValueT;
    public:
        AngleUnits();
        AngleUnits(ValueT value);

        virtual ~AngleUnits() = 0;

        Mixin& operator=(ValueT arg);

        Mixin operator+(const Mixin& arg) const;
        Mixin operator+(ValueT arg) const;
        Mixin& operator+=(const Mixin& arg);
        Mixin& operator+=(ValueT arg);
        Mixin operator-(const Mixin& arg) const;
        Mixin operator-(ValueT arg) const;
        Mixin& operator-=(const Mixin& arg);
        Mixin& operator-=(ValueT arg);
        Mixin operator*(const Mixin& arg) const;
        Mixin operator*(ValueT arg) const;
        Mixin& operator*=(const Mixin& arg);
        Mixin& operator*=(ValueT arg);
        Mixin operator/(const Mixin& arg) const;
        Mixin operator/(ValueT arg) const;
        Mixin& operator/=(const Mixin& arg);
        Mixin& operator/=(ValueT arg);

        operator ValueT() const;
    private:
        ValueT value;
    private:
        Mixin& ThisToMixin();
        const Mixin& ThisToMixin() const;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class Mixin>
    AngleUnits<Mixin>::AngleUnits() : value(0)
    {}

    template<class Mixin>
    AngleUnits<Mixin>::AngleUnits(ValueT value) : value(value)
    {}

    template<class Mixin>
    AngleUnits<Mixin>::~AngleUnits()
    {}

    template<class Mixin>
    Mixin& AngleUnits<Mixin>::operator=(ValueT arg)
    {
        value = arg;
        return ThisToMixin();
    }

    template<class Mixin>
    Mixin AngleUnits<Mixin>::operator+(const Mixin& arg) const
    {
        return Mixin(value + arg.value);
    }

    template<class Mixin>
    Mixin AngleUnits<Mixin>::operator+(ValueT arg) const
    {
        return Mixin(value + arg);
    }

    template<class Mixin>
    Mixin& AngleUnits<Mixin>::operator+=(const Mixin& arg)
    {
        value += arg.value;
        return ThisToMixin();
    }

    template<class Mixin>
    Mixin& AngleUnits<Mixin>::operator+=(ValueT arg)
    {
        value += arg;
        return ThisToMixin();
    }

    template<class Mixin>
    Mixin AngleUnits<Mixin>::operator-(const Mixin& arg) const
    {
        return Mixin(value - arg.value);
    }

    template<class Mixin>
    Mixin AngleUnits<Mixin>::operator-(ValueT arg) const
    {
        return Mixin(value + arg);
    }

    template<class Mixin>
    Mixin& AngleUnits<Mixin>::operator-=(const Mixin& arg)
    {
        value -= arg.value;
        return ThisToMixin();
    }

    template<class Mixin>
    Mixin& AngleUnits<Mixin>::operator-=(ValueT arg)
    {
        value -= arg;
        return ThisToMixin();
    }

    template<class Mixin>
    Mixin AngleUnits<Mixin>::operator*(const Mixin& arg) const
    {
        return Mixin(value * arg.value);
    }

    template<class Mixin>
    Mixin AngleUnits<Mixin>::operator*(ValueT arg) const
    {
        return Mixin(value + arg);
    }

    template<class Mixin>
    Mixin& AngleUnits<Mixin>::operator*=(const Mixin& arg)
    {
        value *= arg.value;
        return ThisToMixin();
    }

    template<class Mixin>
    Mixin& AngleUnits<Mixin>::operator*=(ValueT arg)
    {
        value *= arg;
        return ThisToMixin();
    }

    template<class Mixin>
    Mixin AngleUnits<Mixin>::operator/(const Mixin& arg) const
    {
        return Mixin(value / arg.value);
    }

    template<class Mixin>
    Mixin AngleUnits<Mixin>::operator/(ValueT arg) const
    {
        return Mixin(value + arg);
    }

    template<class Mixin>
    Mixin& AngleUnits<Mixin>::operator/=(const Mixin& arg)
    {
        value /= arg.value;
        return ThisToMixin();
    }

    template<class Mixin>
    Mixin& AngleUnits<Mixin>::operator/=(ValueT arg)
    {
        value += arg;
        return ThisToMixin();
    }

    template<class Mixin>
    AngleUnits<Mixin>::operator ValueT() const
    {
        return value;
    }

    template<class Mixin>
    Mixin& AngleUnits<Mixin>::ThisToMixin()
    {
        return static_cast<Mixin&>(*this);
    }

    template<class Mixin>
    const Mixin& AngleUnits<Mixin>::ThisToMixin() const
    {
        return static_cast<const Mixin&>(*this);
    }

    template<class Mixin>
    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(AngleUnits<Mixin>)
    {
        scribe(value);
    }
}