
#pragma once

#include "Overflow.h"
#include "Math.h"
#include "DivideByZeroException.h"

#include "Serialization.h"

namespace Atmos
{
    template<class T>
    class BoundedNumberBase
    {
    public:
        typedef T ValueT;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT number;

        virtual ValueT UpperBoundImpl() const = 0;
        virtual ValueT LowerBoundImpl() const = 0;
    public:
        BoundedNumberBase(ValueT number);
        bool operator==(const BoundedNumberBase &arg) const;

        operator ValueT() const;

        void Set(ValueT setTo);
        void SetByPercentage(double set);
        void Add(ValueT amount = 1);
        void Subtract(ValueT amount = 1);
        void Multiply(ValueT amount);
        void Divide(ValueT amount);

        bool IsAtUpper() const;
        bool IsAtLower() const;
        ValueT UpperBound() const;
        ValueT LowerBound() const;

        ValueT Value() const;
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE_BINARY(BoundedNumberBase<T>)
    {
        scribe(number);
    }

    template<class T>
    BoundedNumberBase<T>::BoundedNumberBase(ValueT number) : number(number)
    {}

    template<class T>
    bool BoundedNumberBase<T>::operator==(const BoundedNumberBase &arg) const
    {
        return number == arg.number;
    }

    template<class T>
    BoundedNumberBase<T>::operator ValueT() const
    {
        return Value();
    }

    template<class T>
    void BoundedNumberBase<T>::Set(ValueT setTo)
    {
        auto upperBound = UpperBound();
        if (setTo > upperBound)
        {
            number = upperBound;
            return;
        }

        auto lowerBound = LowerBound();
        if (setTo < lowerBound)
        {
            number = lowerBound;
            return;
        }

        number = setTo;
    }

    template<class T>
    void BoundedNumberBase<T>::SetByPercentage(double set)
    {
        auto lowerBound = LowerBound();
        auto upperBound = UpperBound();
        if (set < 0.0)
            Set(lowerBound);
        else if (set > 1.0)
            Set(upperBound);
        else
            Set(static_cast<T>(floor((static_cast<float>((upperBound - lowerBound)) * set) + 0.5)));
    }

    template<class T>
    void BoundedNumberBase<T>::Add(ValueT amount)
    {
        ValueT upperBound = UpperBound();
        ValueT lowerBound = LowerBound();

        if (OverflowAdd(number, amount, upperBound))
            number = upperBound;
        else if (UnderflowAdd(number, amount, lowerBound))
            number = lowerBound;
        else
            number += amount;
    }

    template<class T>
    void BoundedNumberBase<T>::Subtract(ValueT amount)
    {
        ValueT lowerBound = LowerBound();
        ValueT upperBound = UpperBound();

        if (OverflowSubtract(number, amount, upperBound))
            number = upperBound;
        else if (UnderflowSubtract(number, amount, lowerBound))
            number = lowerBound;
        else
            number -= amount;
    }

    template<class T>
    void BoundedNumberBase<T>::Multiply(ValueT amount)
    {
        if (number == 0 || amount == 0)
        {
            number = 0;
            return;
        }

        ValueT lowerBound = LowerBound();
        ValueT upperBound = UpperBound();

        if (OverflowMultiply(number, amount, upperBound))
            number = upperBound;
        else if (UnderflowMultiply(number, amount, lowerBound))
            number = lowerBound;
        else
            number *= amount;
    }

    template<class T>
    void BoundedNumberBase<T>::Divide(ValueT amount)
    {
        if (amount == 0)
            throw DivideByZeroException();

        ValueT lowerBound = LowerBound();
        ValueT upperBound = UpperBound();

        if (OverflowMultiply(number, amount, upperBound))
            number = upperBound;
        else if (UnderflowMultiply(number, amount, lowerBound))
            number = lowerBound;
        else
            number /= amount;
    }

    template<class T>
    bool BoundedNumberBase<T>::IsAtUpper() const
    {
        return number == UpperBound();
    }

    template<class T>
    bool BoundedNumberBase<T>::IsAtLower() const
    {
        return number == LowerBound();
    }

