
#include "LargeInteger.h"
#include "DivideByZeroException.h"

namespace Atmos
{
    bool LargeInteger::operator==(const LargeInteger& arg) const
    {
        return low == arg.low && high == arg.high;
    }

    bool LargeInteger::operator!=(const LargeInteger& arg) const
    {
        return !(*this == arg);
    }

    bool LargeInteger::operator<(const LargeInteger& arg) const
    {
        if (high == arg.high)
            return low < arg.low;
        else
            return high < arg.high;
    }

    bool LargeInteger::operator<=(const LargeInteger& arg) const
    {
        return *this < arg || *this == arg;
    }

    bool LargeInteger::operator>(const LargeInteger& arg) const
    {
        return !(*this < arg || *this == arg);
    }

    bool LargeInteger::operator>=(const LargeInteger& arg) const
    {
        return *this > arg || *this == arg;
    }

    LargeInteger::operator bool() const
    {
        return low != 0 || high != 0;
    }

    LargeInteger LargeInteger::operator+(const LargeInteger& arg) const
    {
        LargeInteger result;
        AddImpl(*this, arg, result);
        return result;
    }

    LargeInteger& LargeInteger::operator+=(const LargeInteger& arg)
    {
        AddImpl(*this, arg, *this);
        return *this;
    }

    LargeInteger LargeInteger::operator-(const LargeInteger& arg) const
    {
        LargeInteger result;
        SubtractImpl(*this, arg, result);
        return result;
    }

    LargeInteger& LargeInteger::operator-=(const LargeInteger& arg)
    {
        SubtractImpl(*this, arg, *this);
        return *this;
    }

    LargeInteger LargeInteger::operator*(const LargeInteger& arg) const
    {
        LargeInteger result;
        MultiplyImpl(*this, arg, result);
        return result;
    }

    LargeInteger& LargeInteger::operator*=(const LargeInteger& arg)
    {
        MultiplyImpl(*this, arg, *this);
        return *this;
    }

    LargeInteger LargeInteger::operator/(const LargeInteger& arg) const
    {
        LargeInteger quotient;
        LargeInteger remainder;
        DivideImpl(*this, arg, quotient, remainder);
        return quotient;
    }

    LargeInteger& LargeInteger::operator/=(const LargeInteger& arg)
    {
        LargeInteger quotient;
        LargeInteger remainder;
        DivideImpl(*this, arg, *this, remainder);
        return *this;
    }

    LargeInteger LargeInteger::operator<<(const LargeInteger& arg) const
    {
        if (arg.high > 0 || arg.low > digitCount)
            return LargeInteger();

        LargeInteger ret;
        const WrappedT halfDigitCount = digitCount / 2;
        if (arg.low == halfDigitCount)
        {
            ret.high = low;
            ret.low = 0;
            return ret;
        }
        else if (arg.low >= halfDigitCount)
        {
            ret.high = low;
            ret.low = 0;
            ret.high <<= arg.low - halfDigitCount;
            return ret;
        }

        WrappedT mask = maxWrappedValue << arg.low;

        ret.high <<= arg.low;
        ret.high += (low & mask) >> (halfDigitCount - arg.low);
        ret.low <<= arg.low;

        return ret;
    }

    LargeInteger LargeInteger::operator<<(size_t arg) const
    {
        return *this << LargeInteger(arg);
    }

    LargeInteger& LargeInteger::operator<<=(const LargeInteger& arg)
    {
        if (arg.high > 0 || arg.low > digitCount)
        {
            high = 0;
            low = 0;
            return *this;
        }

        const WrappedT halfDigitCount = digitCount / 2;
        if (arg.low == halfDigitCount)
        {
            high = low;
            low = 0;
            return *this;
        }
        else if (arg.low >= halfDigitCount)
        {
            high = low;
            low = 0;
            high <<= arg.low - halfDigitCount;
            return *this;
        }

        const WrappedT prevLow = low;
        WrappedT mask = maxWrappedValue << arg.low;

        high <<= arg.low;
        high += (prevLow & mask) >> (halfDigitCount - arg.low);
        low <<= arg.low;

        return *this;
    }

