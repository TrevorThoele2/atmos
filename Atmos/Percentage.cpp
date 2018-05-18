
#include "Percentage.h"

#include "Overflow.h"

namespace Atmos
{
    const Percentage::WrappedT Percentage::max(Percentage::WrappedT::Split(100, 0));
    const Percentage::WrappedT Percentage::min(Percentage::WrappedT::Split(0, 0));

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Percentage)
    {
        scribe(wrapped);
    }

    Percentage::Percentage(const WrappedT::Split &split) : wrapped(split)
    {}

    Percentage& Percentage::operator=(const WrappedT::Split &split)
    {
        wrapped = split;
        return *this;
    }

    bool Percentage::operator==(const Percentage &arg) const
    {
        return wrapped == arg.wrapped;
    }

    bool Percentage::operator==(const WrappedT &arg) const
    {
        return wrapped == arg;
    }

    bool Percentage::operator==(const Split &arg) const
    {
        return wrapped == arg;
    }

    bool Percentage::operator!=(const Percentage &arg) const
    {
        return !(*this == arg);
    }

    bool Percentage::operator!=(const WrappedT &arg) const
    {
        return !(*this == arg);
    }

    bool Percentage::operator!=(const Split &arg) const
    {
        return !(*this == arg);
    }

    bool Percentage::operator<(const Percentage &arg) const
    {
        return wrapped < arg.wrapped;
    }

    bool Percentage::operator<(const WrappedT &arg) const
    {
        return wrapped < arg;
    }

    bool Percentage::operator<(const Split &arg) const
    {
        return wrapped < arg;
    }

    bool Percentage::operator<=(const Percentage &arg) const
    {
        return *this < arg || *this == arg;
    }

    bool Percentage::operator<=(const WrappedT &arg) const
    {
        return *this < arg || *this == arg;
    }

    bool Percentage::operator<=(const Split &arg) const
    {
        return *this < arg || *this == arg;
    }

    bool Percentage::operator>(const Percentage &arg) const
    {
        return wrapped > arg.wrapped;
    }

    bool Percentage::operator>(const WrappedT &arg) const
    {
        return wrapped > arg;
    }

    bool Percentage::operator>(const Split &arg) const
    {
        return wrapped > arg;
    }

    bool Percentage::operator>=(const Percentage &arg) const
    {
        return *this > arg || *this == arg;
    }

    bool Percentage::operator>=(const WrappedT &arg) const
    {
        return *this > arg || *this == arg;
    }

    bool Percentage::operator>=(const Split &arg) const
    {
        return *this > arg || *this == arg;
    }

    Percentage Percentage::operator+(const Percentage &arg) const
    {
        if (OverflowAdd(wrapped, arg.wrapped, WrappedT(max)))
            return Percentage(max);
        else if (UnderflowAdd(wrapped, arg.wrapped, WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped + arg.wrapped);
    }

    Percentage Percentage::operator+(const WrappedT &arg) const
    {
        if (OverflowAdd(wrapped, arg, WrappedT(max)))
            return Percentage(max);
        else if (UnderflowAdd(wrapped, arg, WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped + arg);
    }

    Percentage Percentage::operator+(const Split &arg) const
    {
        if (OverflowAdd(wrapped, WrappedT(arg), WrappedT(max)))
            return Percentage(max);
        else if (UnderflowAdd(wrapped, WrappedT(arg), WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped + arg);
    }

    Percentage Percentage::operator-(const Percentage &arg) const
    {
        if (OverflowSubtract(wrapped, arg.wrapped, WrappedT(max)))
            return Percentage(max);
        else if (UnderflowSubtract(wrapped, arg.wrapped, WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped - arg.wrapped);
    }

    Percentage Percentage::operator-(const WrappedT &arg) const
    {
        if (OverflowSubtract(wrapped, arg, WrappedT(max)))
            return Percentage(max);
        else if (UnderflowSubtract(wrapped, arg, WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped - arg);
    }

    Percentage Percentage::operator-(const Split &arg) const
    {
        if (OverflowSubtract(wrapped, WrappedT(arg), WrappedT(max)))
            return Percentage(max);
        else if (UnderflowSubtract(wrapped, WrappedT(arg), WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped - WrappedT(arg));
    }

    Percentage Percentage::operator*(const Percentage &arg) const
    {
        if (OverflowMultiply(wrapped, arg.wrapped, WrappedT(max)))
            return Percentage(max);
        else if (UnderflowMultiply(wrapped, arg.wrapped, WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped * arg.wrapped);
    }

    Percentage Percentage::operator*(const WrappedT &arg) const
    {
        if (OverflowMultiply(wrapped, arg, WrappedT(max)))
            return Percentage(max);
        else if (UnderflowMultiply(wrapped, arg, WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped * arg);
    }

    Percentage Percentage::operator*(const Split &arg) const
    {
        if (OverflowMultiply(wrapped, WrappedT(arg), WrappedT(max)))
            return Percentage(max);
        else if (UnderflowMultiply(wrapped, WrappedT(arg), WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped * WrappedT(arg));
    }

    Percentage Percentage::operator/(const Percentage &arg) const
    {
        if (OverflowDivide(wrapped, arg.wrapped, WrappedT(max)))
            return Percentage(max);
        else if (UnderflowDivide(wrapped, arg.wrapped, WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped / arg.wrapped);
    }

    Percentage Percentage::operator/(const WrappedT &arg) const
    {
        if (OverflowDivide(wrapped, arg, WrappedT(max)))
            return Percentage(max);
        else if (UnderflowDivide(wrapped, arg, WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped / arg);
    }

    Percentage Percentage::operator/(const Split &arg) const
    {
        if (OverflowDivide(wrapped, WrappedT(arg), WrappedT(max)))
            return Percentage(max);
        else if (UnderflowDivide(wrapped, WrappedT(arg), WrappedT(min)))
            return Percentage(min);

        return Percentage(wrapped / WrappedT(arg));
    }

    Percentage& Percentage::operator+=(const Percentage &arg)
    {
        if (OverflowAdd(wrapped, arg.wrapped, WrappedT(max)))
            wrapped = max;
        else if (UnderflowAdd(wrapped, arg.wrapped, WrappedT(min)))
            wrapped = min;
        else
            wrapped += arg.wrapped;

        return *this;
    }

    Percentage& Percentage::operator+=(const WrappedT &arg)
    {
        if (OverflowAdd(wrapped, arg, WrappedT(max)))
            wrapped = max;
        else if (UnderflowAdd(wrapped, arg, WrappedT(min)))
            wrapped = min;
        else
            wrapped += arg;

        return *this;
    }

    Percentage& Percentage::operator+=(const Split &arg)
    {
        if (OverflowAdd(wrapped, WrappedT(arg), WrappedT(max)))
            wrapped = max;
        else if (UnderflowAdd(wrapped, WrappedT(arg), WrappedT(min)))
            wrapped = min;
        else
            wrapped += WrappedT(arg);

        return *this;
    }

    Percentage& Percentage::operator-=(const Percentage &arg)
    {
        if (OverflowSubtract(wrapped, arg.wrapped, WrappedT(max)))
            wrapped = max;
        else if (UnderflowSubtract(wrapped, arg.wrapped, WrappedT(min)))
            wrapped = min;
        else
            wrapped -= arg.wrapped;

        return *this;
    }

    Percentage& Percentage::operator-=(const WrappedT &arg)
    {
        if (OverflowSubtract(wrapped, arg, WrappedT(max)))
            wrapped = max;
        else if (UnderflowSubtract(wrapped, arg, WrappedT(min)))
            wrapped = min;
        else
            wrapped -= arg;

        return *this;
    }

    Percentage& Percentage::operator-=(const Split &arg)
    {
        if (OverflowSubtract(wrapped, WrappedT(arg), WrappedT(max)))
            wrapped = max;
        else if (UnderflowSubtract(wrapped, WrappedT(arg), WrappedT(min)))
            wrapped = min;
        else
            wrapped -= arg;

        return *this;
    }

    Percentage& Percentage::operator*=(const Percentage &arg)
    {
        if (OverflowMultiply(wrapped, arg.wrapped, WrappedT(max)))
            wrapped = max;
        else if (UnderflowMultiply(wrapped, arg.wrapped, WrappedT(min)))
            wrapped = min;
        else
            wrapped *= arg.wrapped;

        return *this;
    }

    Percentage& Percentage::operator*=(const WrappedT &arg)
    {
        if (OverflowMultiply(wrapped, arg, WrappedT(max)))
            wrapped = max;
        else if (UnderflowMultiply(wrapped, arg, WrappedT(min)))
            wrapped = min;
        else
            wrapped *= arg;

        return *this;
    }

    Percentage& Percentage::operator*=(const Split &arg)
    {
        if (OverflowMultiply(wrapped, WrappedT(arg), WrappedT(max)))
            wrapped = max;
        else if (UnderflowMultiply(wrapped, WrappedT(arg), WrappedT(min)))
            wrapped = min;
        else
            wrapped *= WrappedT(arg);

        return *this;
    }

    Percentage& Percentage::operator/=(const Percentage &arg)
    {
        if (OverflowDivide(wrapped, arg.wrapped, WrappedT(max)))
            wrapped = max;
        else if (UnderflowDivide(wrapped, arg.wrapped, WrappedT(min)))
            wrapped = min;
        else
            wrapped /= arg.wrapped;

        return *this;
    }

    Percentage& Percentage::operator/=(const WrappedT &arg)
    {
        if (OverflowDivide(wrapped, arg, WrappedT(max)))
            wrapped = max;
        else if (UnderflowDivide(wrapped, arg, WrappedT(min)))
            wrapped = min;
        else
            wrapped /= arg;

        return *this;
    }

    Percentage& Percentage::operator/=(const Split &arg)
    {
        if (OverflowDivide(wrapped, WrappedT(arg), WrappedT(max)))
            wrapped = max;
        else if (UnderflowDivide(wrapped, WrappedT(arg), WrappedT(min)))
            wrapped = min;
        else
            wrapped /= WrappedT(arg);

        return *this;
    }

    Percentage& Percentage::operator++()
    {
        ++wrapped;
        return *this;
    }

    Percentage& Percentage::operator--()
    {
        --wrapped;
        return *this;
    }

    Percentage Percentage::operator++(int)
    {
        Percentage hold(*this);
        ++wrapped;
        return hold;
    }

    Percentage Percentage::operator--(int)
    {
        Percentage hold(*this);
        --wrapped;
        return hold;
    }

    Percentage::operator char() const
    {
        return static_cast<char>(wrapped);
    }

    Percentage::operator unsigned char() const
    {
        return static_cast<unsigned char>(wrapped);
    }

    Percentage::operator short() const
    {
        return static_cast<short>(wrapped);
    }

    Percentage::operator unsigned short() const
    {
        return static_cast<unsigned short>(wrapped);
    }

    Percentage::operator int() const
    {
        return static_cast<int>(wrapped);
    }

    Percentage::operator unsigned int() const
    {
        return static_cast<unsigned int>(wrapped);
    }

    Percentage::operator long long() const
    {
        return static_cast<long long>(wrapped);
    }

    Percentage::operator unsigned long long() const
    {
        return static_cast<unsigned long long>(wrapped);
    }

    Percentage::operator float() const
    {
        return static_cast<float>(wrapped);
    }

    Percentage::operator double() const
    {
        return static_cast<double>(wrapped);
    }

    Percentage::operator Split() const
    {
        return static_cast<Split>(wrapped);
    }

    Percentage::operator WrappedT() const
    {
        return wrapped;
    }

    void Percentage::Floor()
    {
        wrapped.Floor();
    }

    void Percentage::Ceiling()
    {
        wrapped.Ceiling();
    }

    Percentage Percentage::ConvertToDecimal() const
    {
        return *this / max;
    }
}