    template<class T>
    typename BoundedNumberBase<T>::ValueT BoundedNumberBase<T>::UpperBound() const
    {
        return UpperBoundImpl();
    }

    template<class T>
    typename BoundedNumberBase<T>::ValueT BoundedNumberBase<T>::LowerBound() const
    {
        return LowerBoundImpl();
    }

    template<class T>
    typename BoundedNumberBase<T>::ValueT BoundedNumberBase<T>::Value() const
    {
        return number;
    }

    template<class T>
    class DynamicBoundedNumber : public BoundedNumberBase<T>
    {
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT lowerBound;
        ValueT upperBound;

        ValueT LowerBoundImpl() const override;
        ValueT UpperBoundImpl() const override;
    public:
        DynamicBoundedNumber();
        explicit DynamicBoundedNumber(ValueT number, ValueT upperBound, ValueT lowerBound);
        bool operator==(const DynamicBoundedNumber &arg) const;
        bool operator!=(const DynamicBoundedNumber &arg) const;
        bool operator<(const DynamicBoundedNumber &arg) const;
        bool operator<=(const DynamicBoundedNumber &arg) const;
        bool operator>(const DynamicBoundedNumber &arg) const;
        bool operator>=(const DynamicBoundedNumber &arg) const;

        friend bool operator==(const DynamicBoundedNumber &arg, ValueT value) { return arg.Value() == value; }
        friend bool operator!=(const DynamicBoundedNumber &arg, ValueT value) { return !(arg == value); }
        friend bool operator<(const DynamicBoundedNumber &arg, ValueT value) { return arg.Value() < value; }
        friend bool operator<=(const DynamicBoundedNumber &arg, ValueT value) { return arg < value || arg == value; }
        friend bool operator>(const DynamicBoundedNumber &arg, ValueT value) { return arg.Value() > value; }
        friend bool operator>=(const DynamicBoundedNumber &arg, ValueT value) { return arg > value || arg == value; }

        explicit operator ValueT() const;

        DynamicBoundedNumber& operator=(const DynamicBoundedNumber &arg);
        DynamicBoundedNumber& operator=(ValueT number);
        DynamicBoundedNumber& operator++();
        DynamicBoundedNumber& operator--();
        DynamicBoundedNumber operator++(int);
        DynamicBoundedNumber operator--(int);
        DynamicBoundedNumber operator+(const DynamicBoundedNumber &arg) const;
        friend DynamicBoundedNumber operator+(const DynamicBoundedNumber &arg, ValueT number)
        {
            DynamicBoundedNumber<T> ret(arg);
            ret.Add(number);
            return ret;
        }

        DynamicBoundedNumber& operator+=(const DynamicBoundedNumber &arg);
        friend DynamicBoundedNumber& operator+=(DynamicBoundedNumber &arg, ValueT number)
        {
            arg.Add(number);
            return arg;
        }

        DynamicBoundedNumber operator-(const DynamicBoundedNumber &arg) const;
        friend DynamicBoundedNumber operator-(const DynamicBoundedNumber &arg, ValueT number)
        {
            DynamicBoundedNumber<T> ret(arg);
            ret.Subtract(arg.Value());
            return ret;
        }

        DynamicBoundedNumber& operator-=(const DynamicBoundedNumber &arg);
        friend DynamicBoundedNumber& operator-=(DynamicBoundedNumber &arg, ValueT number)
        {
            arg.Subtract(number);
            return arg;
        }

        DynamicBoundedNumber operator*(const DynamicBoundedNumber &arg) const;
        friend DynamicBoundedNumber operator*(const DynamicBoundedNumber &arg, ValueT number)
        {
            DynamicBoundedNumber<T> ret(arg);
            ret.Multiply(number);
            return ret;
        }

        DynamicBoundedNumber& operator*=(const DynamicBoundedNumber &arg);
        friend DynamicBoundedNumber& operator*=(DynamicBoundedNumber &arg, ValueT number)
        {
            arg.Multiply(number);
            return arg;
        }

        DynamicBoundedNumber operator/(const DynamicBoundedNumber &arg) const;
        friend DynamicBoundedNumber operator/(const DynamicBoundedNumber &arg, ValueT number)
        {
            DynamicBoundedNumber<T> ret(*this);
            ret.Divide(number);
            return ret;
        }