    LargeInteger& LargeInteger::operator<<=(size_t arg)
    {
        return *this <<= LargeInteger(arg);
    }

    LargeInteger LargeInteger::operator >> (const LargeInteger& arg) const
    {
        if (arg.high > 0 || arg.low > digitCount)
            return LargeInteger();

        LargeInteger ret;
        const WrappedT halfDigitCount = digitCount / 2;
        if (arg.low == halfDigitCount)
        {
            ret.low = high;
            ret.high = 0;
            return ret;
        }
        else if (arg.low >= halfDigitCount)
        {
            ret.low = high;
            ret.high = 0;
            ret.low >>= arg.low - halfDigitCount;
            return ret;
        }

        WrappedT mask = maxWrappedValue >> arg.low;

        ret.low >>= arg.low;
        ret.low += (high & mask) >> (halfDigitCount - arg.low);
        ret.high >>= arg.low;

        return ret;
    }

    LargeInteger LargeInteger::operator >> (size_t arg) const
    {
        return *this >> LargeInteger(arg);
    }

    LargeInteger& LargeInteger::operator>>=(const LargeInteger& arg)
    {
        if (arg.high > 0 || arg.low > digitCount)
        {
            low = 0;
            high = 0;
            return *this;
        }

        const WrappedT halfDigitCount = digitCount / 2;
        if (arg.low == halfDigitCount)
        {
            low = high;
            high = 0;
            return *this;
        }
        else if (arg.low >= halfDigitCount)
        {
            low = high;
            high = 0;
            low >>= arg.low - halfDigitCount;
            return *this;
        }

        const WrappedT prevHigh = high;
        WrappedT mask = maxWrappedValue >> (halfDigitCount - arg.low);

        low >>= arg.low;
        low += (prevHigh & mask) << (halfDigitCount - arg.low);
        high >>= arg.low;

        return *this;
    }

    LargeInteger& LargeInteger::operator>>=(size_t arg)
    {
        return *this >>= LargeInteger(arg);
    }

    LargeInteger& LargeInteger::operator++()
    {
        ++low;
        if (low == 0)
            ++high;
        return *this;
    }

    LargeInteger& LargeInteger::operator--()
    {
        if (low == 0)
        {
            low = maxWrappedValue;
            (high == 0) ? high = maxWrappedValue : --high;
        }
        else
            --low;

        return *this;
    }

    LargeInteger LargeInteger::operator++(int)
    {
        LargeInteger ret(*this);
        ++*this;
        return ret;
    }

    LargeInteger LargeInteger::operator--(int)
    {
        LargeInteger ret(*this);
        --*this;
        return ret;
    }

    LargeInteger LargeInteger::operator%(const LargeInteger& arg) const
    {
    LargeInteger quotient;
    LargeInteger remainder;
    DivideImpl(*this, arg, quotient, remainder);
    return remainder;
    }

    LargeInteger& LargeInteger::operator%=(const LargeInteger& arg)
    {
        LargeInteger quotient;
        DivideImpl(*this, arg, quotient, *this);
        return *this;
    }

    bool LargeInteger::operator!() const
    {
        return !(low != 0 || high != 0);
    }

    LargeInteger LargeInteger::operator~() const
    {
        LargeInteger ret(*this);
        ret.low = ~ret.low;
        ret.high = ~ret.high;
        return ret;
    }

    LargeInteger LargeInteger::operator|(const LargeInteger& arg) const
    {
        LargeInteger ret(*this);
        ret.low |= arg.low;
        ret.high |= arg.high;
        return ret;
    }

    LargeInteger& LargeInteger::operator|=(const LargeInteger& arg)
    {
        low |= arg.low;
        high |= arg.high;
        return *this;
    }

    LargeInteger LargeInteger::operator&(const LargeInteger& arg) const
    {
        LargeInteger ret(*this);
        ret.low &= arg.low;
        ret.high &= arg.high;
        return ret;
    }

    LargeInteger& LargeInteger::operator&=(const LargeInteger& arg)
    {
        low &= arg.low;
        high &= arg.high;
        return *this;
    }

