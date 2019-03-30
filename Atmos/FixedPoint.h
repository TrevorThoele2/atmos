#pragma once

// For floating point conversions
#include <sstream>
// For floating point conversions
#include <iomanip>

#include <type_traits>

#include "MathUtility.h"
#include "NumericLimits.h"

#include "DivideByZeroException.h"

#include "String.h"
#include "Assert.h"

#include "Serialization.h"
#include <Inscription/Scribe.h>

namespace Atmos
{
    class RadixPoint
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        typedef size_t ValueT;
        ValueT value;
        explicit constexpr RadixPoint(ValueT value = 0) : value(value) {}
        RadixPoint& operator=(ValueT arg);
        bool operator==(const RadixPoint &arg) const;
        bool operator!=(const RadixPoint &arg) const;
        explicit operator ValueT() const;
        ValueT Get() const;
    };

    template<class T>
    class FixedPoint
    {
    public:
        typedef T ValueT;
        typedef size_t DigitCount;
        typedef RadixPoint Radix;

        constexpr static DigitCount maxDigits = NumericLimits<ValueT>::digits10();
    public:
        class Split
        {
        private:
            ValueT MakeValue() const;
            friend FixedPoint;
        public:
            typedef typename FixedPoint::ValueT IPart;
            typedef typename FixedPoint::ValueT FPart;
            typedef typename FixedPoint::Radix Radix;
            IPart i;
            FPart f;
            Radix radixPoint;
            Split();
            Split(IPart i, FPart f, Radix radixPoint = GetDefaultRadixPoint());
            Split(const FixedPoint &arg);
            bool operator==(const Split &arg) const;
            bool operator!=(const Split &arg) const;
            bool operator<(const Split &arg) const;
            bool operator<=(const Split &arg) const;
            bool operator>(const Split &arg) const;
            bool operator>=(const Split &arg) const;

            Split operator+(const Split &arg) const;
            Split operator-(const Split &arg) const;
            Split operator*(const Split &arg) const;
            Split operator/(const Split &arg) const;
            Split operator%(const Split &arg) const;
            Split& operator+=(const Split &arg);
            Split& operator-=(const Split &arg);
            Split& operator*=(const Split &arg);
            Split& operator/=(const Split &arg);
            Split& operator%=(const Split &arg);

            // A split is negative if any of its parts are negative
            bool IsNegative() const;
            // Returns the value with it pushed into the correct position for the fractional part of a split
            // The position is calculated from the right side of the decimal
            // 1, 0 = equivalent of 0.1
            // 1, 1 = equivalent of 0.01
            static FPart AdjustF(ValueT value, Radix radixPoint);
        };
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT value;
        Radix radixPoint;

        static void AssertRadixPoint(Radix proposed);

        template<class U, typename std::enable_if<std::is_floating_point<U>::value, int>::type = 0>
        static ValueT GetValueFromArg(U arg, Radix radixPoint);
        template<class U, typename std::enable_if<!std::is_floating_point<U>::value, int>::type = 0>
        static ValueT GetValueFromArg(U arg, Radix radixPoint);

        template<class U>
        static ValueT BreakFloatingPoint(U u, Radix radixPoint);
        template<class U>
        static U FabricateFloatingPoint(ValueT val, Radix radixPoint);

        static ValueT GetInflectionPointStatic(Radix radixPoint);
        ValueT GetInflectionPoint() const;

        ValueT SplitValueIPart() const;
        ValueT SplitValueFPart() const;
        static Split SplitValueStatic(ValueT value, Radix radixPoint);

        static DigitCount GetDigitCount(ValueT value);

        static inline ValueT Add(ValueT left, ValueT right);
        static inline ValueT Subtract(ValueT left, ValueT right);
        static inline ValueT Multiply(ValueT left, ValueT right, RadixPoint radixPointRight);
        // Only need the right radix point; the result will be using the left radix point
        static inline ValueT Divide(ValueT left, ValueT right, RadixPoint radixPointRight);
        static inline ValueT Modulo(ValueT left, ValueT right);
    public:
        constexpr FixedPoint(Radix radixPoint = GetDefaultRadixPoint()) : value(0), radixPoint(radixPoint) {}
        template<class U>
        explicit FixedPoint(U i, Radix radixPoint = GetDefaultRadixPoint());
        FixedPoint(const Split &split);
        explicit FixedPoint(ValueT i, ValueT f, Radix radixPoint = GetDefaultRadixPoint());
        FixedPoint(const FixedPoint &arg) = default;
        FixedPoint& operator=(const Split &split);
        FixedPoint& operator=(const FixedPoint &arg) = default;

        bool operator==(const FixedPoint &arg) const;
        bool operator==(const Split &arg) const;
        bool operator!=(const FixedPoint &arg) const;
        bool operator!=(const Split &arg) const;
        bool operator<(const FixedPoint &arg) const;
        bool operator<(const Split &arg) const;
        bool operator<=(const FixedPoint &arg) const;
        bool operator<=(const Split &arg) const;
        bool operator>(const FixedPoint &arg) const;
        bool operator>(const Split &arg) const;
        bool operator>=(const FixedPoint &arg) const;
        bool operator>=(const Split &arg) const;

        FixedPoint operator+(const FixedPoint &arg) const;
        FixedPoint operator+(const Split &arg) const;
        FixedPoint operator-(const FixedPoint &arg) const;
        FixedPoint operator-(const Split &arg) const;
        FixedPoint operator*(const FixedPoint &arg) const;
        FixedPoint operator*(const Split &arg) const;
        FixedPoint operator/(const FixedPoint &arg) const;
        FixedPoint operator/(const Split &arg) const;
        FixedPoint operator%(const FixedPoint &arg) const;
        FixedPoint operator%(const Split &arg) const;
        FixedPoint& operator+=(const FixedPoint &arg);
        FixedPoint& operator+=(const Split &arg);
        FixedPoint& operator-=(const FixedPoint &arg);
        FixedPoint& operator-=(const Split &arg);
        FixedPoint& operator*=(const FixedPoint &arg);
        FixedPoint& operator*=(const Split &arg);
        FixedPoint& operator/=(const FixedPoint &arg);
        FixedPoint& operator/=(const Split &arg);
        FixedPoint& operator%=(const FixedPoint &arg);
        FixedPoint& operator%=(const Split &arg);

        FixedPoint& operator++();
        FixedPoint& operator--();
        FixedPoint operator++(int);
        FixedPoint operator--(int);

        FixedPoint operator-() const;

        explicit operator char() const;
        explicit operator unsigned char() const;
        explicit operator short() const;
        explicit operator unsigned short() const;
        explicit operator int() const;
        explicit operator unsigned int() const;
        explicit operator long long() const;
        explicit operator unsigned long long() const;
        explicit operator float() const;
        explicit operator double() const;
        explicit operator Split() const;

        void SetRawValue(ValueT set);
        ValueT GetRawValue() const;
        void Floor();
        void Ceiling();
        // If manipulateValue == false, then this won't change the value
        // (can be used to move the decimal point without potentially losing value)
        FixedPoint& SetRadixPoint(Radix set, bool manipulateValue = true);
        Radix GetRadixPoint() const;

        String ToString() const;
        void FromString(const String &arg);

        static ValueT GetMax();
        static ValueT GetMin();
        static ValueT GetLowest();

        Split SplitValue() const;
        static Split SplitValue(ValueT split);

        static constexpr Radix GetDefaultRadixPoint();
    };

    template<class T>
    inline typename FixedPoint<T>::ValueT FixedPoint<T>::Split::MakeValue() const
    {
        return i * FixedPoint::GetInflectionPointStatic(radixPoint) + f;
    }

    template<class T>
    FixedPoint<T>::Split::Split() : i(0), f(0), radixPoint(GetDefaultRadixPoint())
    {}

    template<class T>
    FixedPoint<T>::Split::Split(IPart i, FPart f, Radix radixPoint) : i(i), f(f), radixPoint(radixPoint)
    {
        FixedPoint::AssertRadixPoint(radixPoint);
    }

    template<class T>
    FixedPoint<T>::Split::Split(const FixedPoint &arg) : i(arg.value / FixedPoint::GetInflectionPointStatic(arg.radixPoint)), f(arg.value - (arg.value / FixedPoint::GetInflectionPointStatic(arg.radixPoint)) * FixedPoint::GetInflectionPointStatic(arg.radixPoint))
    {}

    template<class T>
    bool FixedPoint<T>::Split::operator==(const Split &arg) const
    {
        return i == arg.i && f == arg.f && radixPoint == arg.radixPoint;
    }

    template<class T>
    bool FixedPoint<T>::Split::operator!=(const Split &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    bool FixedPoint<T>::Split::operator<(const Split &arg) const
    {
        return (i == arg.i) ? f < arg.f : i < arg.i;
    }

    template<class T>
    bool FixedPoint<T>::Split::operator<=(const Split &arg) const
    {
        return *this < arg || *this == arg;
    }

    template<class T>
    bool FixedPoint<T>::Split::operator>(const Split &arg) const
    {
        return (i == arg.i) ? f > arg.f : i > arg.i;
    }

    template<class T>
    bool FixedPoint<T>::Split::operator>=(const Split &arg) const
    {
        return *this > arg || *this == arg;
    }

    template<class T>
    typename FixedPoint<T>::Split FixedPoint<T>::Split::operator+(const Split &arg) const
    {
        return FixedPoint::SplitValueStatic(FixedPoint::Add(MakeValue(), arg.MakeValue()), radixPoint);
    }

    template<class T>
    typename FixedPoint<T>::Split FixedPoint<T>::Split::operator-(const Split &arg) const
    {
        return FixedPoint::SplitValueStatic(FixedPoint::Subtract(MakeValue(), arg.MakeValue()), radixPoint);
    }

    template<class T>
    typename FixedPoint<T>::Split FixedPoint<T>::Split::operator*(const Split &arg) const
    {
        return FixedPoint::SplitValueStatic(FixedPoint::Multiply(MakeValue(), arg.MakeValue(), arg.radixPoint), radixPoint);
    }

    template<class T>
    typename FixedPoint<T>::Split FixedPoint<T>::Split::operator/(const Split &arg) const
    {
        return FixedPoint::SplitValueStatic(FixedPoint::Divide(MakeValue(), arg.MakeValue(), arg.radixPoint), radixPoint);
    }

    template<class T>
    typename FixedPoint<T>::Split FixedPoint<T>::Split::operator%(const Split &arg) const
    {
        return FixedPoint::SplitValueStatic(FixedPoint::Modulo(MakeValue(), arg.MakeValue()), radixPoint);
    }

    template<class T>
    typename FixedPoint<T>::Split& FixedPoint<T>::Split::operator+=(const Split &arg)
    {
        *this = FixedPoint::SplitValueStatic(FixedPoint::Add(MakeValue(), arg.MakeValue()), radixPoint);
        return *this;
    }

    template<class T>
    typename FixedPoint<T>::Split& FixedPoint<T>::Split::operator-=(const Split &arg)
    {
        *this = FixedPoint::SplitValueStatic(FixedPoint::Subtract(MakeValue(), arg.MakeValue()), radixPoint);
        return *this;
    }

    template<class T>
    typename FixedPoint<T>::Split& FixedPoint<T>::Split::operator*=(const Split &arg)
    {
        *this = FixedPoint::SplitValueStatic(FixedPoint::Multiply(MakeValue(), arg.MakeValue(), arg.radixPoint), radixPoint);
        return *this;
    }

    template<class T>
    typename FixedPoint<T>::Split& FixedPoint<T>::Split::operator/=(const Split &arg)
    {
        *this = FixedPoint::SplitValueStatic(FixedPoint::Divide(MakeValue(), arg.MakeValue(), arg.radixPoint), radixPoint);
        return *this;
    }

    template<class T>
    typename FixedPoint<T>::Split& FixedPoint<T>::Split::operator%=(const Split &arg)
    {
        *this = FixedPoint::SplitValueStatic(FixedPoint::Modulo(MakeValue(), arg.MakeValue()), radixPoint);
        return *this;
    }

    template<class T>
    bool FixedPoint<T>::Split::IsNegative() const
    {
        return i < 0 || f < 0;
    }

    template<class T>
    typename FixedPoint<T>::Split::FPart FixedPoint<T>::Split::AdjustF(ValueT value, Radix radixPoint)
    {
        return (value * FixedPoint::GetInflectionPointStatic(Radix(radixPoint.Get() - GetDigitCount(value))));
    }

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(FixedPoint<T>)
    {
        scribe(value);
        scribe(radixPoint);
    }

    template<class T>
    inline void FixedPoint<T>::AssertRadixPoint(Radix proposed)
    {
        ATMOS_ASSERT_MESSAGE(proposed.Get() <= FixedPoint::maxDigits, "The radix point must be within the digit values.");
    }

    template<class T>
    template<class U, typename std::enable_if<std::is_floating_point<U>::value, int>::type>
    typename FixedPoint<T>::ValueT FixedPoint<T>::GetValueFromArg(U arg, Radix radixPoint)
    {
        return BreakFloatingPoint(arg, radixPoint);
    }

    template<class T>
    template<class U, typename std::enable_if<!std::is_floating_point<U>::value, int>::type>
    typename FixedPoint<T>::ValueT FixedPoint<T>::GetValueFromArg(U arg, Radix radixPoint)
    {
        return arg * GetInflectionPointStatic(radixPoint);
    }

    template<class T>
    template<class U>
    typename FixedPoint<T>::ValueT FixedPoint<T>::BreakFloatingPoint(U u, Radix radixPoint)
    {
        ValueT val = 0;
        ValueT selector = 1;

        std::ostringstream out;
        out << std::setprecision(radixPoint.Get()) << std::fixed << u;

        bool negative = false;
        size_t loop = out.str().size();
        while (loop-- > 0)
        {
            auto substr = out.str().substr(loop, 1);
            if (substr == "-")
            {
                negative = true;
                continue;
            }
            if (substr == ".")
                continue;

            val += (strtoll(substr.c_str(), nullptr, 0) * selector);
            selector *= 10;
        }

        if (negative)
            val *= -1;

        return val;
    }

    template<class T>
    template<class U>
    U FixedPoint<T>::FabricateFloatingPoint(ValueT val, Radix radixPoint)
    {
        U ret = 0;
        U otherInflectionPoint = 1;
        for (size_t loop = 0; loop != radixPoint.Get(); ++loop)
            otherInflectionPoint /= 10;

        while (val != 0)
        {
            ret += (val % 10) * otherInflectionPoint;
            otherInflectionPoint *= 10;
            val /= 10;
        }

        return ret;
    }

    template<class T>
    typename FixedPoint<T>::ValueT FixedPoint<T>::GetInflectionPointStatic(Radix radixPoint)
    {
        ValueT ret = ValueT(1);
        for (size_t loop = 0; loop < radixPoint.Get(); ++loop)
            ret *= ValueT(10);

        return ret;
    }

    template<class T>
    typename FixedPoint<T>::ValueT FixedPoint<T>::GetInflectionPoint() const
    {
        ValueT ret = ValueT(1);
        for (size_t loop = 0; loop < radixPoint.Get(); ++loop)
            ret *= ValueT(10);

        return ret;
    }

    template<class T>
    inline typename FixedPoint<T>::ValueT FixedPoint<T>::SplitValueIPart() const
    {
        return value / GetInflectionPoint();
    }

    template<class T>
    inline typename FixedPoint<T>::ValueT FixedPoint<T>::SplitValueFPart() const
    {
        return value - ((value / GetInflectionPoint()) * GetInflectionPoint());
    }

    template<class T>
    inline typename FixedPoint<T>::Split FixedPoint<T>::SplitValueStatic(ValueT value, Radix radixPoint)
    {
        ValueT i = value / GetInflectionPointStatic(radixPoint);
        return Split(i, value - i * GetInflectionPointStatic(radixPoint));
    }

    template<class T>
    inline typename FixedPoint<T>::DigitCount FixedPoint<T>::GetDigitCount(ValueT value)
    {
        if (value == 0)
            return 1;

        DigitCount count = 0;
        while (value != 0)
        {
            value /= 10;
            ++count;
        }

        return count;
    }

    template<class T>
    inline typename FixedPoint<T>::ValueT FixedPoint<T>::Add(ValueT left, ValueT right)
    {
        return left + right;
    }

    template<class T>
    inline typename FixedPoint<T>::ValueT FixedPoint<T>::Subtract(ValueT left, ValueT right)
    {
        return left - right;
    }

    template<class T>
    inline typename FixedPoint<T>::ValueT FixedPoint<T>::Multiply(ValueT left, ValueT right, RadixPoint radixPointRight)
    {
        const ValueT multiplicand = left;
        ValueT multiplier = right;
        ValueT digitPlacement = 10;
        ValueT result = 0;

        DigitCount count = 0;
        const ValueT radixDifference(Power(10, radixPointRight.Get()));
        while (multiplier != 0 || count <= radixPointRight.Get())
        {
            const ValueT digit = multiplier % 10;
            ValueT multiplication(multiplicand * digit);
            // If the count of digits is less than or equal to the radixPoint, shift result to the right
            // Else if the count of digits is greater than the radixPoint, move the newly added value to the left
            // Multiplication results in twice as many digits, and we're using the original data type so we can only store half the digits that come from multiplication
            if (count <= radixPointRight.Get())
                result /= 10;
            else
            {
                multiplication *= digitPlacement;
                digitPlacement *= 10;
            }

            ++count;
            result += multiplication;
            multiplier /= 10;
        }

        return result;
    }

    // Only need the right radix point; the result will be using the left radix point
    template<class T>
    inline typename FixedPoint<T>::ValueT FixedPoint<T>::Divide(ValueT left, ValueT right, RadixPoint radixPointRight)
    {
        if (right == 0)
            throw DivideByZeroException();

        ValueT dividend = left;
        const ValueT divisor = right;
        const DigitCount dividendDigits(GetDigitCount(dividend));
        ValueT result = 0;
        ValueT selector = Power(ValueT(10), dividendDigits - 1);
        ValueT digit = 0;
        ValueT answer = 0;
        ValueT running = 0;

        DigitCount count = 0;
        while (count <= dividendDigits + radixPointRight.Get() - 1)
        {
            if (selector == 0)
                digit = 0;
            else
                digit = dividend / selector;
            running += digit;
            answer = running / divisor;
            result *= 10;

            result += answer;
            running -= answer * divisor;

            dividend -= digit * selector;
            selector /= 10;
            running *= 10;

            ++count;
        }

        return result;
    }

    template<class T>
    inline typename FixedPoint<T>::ValueT FixedPoint<T>::Modulo(ValueT left, ValueT right)
    {
        if (right == 0)
            throw DivideByZeroException();

        return left - (left / right * right);
    }

    template<class T>
    template<class U>
    FixedPoint<T>::FixedPoint(U i, Radix radixPoint) : value(GetValueFromArg(i, radixPoint)), radixPoint(radixPoint)
    {
        FixedPoint::AssertRadixPoint(radixPoint);
    }

    template<class T>
    FixedPoint<T>::FixedPoint(const Split &split) : value(split.i * GetInflectionPointStatic(split.radixPoint) + ((!split.IsNegative()) ? split.f : ((split.f > 0) ? -split.f : split.f))), radixPoint(split.radixPoint)
    {
        FixedPoint::AssertRadixPoint(split.radixPoint);
    }

    template<class T>
    FixedPoint<T>::FixedPoint(ValueT i, ValueT f, Radix radixPoint) : value(i * GetInflectionPointStatic(radixPoint) + f), radixPoint(radixPoint)
    {
        FixedPoint::AssertRadixPoint(radixPoint);
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator=(const Split &split)
    {
        this->value = value;
        radixPoint = split.radixPoint;
        return *this;
    }

    template<class T>
    bool FixedPoint<T>::operator==(const FixedPoint &arg) const
    {
        return value == FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value;
    }

    template<class T>
    bool FixedPoint<T>::operator==(const Split &arg) const
    {
        return SplitValue() == arg;
    }

    template<class T>
    bool FixedPoint<T>::operator!=(const FixedPoint &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    bool FixedPoint<T>::operator!=(const Split &arg) const
    {
        return SplitValue() != arg;
    }

    template<class T>
    bool FixedPoint<T>::operator<(const FixedPoint &arg) const
    {
        return value < FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value;
    }

    template<class T>
    bool FixedPoint<T>::operator<(const Split &arg) const
    {
        return SplitValue() < arg;
    }

    template<class T>
    bool FixedPoint<T>::operator<=(const FixedPoint &arg) const
    {
        return value <= FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value;
    }

    template<class T>
    bool FixedPoint<T>::operator<=(const Split &arg) const
    {
        return SplitValue() <= arg;
    }

    template<class T>
    bool FixedPoint<T>::operator>(const FixedPoint &arg) const
    {
        return value > FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value;
    }

    template<class T>
    bool FixedPoint<T>::operator>(const Split &arg) const
    {
        return SplitValue() > arg;
    }

    template<class T>
    bool FixedPoint<T>::operator>=(const FixedPoint &arg) const
    {
        return value >= FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value;
    }

    template<class T>
    bool FixedPoint<T>::operator>=(const Split &arg) const
    {
        return SplitValue() >= arg;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator+(const FixedPoint &arg) const
    {
        FixedPoint ret;
        ret.value = Add(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return ret;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator+(const Split &arg) const
    {
        FixedPoint ret;
        ret.value = Add(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return ret;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator-(const FixedPoint &arg) const
    {
        FixedPoint ret;
        ret.value = Subtract(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return ret;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator-(const Split &arg) const
    {
        FixedPoint ret;
        ret.value = Subtract(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return ret;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator*(const FixedPoint &arg) const
    {
        FixedPoint ret;
        ret.value = Multiply(value, arg.value, arg.radixPoint);
        return ret;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator*(const Split &arg) const
    {
        FixedPoint ret;
        ret.value = Multiply(value, arg.MakeValue(), arg.radixPoint);
        return ret;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator/(const FixedPoint &arg) const
    {
        FixedPoint ret;
        ret.value = Divide(value, arg.value, arg.radixPoint);
        return ret;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator/(const Split &arg) const
    {
        FixedPoint ret;
        ret.value = Divide(value, arg.MakeValue(), arg.radixPoint);
        return ret;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator%(const FixedPoint &arg) const
    {
        FixedPoint ret;
        ret.value = Modulo(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return ret;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator%(const Split &arg) const
    {
        FixedPoint ret;
        ret.value = Modulo(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return ret;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator+=(const FixedPoint &arg)
    {
        value = Add(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator+=(const Split &arg)
    {
        value = Add(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator-=(const FixedPoint &arg)
    {
        value = Subtract(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator-=(const Split &arg)
    {
        value = Subtract(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator*=(const FixedPoint &arg)
    {
        value = Multiply(value, arg.value, arg.radixPoint);
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator*=(const Split &arg)
    {
        value = Multiply(value, arg.MakeValue(), arg.radixPoint);
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator/=(const FixedPoint &arg)
    {
        value = Divide(value, arg.value, arg.radixPoint);
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator/=(const Split &arg)
    {
        value = Divide(value, arg.MakeValue(), arg.radixPoint);
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator%=(const FixedPoint &arg)
    {
        value = Modulo(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator%=(const Split &arg)
    {
        value = Modulo(value, FixedPoint(arg).SetRadixPoint(GetRadixPoint()).value);
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator++()
    {
        value += GetInflectionPoint();
        return *this;
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::operator--()
    {
        value -= GetInflectionPoint();
        return *this;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator++(int)
    {
        FixedPoint hold(*this);
        value += GetInflectionPoint();
        return hold;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator--(int)
    {
        FixedPoint hold(*this);
        value -= GetInflectionPoint();
        return hold;
    }

    template<class T>
    FixedPoint<T> FixedPoint<T>::operator-() const
    {
        return FixedPoint(-value);
    }

    template<class T>
    FixedPoint<T>::operator char() const
    {
        return static_cast<char>(SplitValueIPart());
    }

    template<class T>
    FixedPoint<T>::operator unsigned char() const
    {
        return static_cast<unsigned char>(SplitValueIPart());
    }

    template<class T>
    FixedPoint<T>::operator short() const
    {
        return static_cast<short>(SplitValueIPart());
    }

    template<class T>
    FixedPoint<T>::operator unsigned short() const
    {
        return static_cast<unsigned short>(SplitValueIPart());
    }

    template<class T>
    FixedPoint<T>::operator int() const
    {
        return static_cast<int>(SplitValueIPart());
    }

    template<class T>
    FixedPoint<T>::operator unsigned int() const
    {
        return static_cast<unsigned int>(SplitValueIPart());
    }

    template<class T>
    FixedPoint<T>::operator long long() const
    {
        return static_cast<long long>(SplitValueIPart());
    }

    template<class T>
    FixedPoint<T>::operator unsigned long long() const
    {
        return static_cast<unsigned long long>(SplitValueIPart());
    }

    template<class T>
    FixedPoint<T>::operator float() const
    {
        return FabricateFloatingPoint<float>(value, radixPoint);
    }

    template<class T>
    FixedPoint<T>::operator double() const
    {
        return FabricateFloatingPoint<double>(value, radixPoint);
    }

    template<class T>
    FixedPoint<T>::operator Split() const
    {
        return SplitValue();
    }

    template<class T>
    void FixedPoint<T>::SetRawValue(ValueT set)
    {
        value = set;
    }

    template<class T>
    typename FixedPoint<T>::ValueT FixedPoint<T>::GetRawValue() const
    {
        return value;
    }

    template<class T>
    void FixedPoint<T>::Floor()
    {
        Split split = SplitValue();
        if (split.f != 0)
            value = (value >= 0) ? split.i * GetInflectionPoint() : (split.i - 1) * GetInflectionPoint();
    }

    template<class T>
    void FixedPoint<T>::Ceiling()
    {
        Split split = SplitValue();
        if (split.f != 0)
            value = (value >= 0) ? (split.i + 1) * GetInflectionPoint() : split.i * GetInflectionPoint();
    }

    template<class T>
    FixedPoint<T>& FixedPoint<T>::SetRadixPoint(Radix set, bool manipulateValue)
    {
        if (radixPoint == set)
            return *this;

        typedef int RelativeRadix;
        RelativeRadix relativeRadix = static_cast<RelativeRadix>(radixPoint.Get() - set.Get());
        radixPoint = set;
        FixedPoint::AssertRadixPoint(radixPoint);

        if (manipulateValue)
        {
            ValueT normalizer = 1;
            for (; relativeRadix > 0; --relativeRadix)
                normalizer *= ValueT(10);
            // POSITIVE relative radix point; move via multiplication (add zeros)
            if (relativeRadix > 0)
                value *= normalizer;
            else // NEGATIVE relative radix point; move via division (remove end digits)
                value /= normalizer;
        }

        return *this;
    }

    template<class T>
    typename FixedPoint<T>::Radix FixedPoint<T>::GetRadixPoint() const
    {
        return radixPoint;
    }

    template<class T>
    String FixedPoint<T>::ToString() const
    {
        String ret;
        ValueT hold = value;
        DigitCount count = 0;
        // Find out if negative, and if so, remove negativity (we'll add it in at the end)
        const bool negative = value < 0;
        if (negative)
            hold *= -1;
        // Go until the number is gone AND we've passed the radix point (showing the radix point in the string)
        while (hold > 0 || count <= radixPoint.Get())
        {
            if (count == radixPoint.Get())
                ret.insert(0, ".");

            ret.insert(0, ::Atmos::ToString(hold % 10));
            hold /= 10;
            ++count;
        }
        
        // Add in the negative if it needs to be there here
        if (negative)
            ret.insert(0, "-");

        return ret;
    }

    template<class T>
    void FixedPoint<T>::FromString(const String &arg)
    {
        //double fromString = ::Atmos::FromString<double>(arg);
        //value = BreakFloatingPoint(fromString, radixPoint);
    }

    template<class T>
    typename FixedPoint<T>::ValueT FixedPoint<T>::GetMax()
    {
        ValueT value = 0;
        ValueT significantDigit = 1;
        for (DigitCount loop = 0; loop < maxDigits; ++loop)
        {
            value += 9 * significantDigit;
            significantDigit *= 10;
        }

        return value;
    }

    template<class T>
    typename FixedPoint<T>::ValueT FixedPoint<T>::GetMin()
    {
        if (NumericLimits<T>::is_signed)
            return ValueT(-1);
        else
            return ValueT(1);
    }

    template<class T>
    typename FixedPoint<T>::ValueT FixedPoint<T>::GetLowest()
    {
        if (NumericLimits<T>::is_signed)
        {
            ValueT value = 0;
            ValueT significantDigit = 1;
            for (DigitCount loop = 0; loop < maxDigits; ++loop)
            {
                value += 9 * significantDigit;
                significantDigit *= 10;
            }

            value *= -1;
            return value;
        }

        return 0;
    }

    template<class T>
    inline typename FixedPoint<T>::Split FixedPoint<T>::SplitValue() const
    {
        return SplitValueStatic(value, radixPoint);
    }

    template<class T>
    typename FixedPoint<T>::Split FixedPoint<T>::SplitValue(ValueT split)
    {
        return FixedPoint(split).SplitValue();
    }

    template<class T>
    constexpr typename FixedPoint<T>::Radix FixedPoint<T>::GetDefaultRadixPoint()
    {
        return Radix(NumericLimits<ValueT>::digits10() / 2);
    }

    template<class T>
    FixedPoint<T> Floor(FixedPoint<T> arg)
    {
        arg.Floor();
        return arg;
    }

    template<class T>
    FixedPoint<T> Ceiling(FixedPoint<T> arg)
    {
        arg.Ceiling();
        return arg;
    }

    typedef FixedPoint<int> FixedPoint32;
    typedef FixedPoint<long long> FixedPoint64;

    template<class T>
    struct NumericLimits<FixedPoint<T>>
    {
        static constexpr FixedPoint<T> min()
        {
            return FixedPoint<T>::GetMin();
        }

        static constexpr FixedPoint<T> lowest()
        {
            return FixedPoint<T>::GetLowest();
        }

        static constexpr FixedPoint<T> max()
        {
            return FixedPoint<T>::GetMax();
        }

        static constexpr int digits10()
        {
            return NumericLimits<FixedPoint<T>>::digits10();
        }

        static constexpr bool is_signed = NumericLimits<FixedPoint<T>>::is_signed;
    };
}