        DynamicBoundedNumber& operator/=(const DynamicBoundedNumber &arg);
        friend DynamicBoundedNumber& operator/=(DynamicBoundedNumber &arg, ValueT number)
        {
            arg.Divide(number);
            return *this;
        }

        void SetLowerBound(ValueT setTo);
        void SetUpperBound(ValueT setTo);
        void IncrementLowerBound(ValueT amount);
        void IncrementUpperBound(ValueT amount);
        void DecrementLowerBound(ValueT amount);
        void DecrementUpperBound(ValueT amount);
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE_BINARY(DynamicBoundedNumber<T>)
    {
        ::Inscription::BaseSerialize<BoundedNumberBase<T>>(scribe, *this);
    }

    template<class T>
    typename DynamicBoundedNumber<T>::ValueT DynamicBoundedNumber<T>::LowerBoundImpl() const
    {
        return lowerBound;
    }

    template<class T>
    typename DynamicBoundedNumber<T>::ValueT DynamicBoundedNumber<T>::UpperBoundImpl() const
    {
        return upperBound;
    }

    template<class T>
    DynamicBoundedNumber<T>::DynamicBoundedNumber() : BoundedNumberBase(0), upperBound(0), lowerBound(0)
    {}

    template<class T>
    DynamicBoundedNumber<T>::DynamicBoundedNumber(ValueT number, ValueT upperBound, ValueT lowerBound) : BoundedNumberBase(number), upperBound(upperBound), lowerBound(lowerBound)
    {}

    template<class T>
    bool DynamicBoundedNumber<T>::operator==(const DynamicBoundedNumber &arg) const
    {
        return BoundedNumberBase::operator==(arg);
    }

