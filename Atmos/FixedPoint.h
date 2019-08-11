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
    public:
        typedef size_t Value;
    public:
        Value value;
    public:
        explicit constexpr RadixPoint(Value value = 0) : value(value)
        {}

        RadixPoint& operator=(Value arg);

        bool operator==(const RadixPoint& arg) const;
        bool operator!=(const RadixPoint& arg) const;

        explicit operator Value() const;
        Value Get() const;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    class FixedPoint
    {
    public:
        typedef T Value;
        typedef size_t DigitCount;
        typedef RadixPoint Radix;

        constexpr static DigitCount maxDigits = NumericLimits<Value>::digits10();
    public:
        class Split
        {
        private:
            Value MakeValue() const;
            friend FixedPoint;
        public:
            typedef typename FixedPoint::Value IPart;
            typedef typename FixedPoint::Value FPart;
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
            static FPart AdjustF(Value value, Radix radixPoint);
        };
    public:
        constexpr FixedPoint(Radix radixPoint = GetDefaultRadixPoint()) : value(0), radixPoint(radixPoint) {}
        template<class U>
        explicit FixedPoint(U i, Radix radixPoint = GetDefaultRadixPoint());
        FixedPoint(const Split &split);
        explicit FixedPoint(Value i, Value f, Radix radixPoint = GetDefaultRadixPoint());
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

        void SetRawValue(Value set);
        Value GetRawValue() const;
        void Floor();
        void Ceiling();
        // If manipulateValue == false, then this won't change the value
        // (can be used to move the decimal point without potentially losing value)
        FixedPoint& SetRadixPoint(Radix set, bool manipulateValue = true);
        Radix GetRadixPoint() const;

        String ToString() const;
        void FromString(const String &arg);

        static Value GetMax();
        static Value GetMin();
        static Value GetLowest();

        Split SplitValue() const;
        static Split SplitValue(Value split);

        static constexpr Radix GetDefaultRadixPoint();
    private:
        Value value;
        Radix radixPoint;

        static void AssertRadixPoint(Radix proposed);

        template<class U, typename std::enable_if<std::is_floating_point<U>::value, int>::type = 0>
        static Value GetValueFromArg(U arg, Radix radixPoint);
        template<class U, typename std::enable_if<!std::is_floating_point<U>::value, int>::type = 0>
        static Value GetValueFromArg(U arg, Radix radixPoint);

        template<class U>
        static Value BreakFloatingPoint(U u, Radix radixPoint);
        template<class U>
        static U FabricateFloatingPoint(Value val, Radix radixPoint);

        static Value GetInflectionPointStatic(Radix radixPoint);
        Value GetInflectionPoint() const;

        Value SplitValueIPart() const;
        Value SplitValueFPart() const;
        static Split SplitValueStatic(Value value, Radix radixPoint);

        static DigitCount GetDigitCount(Value value);

        static inline Value Add(Value left, Value right);
        static inline Value Subtract(Value left, Value right);
        static inline Value Multiply(Value left, Value right, RadixPoint radixPointRight);
        // Only need the right radix point; the result will be using the left radix point
        static inline Value Divide(Value left, Value right, RadixPoint radixPointRight);
        static inline Value Modulo(Value left, Value right);
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    inline typename FixedPoint<T>::Value FixedPoint<T>::Split::MakeValue() const
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
    typename FixedPoint<T>::Split::FPart FixedPoint<T>::Split::AdjustF(Value value, Radix radixPoint)
    {
        return (value * FixedPoint::GetInflectionPointStatic(Radix(radixPoint.Get() - GetDigitCount(value))));
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
    FixedPoint<T>::FixedPoint(Value i, Value f, Radix radixPoint) : value(i * GetInflectionPointStatic(radixPoint) + f), radixPoint(radixPoint)
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
    void FixedPoint<T>::SetRawValue(Value set)
    {
        value = set;
    }

    template<class T>
    typename FixedPoint<T>::Value FixedPoint<T>::GetRawValue() const
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
            Value normalizer = 1;
            for (; relativeRadix > 0; --relativeRadix)
                normalizer *= Value(10);
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
        Value hold = value;
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
    typename FixedPoint<T>::Value FixedPoint<T>::GetMax()
    {
        Value value = 0;
        Value significantDigit = 1;
        for (DigitCount loop = 0; loop < maxDigits; ++loop)
        {
            value += 9 * significantDigit;
            significantDigit *= 10;
        }

        return value;
    }

    template<class T>
    typename FixedPoint<T>::Value FixedPoint<T>::GetMin()
    {
        if (NumericLimits<T>::is_signed)
            return Value(-1);
        else
            return Value(1);
    }

    template<class T>
    typename FixedPoint<T>::Value FixedPoint<T>::GetLowest()
    {
        if (NumericLimits<T>::is_signed)
        {
            Value value = 0;
            Value significantDigit = 1;
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
    typename FixedPoint<T>::Split FixedPoint<T>::SplitValue(Value split)
    {
        return FixedPoint(split).SplitValue();
    }

    template<class T>
    constexpr typename FixedPoint<T>::Radix FixedPoint<T>::GetDefaultRadixPoint()
    {
        return Radix(NumericLimits<Value>::digits10() / 2);
    }

    template<class T>
    inline void FixedPoint<T>::AssertRadixPoint(Radix proposed)
    {
        ATMOS_ASSERT_MESSAGE(proposed.Get() <= FixedPoint::maxDigits, "The radix point must be within the digit values.");
    }

    template<class T>
    template<class U, typename std::enable_if<std::is_floating_point<U>::value, int>::type>
    typename FixedPoint<T>::Value FixedPoint<T>::GetValueFromArg(U arg, Radix radixPoint)
    {
        return BreakFloatingPoint(arg, radixPoint);
    }

    template<class T>
    template<class U, typename std::enable_if<!std::is_floating_point<U>::value, int>::type>
    typename FixedPoint<T>::Value FixedPoint<T>::GetValueFromArg(U arg, Radix radixPoint)
    {
        return arg * GetInflectionPointStatic(radixPoint);
    }

    template<class T>
    template<class U>
    typename FixedPoint<T>::Value FixedPoint<T>::BreakFloatingPoint(U u, Radix radixPoint)
    {
        Value val = 0;
        Value selector = 1;

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
    U FixedPoint<T>::FabricateFloatingPoint(Value val, Radix radixPoint)
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
    typename FixedPoint<T>::Value FixedPoint<T>::GetInflectionPointStatic(Radix radixPoint)
    {
        Value ret = Value(1);
        for (size_t loop = 0; loop < radixPoint.Get(); ++loop)
            ret *= Value(10);

        return ret;
    }

    template<class T>
    typename FixedPoint<T>::Value FixedPoint<T>::GetInflectionPoint() const
    {
        Value ret = Value(1);
        for (size_t loop = 0; loop < radixPoint.Get(); ++loop)
            ret *= Value(10);

        return ret;
    }

    template<class T>
    inline typename FixedPoint<T>::Value FixedPoint<T>::SplitValueIPart() const
    {
        return value / GetInflectionPoint();
    }

    template<class T>
    inline typename FixedPoint<T>::Value FixedPoint<T>::SplitValueFPart() const
    {
        return value - ((value / GetInflectionPoint()) * GetInflectionPoint());
    }

    template<class T>
    inline typename FixedPoint<T>::Split FixedPoint<T>::SplitValueStatic(Value value, Radix radixPoint)
    {
        Value i = value / GetInflectionPointStatic(radixPoint);
        return Split(i, value - i * GetInflectionPointStatic(radixPoint));
    }

    template<class T>
    inline typename FixedPoint<T>::DigitCount FixedPoint<T>::GetDigitCount(Value value)
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
    inline typename FixedPoint<T>::Value FixedPoint<T>::Add(Value left, Value right)
    {
        return left + right;
    }

    template<class T>
    inline typename FixedPoint<T>::Value FixedPoint<T>::Subtract(Value left, Value right)
    {
        return left - right;
    }

    template<class T>
    inline typename FixedPoint<T>::Value FixedPoint<T>::Multiply(Value left, Value right, RadixPoint radixPointRight)
    {
        const Value multiplicand = left;
        Value multiplier = right;
        Value digitPlacement = 10;
        Value result = 0;

        DigitCount count = 0;
        const Value radixDifference(Power(10, radixPointRight.Get()));
        while (multiplier != 0 || count <= radixPointRight.Get())
        {
            const Value digit = multiplier % 10;
            Value multiplication(multiplicand * digit);
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
    inline typename FixedPoint<T>::Value FixedPoint<T>::Divide(Value left, Value right, RadixPoint radixPointRight)
    {
        if (right == 0)
            throw DivideByZeroException();

        Value dividend = left;
        const Value divisor = right;
        const DigitCount dividendDigits(GetDigitCount(dividend));
        Value result = 0;
        Value selector = Power(Value(10), dividendDigits - 1);
        Value digit = 0;
        Value answer = 0;
        Value running = 0;

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
    inline typename FixedPoint<T>::Value FixedPoint<T>::Modulo(Value left, Value right)
    {
        if (right == 0)
            throw DivideByZeroException();

        return left - (left / right * right);
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

namespace Inscription
{
    template<>
    class Scribe<::Atmos::RadixPoint, BinaryArchive> : public CompositeScribe<::Atmos::RadixPoint, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };

    template<class T>
    class Scribe<::Atmos::FixedPoint<T>, BinaryArchive> : public CompositeScribe<::Atmos::FixedPoint<T>, BinaryArchive>
    {
    private:
        using BaseT = typename CompositeScribe<::Atmos::FixedPoint<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
        using BaseT::Construct;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.value);
            archive(object.radixPoint);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }

        using BaseT::DoBasicConstruction;
    };
}