    LargeInteger LargeInteger::operator^(const LargeInteger& arg) const
    {
        LargeInteger ret(*this);
        ret.low ^= arg.low;
        ret.high ^= arg.high;
        return ret;
    }

    LargeInteger& LargeInteger::operator^=(const LargeInteger& arg)
    {
        low ^= arg.low;
        high ^= arg.high;
        return *this;
    }

    LargeInteger::WrappedT LargeInteger::GetLow() const
    {
        return low;
    }

    LargeInteger::WrappedT LargeInteger::GetHigh() const
    {
        return high;
    }

    String LargeInteger::ToString() const
    {
        std::string ret;
        LargeInteger hold(*this);
        while (hold > 0)
        {
            LargeInteger mod(hold % 10);
            ret.insert(0, std::to_string(static_cast<unsigned int>((mod).low)));
            hold /= 10;
        }

        return ret;
    }

    void LargeInteger::FromString(const String& arg)
    {
        size_t count = 0;
        WrappedT selector = 1;
        WrappedT *focused = &low;
        for (auto loop = arg.rbegin(); loop != arg.rend(); ++loop)
        {
            if (count > digitCount)
                focused = &high;

            WrappedT digit = 0;
            switch (*loop)
            {
            case '0':
                digit = 0;
                break;
            case '1':
                digit = 1;
                break;
            case '2':
                digit = 2;
                break;
            case '3':
                digit = 3;
                break;
            case '4':
                digit = 4;
                break;
            case '5':
                digit = 5;
                break;
            case '6':
                digit = 6;
                break;
            case '7':
                digit = 7;
                break;
            case '8':
                digit = 8;
                break;
            case '9':
                digit = 9;
                break;
            }

            *focused += (digit * selector);
            selector *= 10;
            ++count;
        }
    }

    void LargeInteger::AddImpl(const LargeInteger& left, const LargeInteger& right, LargeInteger& result)
    {
        result = left;
        const WrappedT oldLow = result.low;
        result.low += right.low;
        result.high += right.high;
        if (result.low < oldLow)
            ++result.high;
    }

    void LargeInteger::SubtractImpl(const LargeInteger& left, const LargeInteger& right, LargeInteger& result)
    {
        result = left;
        const WrappedT oldLow = result.low;
        result.low -= right.low;
        result.high -= right.high;
        if (result.low > oldLow)
            --result.high;
    }

    void LargeInteger::MultiplyImpl(const LargeInteger& multiplicand, const LargeInteger& multiplier, LargeInteger& result)
    {
        LargeInteger a = multiplicand;
        LargeInteger b = multiplier;
        if (a < b)
        {
            auto prev = a;
            a = b;
            b = prev;
        }

        while (b != LargeInteger(0U))
        {
            // If b is odd, add a to result
            if (b & LargeInteger(1U))
                result += a;

            a <<= LargeInteger(1U);
            b >>= LargeInteger(1U);
        }
    }

    void LargeInteger::DivideImpl(const LargeInteger& dividend, const LargeInteger& divisor, LargeInteger& quotient, LargeInteger& remainder)
    {
        if (divisor == LargeInteger(0U))
            throw DivideByZeroException();
        else if (dividend == LargeInteger(0U))
        {
            quotient = 0U;
            remainder = 0U;
            return;
        }
        else if (divisor == LargeInteger(1U) || dividend == divisor)
        {
            quotient = 1U;
            remainder = 0U;
            return;
        }
        else if (divisor > dividend)
        {
            quotient = 0U;
            remainder = dividend;
            return;
        }

        LargeInteger num = dividend;
        LargeInteger den = divisor;
        quotient = 0U;

        const LargeInteger one(1U);
        LargeInteger current(one);
        while (den <= num)
        {
            den <<= one;
            current <<= one;
        }

        den >>= one;
        current >>= one;

        while (current > LargeInteger(0U))
        {
            if (num >= den)
            {
                num -= den;
                quotient |= current;
            }

            current >>= one;
            den >>= one;
        }

        remainder = num;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::LargeInteger, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.low);
        archive(object.high);
    }

    void Scribe<::Atmos::LargeInteger, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}