    template<class T>
    bool DynamicBoundedNumber<T>::operator!=(const DynamicBoundedNumber &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    bool DynamicBoundedNumber<T>::operator<(const DynamicBoundedNumber &arg) const
    {
        return Value() < arg.Value();
    }

    template<class T>
    bool DynamicBoundedNumber<T>::operator<=(const DynamicBoundedNumber &arg) const
    {
        return Value() <= arg.Value();
    }

    template<class T>
    bool DynamicBoundedNumber<T>::operator>(const DynamicBoundedNumber &arg) const
    {
        return Value() > arg.Value();
    }

    template<class T>
    bool DynamicBoundedNumber<T>::operator>=(const DynamicBoundedNumber &arg) const
    {
        return Value() >= arg.Value();
    }

    template<class T>
    DynamicBoundedNumber<T>::operator ValueT() const
    {
        return Value();
    }

    template<class T>
    DynamicBoundedNumber<T>& DynamicBoundedNumber<T>::operator=(const DynamicBoundedNumber &arg)
    {
        Set(arg.Value());
        return *this;
    }

    template<class T>
    DynamicBoundedNumber<T>& DynamicBoundedNumber<T>::operator=(ValueT number)
    {
        Set(number);
        return *this;
    }

    template<class T>
    DynamicBoundedNumber<T>& DynamicBoundedNumber<T>::operator++()
    {
        Add();
        return *this;
    }

    template<class T>
    DynamicBoundedNumber<T>& DynamicBoundedNumber<T>::operator--()
    {
        Subtract();
        return *this;
    }

    template<class T>
    DynamicBoundedNumber<T> DynamicBoundedNumber<T>::operator++(int)
    {
        DynamicBoundedNumber hold(*this);
        Add();
        return hold;
    }

    template<class T>
    DynamicBoundedNumber<T> DynamicBoundedNumber<T>::operator--(int)
    {
        DynamicBoundedNumber hold(*this);
        Decrement();
        return hold;
    }

    template<class T>
    DynamicBoundedNumber<T> DynamicBoundedNumber<T>::operator+(const DynamicBoundedNumber &arg) const
    {
        DynamicBoundedNumber<T> ret(*this);
        ret.Add(arg.Value());
        return ret;
    }

    template<class T>
    DynamicBoundedNumber<T>& DynamicBoundedNumber<T>::operator+=(const DynamicBoundedNumber &arg)
    {
        Add(arg.Value());
        return *this;
    }

    template<class T>
    DynamicBoundedNumber<T> DynamicBoundedNumber<T>::operator-(const DynamicBoundedNumber &arg) const
    {
        DynamicBoundedNumber<T> ret(*this);
        ret.Subtract(arg.Value());
        return ret;
    }

    template<class T>
    DynamicBoundedNumber<T>& DynamicBoundedNumber<T>::operator-=(const DynamicBoundedNumber &arg)
    {
        Subtract(arg.Value());
        return *this;
    }

    template<class T>
    DynamicBoundedNumber<T> DynamicBoundedNumber<T>::operator*(const DynamicBoundedNumber<T> &arg) const
    {
        DynamicBoundedNumber<T> ret(*this);
        ret.Multiply(arg.Value());
        return ret;
    }

    template<class T>
    DynamicBoundedNumber<T>& DynamicBoundedNumber<T>::operator*=(const DynamicBoundedNumber<T> &arg)
    {
        Multiply(arg.Value());
        return *this;
    }

    template<class T>
    DynamicBoundedNumber<T> DynamicBoundedNumber<T>::operator/(const DynamicBoundedNumber<T> &arg) const
    {
        DynamicBoundedNumber<T> ret(*this);
        ret.Divide(arg.Value());
        return ret;
    }

    template<class T>
    DynamicBoundedNumber<T>& DynamicBoundedNumber<T>::operator/=(const DynamicBoundedNumber<T> &arg)
    {
        Divide(arg.Value());
        return *this;
    }

    template<class T>
    void DynamicBoundedNumber<T>::SetLowerBound(ValueT setTo)
    {
        if (setTo > upperBound)
            return;

        lowerBound = setTo;
        if (Value() < lowerBound)
            Set(lowerBound);
    }

    template<class T>
    void DynamicBoundedNumber<T>::SetUpperBound(ValueT setTo)
    {
        if (setTo < lowerBound)
            return;

        upperBound = setTo;
        if (Value() > upperBound)
            Set(upperBound);
    }

    template<class T>
    void DynamicBoundedNumber<T>::IncrementLowerBound(ValueT amount)
    {
        SetLowerBound(lowerBound + amount);
    }

    template<class T>
    void DynamicBoundedNumber<T>::IncrementUpperBound(ValueT amount)
    {
        SetUpperBound(upperBound + amount);
    }

    template<class T>
    void DynamicBoundedNumber<T>::DecrementLowerBound(ValueT amount)
    {
        SetLowerBound(lowerBound + amount);
    }

    template<class T>
    void DynamicBoundedNumber<T>::DecrementUpperBound(ValueT amount)
    {
        SetUpperBound(upperBound + amount);
    }

    namespace Detail
    {
        template<class T>
        struct BoundedNumberFloatTransform
        {
            typedef T Type;
        };

        template<>
		struct BoundedNumberFloatTransform<float>
        {
            typedef unsigned int Type;
        };

        template<>
		struct BoundedNumberFloatTransform<double>
        {
            typedef unsigned long long Type;
        };
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    class StaticBoundedNumber : public BoundedNumberBase<T>
    {
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT UpperBoundImpl() const override;
        ValueT LowerBoundImpl() const override;
    public:
        StaticBoundedNumber();
        explicit StaticBoundedNumber(ValueT value);
        bool operator==(const StaticBoundedNumber &arg) const;
        bool operator!=(const StaticBoundedNumber &arg) const;
        bool operator<(const StaticBoundedNumber &arg) const;
        bool operator<=(const StaticBoundedNumber &arg) const;
        bool operator>(const StaticBoundedNumber &arg) const;
        bool operator>=(const StaticBoundedNumber &arg) const;

        friend bool operator==(const StaticBoundedNumber &arg, ValueT value) { return arg.Value() == value; }
        friend bool operator!=(const StaticBoundedNumber &arg, ValueT value) { return !(arg == value); }
        friend bool operator<(const StaticBoundedNumber &arg, ValueT value) { return arg.Value() < value; }
        friend bool operator<=(const StaticBoundedNumber &arg, ValueT value) { return arg < value || arg == value; }
        friend bool operator>(const StaticBoundedNumber &arg, ValueT value) { return arg.Value() > value; }
        friend bool operator>=(const StaticBoundedNumber &arg, ValueT value) { return arg > value || arg == value; }

        explicit operator ValueT() const;

        StaticBoundedNumber& operator=(const StaticBoundedNumber &arg);
        StaticBoundedNumber& operator=(ValueT arg);
        StaticBoundedNumber& operator++();
        StaticBoundedNumber& operator--();
        StaticBoundedNumber operator++(int);
        StaticBoundedNumber operator--(int);
        StaticBoundedNumber operator+(const StaticBoundedNumber &arg) const;
        friend StaticBoundedNumber operator+(const StaticBoundedNumber &arg, ValueT number)
        {
            StaticBoundedNumber<T, lowerBound, upperBound> ret(arg);
            ret.Add(number);
            return ret;
        }

        StaticBoundedNumber& operator+=(const StaticBoundedNumber &arg);
        friend StaticBoundedNumber& operator+=(StaticBoundedNumber &arg, ValueT number)
        {
            arg.Add(number);
            return arg;
        }

        StaticBoundedNumber operator-(const StaticBoundedNumber &arg) const;
        friend StaticBoundedNumber operator-(const StaticBoundedNumber &arg, ValueT number)
        {
            StaticBoundedNumber<T, lowerBound, upperBound> ret(arg);
            ret.Subtract(number);
            return ret;
        }

        StaticBoundedNumber& operator-=(const StaticBoundedNumber &arg);
        friend StaticBoundedNumber& operator-=(StaticBoundedNumber &arg, ValueT number)
        {
            arg.Subtract(number);
            return arg;
        }

        StaticBoundedNumber operator*(const StaticBoundedNumber &arg) const;
        friend StaticBoundedNumber operator*(const StaticBoundedNumber &arg, ValueT number)
        {
            StaticBoundedNumber<T, lowerBound, upperBound> ret(arg);
            ret.Multiply(number);
            return ret;
        }

        StaticBoundedNumber& operator*=(const StaticBoundedNumber &arg);
        friend StaticBoundedNumber& operator*=(StaticBoundedNumber &arg, ValueT number)
        {
            arg.Multiply(number);
            return arg;
        }

        StaticBoundedNumber operator/(const StaticBoundedNumber &arg) const;
        friend StaticBoundedNumber operator/(const StaticBoundedNumber &arg, ValueT number)
        {
            StaticBoundedNumber<T, lowerBound, upperBound> ret(arg);
            ret.Divide(number);
            return ret;
        }

        StaticBoundedNumber& operator/=(const StaticBoundedNumber &arg);
        friend StaticBoundedNumber& operator/=(StaticBoundedNumber &arg, ValueT number)
        {
            arg.Divide(number);
            return arg;
        }

        static constexpr ValueT UpperBoundStatic();
        static constexpr ValueT LowerBoundStatic();
    };

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    void StaticBoundedNumber<T, lowerBound, upperBound>::Serialize(::Inscription::Scribe &scribe)
    {
        ::Inscription::BaseSerialize<BoundedNumberBase<T>>(scribe, *this);
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    typename StaticBoundedNumber<T, lowerBound, upperBound>::ValueT StaticBoundedNumber<T, lowerBound, upperBound>::UpperBoundImpl() const
    {
        return upperBound;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    typename StaticBoundedNumber<T, lowerBound, upperBound>::ValueT StaticBoundedNumber<T, lowerBound, upperBound>::LowerBoundImpl() const
    {
        return lowerBound;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>::StaticBoundedNumber() : BoundedNumberBase(0)
    {}

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>::StaticBoundedNumber(ValueT number) : BoundedNumberBase(number)
    {}

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    bool StaticBoundedNumber<T, lowerBound, upperBound>::operator==(const StaticBoundedNumber &arg) const
    {
        return BoundedNumberBase::operator==(arg);
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    bool StaticBoundedNumber<T, lowerBound, upperBound>::operator!=(const StaticBoundedNumber &arg) const
    {
        return !(*this == arg);
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    bool StaticBoundedNumber<T, lowerBound, upperBound>::operator<(const StaticBoundedNumber &arg) const
    {
        return Value() < arg.Value();
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    bool StaticBoundedNumber<T, lowerBound, upperBound>::operator<=(const StaticBoundedNumber &arg) const
    {
        return Value() <= arg.Value();
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    bool StaticBoundedNumber<T, lowerBound, upperBound>::operator>(const StaticBoundedNumber &arg) const
    {
        return Value() > arg.Value();
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    bool StaticBoundedNumber<T, lowerBound, upperBound>::operator>=(const StaticBoundedNumber &arg) const
    {
        return Value() >= arg.Value();
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>::operator ValueT() const
    {
        return Value();
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>& StaticBoundedNumber<T, lowerBound, upperBound>::operator=(const StaticBoundedNumber &arg)
    {
        Set(arg.Value());
        return *this;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>& StaticBoundedNumber<T, lowerBound, upperBound>::operator=(ValueT arg)
    {
        Set(arg);
        return *this;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>& StaticBoundedNumber<T, lowerBound, upperBound>::operator++()
    {
        Add();
        return *this;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>& StaticBoundedNumber<T, lowerBound, upperBound>::operator--()
    {
        Subtract();
        return *this;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound> StaticBoundedNumber<T, lowerBound, upperBound>::operator++(int)
    {
        StaticBoundedNumber hold(*this);
        Add();
        return hold;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound> StaticBoundedNumber<T, lowerBound, upperBound>::operator--(int)
    {
        StaticBoundedNumber hold(*this);
        Subtract();
        return hold;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound> StaticBoundedNumber<T, lowerBound, upperBound>::operator+(const StaticBoundedNumber &arg) const
    {
        StaticBoundedNumber<T, lowerBound, upperBound> ret(*this);
        ret.Add(arg.Value());
        return ret;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>& StaticBoundedNumber<T, lowerBound, upperBound>::operator+=(const StaticBoundedNumber &arg)
    {
        Add(arg.Value());
        return *this;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound> StaticBoundedNumber<T, lowerBound, upperBound>::operator-(const StaticBoundedNumber<T, lowerBound, upperBound> &arg) const
    {
        StaticBoundedNumber<T, lowerBound, upperBound> ret(*this);
        ret.Subtract(arg.Value());
        return ret;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>& StaticBoundedNumber<T, lowerBound, upperBound>::operator-=(const StaticBoundedNumber &arg)
    {
        Subtract(arg.Value());
        return *this;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound> StaticBoundedNumber<T, lowerBound, upperBound>::operator*(const StaticBoundedNumber &arg) const
    {
        StaticBoundedNumber<T, lowerBound, upperBound> ret(*this);
        ret.Multiply(arg.Value());
        return ret;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>& StaticBoundedNumber<T, lowerBound, upperBound>::operator*=(const StaticBoundedNumber &arg)
    {
        Multiply(arg.Value());
        return *this;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound> StaticBoundedNumber<T, lowerBound, upperBound>::operator/(const StaticBoundedNumber &arg) const
    {
        StaticBoundedNumber<T, lowerBound, upperBound> ret(*this);
        ret.Divide(arg.Value());
        return ret;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    StaticBoundedNumber<T, lowerBound, upperBound>& StaticBoundedNumber<T, lowerBound, upperBound>::operator/=(const StaticBoundedNumber &arg)
    {
        Divide(arg.Value());
        return *this;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    typename constexpr StaticBoundedNumber<T, lowerBound, upperBound>::ValueT StaticBoundedNumber<T, lowerBound, upperBound>::UpperBoundStatic()
    {
        return upperBound;
    }

    template<class T, typename Detail::BoundedNumberFloatTransform<T>::Type lowerBound, typename Detail::BoundedNumberFloatTransform<T>::Type upperBound>
    typename constexpr StaticBoundedNumber<T, lowerBound, upperBound>::ValueT StaticBoundedNumber<T, lowerBound, upperBound>::LowerBoundStatic()
    {
        return lowerBound;
    }
}