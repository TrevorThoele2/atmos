
#include "Percentage.h"

#include "Overflow.h"

namespace Atmos
{
    const Percentage::Wrapped Percentage::max(Percentage::Wrapped::Split(100, 0));
    const Percentage::Wrapped Percentage::min(Percentage::Wrapped::Split(0, 0));

    Percentage::Percentage(const Wrapped::Split& split) : wrapped(split)
    {}

    Percentage& Percentage::operator=(const Wrapped::Split& split)
    {
        wrapped = split;
        return *this;
    }

    bool Percentage::operator==(const Percentage& arg) const
    {
        return wrapped == arg.wrapped;
    }

    bool Percentage::operator==(const Wrapped& arg) const
    {
        return wrapped == arg;
    }

    bool Percentage::operator==(const Split& arg) const
    {
        return wrapped == arg;
    }

    bool Percentage::operator!=(const Percentage& arg) const
    {
        return !(*this == arg);
    }

    bool Percentage::operator!=(const Wrapped& arg) const
    {
        return !(*this == arg);
    }

    bool Percentage::operator!=(const Split& arg) const
    {
        return !(*this == arg);
    }

    bool Percentage::operator<(const Percentage& arg) const
    {
        return wrapped < arg.wrapped;
    }

    bool Percentage::operator<(const Wrapped& arg) const
    {
        return wrapped < arg;
    }

    bool Percentage::operator<(const Split& arg) const
    {
        return wrapped < arg;
    }

    bool Percentage::operator<=(const Percentage& arg) const
    {
        return *this < arg || *this == arg;
    }

    bool Percentage::operator<=(const Wrapped& arg) const
    {
        return *this < arg || *this == arg;
    }

    bool Percentage::operator<=(const Split& arg) const
    {
        return *this < arg || *this == arg;
    }

    bool Percentage::operator>(const Percentage& arg) const
    {
        return wrapped > arg.wrapped;
    }

    bool Percentage::operator>(const Wrapped& arg) const
    {
        return wrapped > arg;
    }

    bool Percentage::operator>(const Split& arg) const
    {
        return wrapped > arg;
    }

    bool Percentage::operator>=(const Percentage& arg) const
    {
        return *this > arg || *this == arg;
    }

    bool Percentage::operator>=(const Wrapped& arg) const
    {
        return *this > arg || *this == arg;
    }

    bool Percentage::operator>=(const Split& arg) const
    {
        return *this > arg || *this == arg;
    }

    Percentage Percentage::operator+(const Percentage& arg) const
    {
        if (OverflowAdd(wrapped, arg.wrapped, Wrapped(max)))
            return Percentage(max);
        else if (UnderflowAdd(wrapped, arg.wrapped, Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped + arg.wrapped);
    }

    Percentage Percentage::operator+(const Wrapped& arg) const
    {
        if (OverflowAdd(wrapped, arg, Wrapped(max)))
            return Percentage(max);
        else if (UnderflowAdd(wrapped, arg, Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped + arg);
    }

    Percentage Percentage::operator+(const Split& arg) const
    {
        if (OverflowAdd(wrapped, Wrapped(arg), Wrapped(max)))
            return Percentage(max);
        else if (UnderflowAdd(wrapped, Wrapped(arg), Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped + arg);
    }

    Percentage Percentage::operator-(const Percentage& arg) const
    {
        if (OverflowSubtract(wrapped, arg.wrapped, Wrapped(max)))
            return Percentage(max);
        else if (UnderflowSubtract(wrapped, arg.wrapped, Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped - arg.wrapped);
    }

    Percentage Percentage::operator-(const Wrapped& arg) const
    {
        if (OverflowSubtract(wrapped, arg, Wrapped(max)))
            return Percentage(max);
        else if (UnderflowSubtract(wrapped, arg, Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped - arg);
    }

    Percentage Percentage::operator-(const Split& arg) const
    {
        if (OverflowSubtract(wrapped, Wrapped(arg), Wrapped(max)))
            return Percentage(max);
        else if (UnderflowSubtract(wrapped, Wrapped(arg), Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped - Wrapped(arg));
    }

    Percentage Percentage::operator*(const Percentage& arg) const
    {
        if (OverflowMultiply(wrapped, arg.wrapped, Wrapped(max)))
            return Percentage(max);
        else if (UnderflowMultiply(wrapped, arg.wrapped, Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped * arg.wrapped);
    }

    Percentage Percentage::operator*(const Wrapped& arg) const
    {
        if (OverflowMultiply(wrapped, arg, Wrapped(max)))
            return Percentage(max);
        else if (UnderflowMultiply(wrapped, arg, Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped * arg);
    }

    Percentage Percentage::operator*(const Split& arg) const
    {
        if (OverflowMultiply(wrapped, Wrapped(arg), Wrapped(max)))
            return Percentage(max);
        else if (UnderflowMultiply(wrapped, Wrapped(arg), Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped * Wrapped(arg));
    }

    Percentage Percentage::operator/(const Percentage& arg) const
    {
        if (OverflowDivide(wrapped, arg.wrapped, Wrapped(max)))
            return Percentage(max);
        else if (UnderflowDivide(wrapped, arg.wrapped, Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped / arg.wrapped);
    }

    Percentage Percentage::operator/(const Wrapped& arg) const
    {
        if (OverflowDivide(wrapped, arg, Wrapped(max)))
            return Percentage(max);
        else if (UnderflowDivide(wrapped, arg, Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped / arg);
    }

    Percentage Percentage::operator/(const Split& arg) const
    {
        if (OverflowDivide(wrapped, Wrapped(arg), Wrapped(max)))
            return Percentage(max);
        else if (UnderflowDivide(wrapped, Wrapped(arg), Wrapped(min)))
            return Percentage(min);

        return Percentage(wrapped / Wrapped(arg));
    }

    Percentage& Percentage::operator+=(const Percentage& arg)
    {
        if (OverflowAdd(wrapped, arg.wrapped, Wrapped(max)))
            wrapped = max;
        else if (UnderflowAdd(wrapped, arg.wrapped, Wrapped(min)))
            wrapped = min;
        else
            wrapped += arg.wrapped;

        return *this;
    }

    Percentage& Percentage::operator+=(const Wrapped& arg)
    {
        if (OverflowAdd(wrapped, arg, Wrapped(max)))
            wrapped = max;
        else if (UnderflowAdd(wrapped, arg, Wrapped(min)))
            wrapped = min;
        else
            wrapped += arg;

        return *this;
    }

    Percentage& Percentage::operator+=(const Split& arg)
    {
        if (OverflowAdd(wrapped, Wrapped(arg), Wrapped(max)))
            wrapped = max;
        else if (UnderflowAdd(wrapped, Wrapped(arg), Wrapped(min)))
            wrapped = min;
        else
            wrapped += Wrapped(arg);

        return *this;
    }

    Percentage& Percentage::operator-=(const Percentage& arg)
    {
        if (OverflowSubtract(wrapped, arg.wrapped, Wrapped(max)))
            wrapped = max;
        else if (UnderflowSubtract(wrapped, arg.wrapped, Wrapped(min)))
            wrapped = min;
        else
            wrapped -= arg.wrapped;

        return *this;
    }

    Percentage& Percentage::operator-=(const Wrapped& arg)
    {
        if (OverflowSubtract(wrapped, arg, Wrapped(max)))
            wrapped = max;
        else if (UnderflowSubtract(wrapped, arg, Wrapped(min)))
            wrapped = min;
        else
            wrapped -= arg;

        return *this;
    }

    Percentage& Percentage::operator-=(const Split& arg)
    {
        if (OverflowSubtract(wrapped, Wrapped(arg), Wrapped(max)))
            wrapped = max;
        else if (UnderflowSubtract(wrapped, Wrapped(arg), Wrapped(min)))
            wrapped = min;
        else
            wrapped -= arg;

        return *this;
    }

    Percentage& Percentage::operator*=(const Percentage& arg)
    {
        if (OverflowMultiply(wrapped, arg.wrapped, Wrapped(max)))
            wrapped = max;
        else if (UnderflowMultiply(wrapped, arg.wrapped, Wrapped(min)))
            wrapped = min;
        else
            wrapped *= arg.wrapped;

        return *this;
    }

    Percentage& Percentage::operator*=(const Wrapped& arg)
    {
        if (OverflowMultiply(wrapped, arg, Wrapped(max)))
            wrapped = max;
        else if (UnderflowMultiply(wrapped, arg, Wrapped(min)))
            wrapped = min;
        else
            wrapped *= arg;

        return *this;
    }

    Percentage& Percentage::operator*=(const Split& arg)
    {
        if (OverflowMultiply(wrapped, Wrapped(arg), Wrapped(max)))
            wrapped = max;
        else if (UnderflowMultiply(wrapped, Wrapped(arg), Wrapped(min)))
            wrapped = min;
        else
            wrapped *= Wrapped(arg);

        return *this;
    }

    Percentage& Percentage::operator/=(const Percentage& arg)
    {
        if (OverflowDivide(wrapped, arg.wrapped, Wrapped(max)))
            wrapped = max;
        else if (UnderflowDivide(wrapped, arg.wrapped, Wrapped(min)))
            wrapped = min;
        else
            wrapped /= arg.wrapped;

        return *this;
    }

    Percentage& Percentage::operator/=(const Wrapped& arg)
    {
        if (OverflowDivide(wrapped, arg, Wrapped(max)))
            wrapped = max;
        else if (UnderflowDivide(wrapped, arg, Wrapped(min)))
            wrapped = min;
        else
            wrapped /= arg;

        return *this;
    }

    Percentage& Percentage::operator/=(const Split& arg)
    {
        if (OverflowDivide(wrapped, Wrapped(arg), Wrapped(max)))
            wrapped = max;
        else if (UnderflowDivide(wrapped, Wrapped(arg), Wrapped(min)))
            wrapped = min;
        else
            wrapped /= Wrapped(arg);

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

    Percentage::operator Wrapped() const
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

namespace Inscription
{
    void Scribe<::Atmos::Percentage, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.wrapped);
    }
}