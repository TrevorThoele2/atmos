#pragma once

#include "BoundedNumber.h"
#include "WrapMath.h"

#include "Serialization.h"

namespace Atmos
{
    template<class T>
    class DynamicBoundedNumberWrap
    {
    private:
        typedef DynamicBoundedNumber<T> WrappedT;
    public:
        typedef typename WrappedT::ValueT ValueT;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        WrappedT wrapped;
    public:
        DynamicBoundedNumberWrap();
        explicit DynamicBoundedNumberWrap(ValueT number, ValueT upperBound, ValueT lowerBound);

        void Set(ValueT setTo);
        void SetByPercentage(double set);
        void Add(ValueT amount = 1);
        void Subtract(ValueT amount = 1);

        bool IsAtUpper() const;
        bool IsAtLower() const;
        ValueT GetUpperBound() const;
        ValueT GetLowerBound() const;

        ValueT Get() const;

        bool operator==(const DynamicBoundedNumberWrap &arg) const;
        bool operator!=(const DynamicBoundedNumberWrap &arg) const;
        bool operator<(const DynamicBoundedNumberWrap &arg) const;
        bool operator<=(const DynamicBoundedNumberWrap &arg) const;
        bool operator>(const DynamicBoundedNumberWrap &arg) const;
        bool operator>=(const DynamicBoundedNumberWrap &arg) const;

        friend bool operator==(const DynamicBoundedNumberWrap &arg, ValueT value) { return arg.Get() == value; }
        friend bool operator!=(const DynamicBoundedNumberWrap &arg, ValueT value) { return !(arg == value); }
        friend bool operator<(const DynamicBoundedNumberWrap &arg, ValueT value) { return arg.Get() < value; }
        friend bool operator<=(const DynamicBoundedNumberWrap &arg, ValueT value) { return arg < value || arg == value; }
        friend bool operator>(const DynamicBoundedNumberWrap &arg, ValueT value) { return arg.Get() > value; }
        friend bool operator>=(const DynamicBoundedNumberWrap &arg, ValueT value) { return arg > value || arg == value; }

        explicit operator ValueT() const;

        DynamicBoundedNumberWrap& operator=(const DynamicBoundedNumberWrap &arg);
        DynamicBoundedNumberWrap& operator=(ValueT number);
        DynamicBoundedNumberWrap& operator++();
        DynamicBoundedNumberWrap& operator--();
        DynamicBoundedNumberWrap operator++(int);
        DynamicBoundedNumberWrap operator--(int);
        DynamicBoundedNumberWrap operator+(const DynamicBoundedNumberWrap &arg) const;
        friend DynamicBoundedNumberWrap operator+(const DynamicBoundedNumberWrap &arg, ValueT number)
        {
            DynamicBoundedNumber<T> ret(arg);
            ret.Add(number);
            return ret;
        }

        DynamicBoundedNumberWrap& operator+=(const DynamicBoundedNumberWrap &arg);
        friend DynamicBoundedNumberWrap& operator+=(DynamicBoundedNumberWrap &arg, ValueT number)
        {
            arg.Add(number);
            return arg;
        }

        DynamicBoundedNumberWrap operator-(const DynamicBoundedNumberWrap &arg) const;
        friend DynamicBoundedNumberWrap operator-(const DynamicBoundedNumberWrap &arg, ValueT number)
        {
            DynamicBoundedNumber<T> ret(arg);
            ret.Subtract(arg.Get());
            return ret;
        }

        DynamicBoundedNumberWrap& operator-=(const DynamicBoundedNumberWrap &arg);
        friend DynamicBoundedNumberWrap& operator-=(DynamicBoundedNumberWrap &arg, ValueT number)
        {
            arg.Subtract(number);
            return arg;
        }

        void SetLowerBound(ValueT setTo);
        void SetUpperBound(ValueT setTo);
        void IncrementLowerBound(ValueT amount);
        void IncrementUpperBound(ValueT amount);
        void DecrementLowerBound(ValueT amount);
        void DecrementUpperBound(ValueT amount);
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE_BINARY(DynamicBoundedNumberWrap<T>)
    {
        scribe(wrapped);
    }

    template<class T>
    DynamicBoundedNumberWrap<T>::DynamicBoundedNumberWrap()
    {}

    template<class T>
    DynamicBoundedNumberWrap<T>::DynamicBoundedNumberWrap(ValueT number, ValueT upperBound, ValueT lowerBound) : wrapped(number, upperBound, lowerBound)
    {}

    template<class T>
    void DynamicBoundedNumberWrap<T>::Set(ValueT setTo)
    {
        if (setTo > Get())
            Add(setTo - Get());
        else
            Subtract(Get() - setTo);
    }

    template<class T>
    void DynamicBoundedNumberWrap<T>::SetByPercentage(double set)
    {
        wrapped.SetByPercentage(set % 100.0);
    }

    template<class T>
    void DynamicBoundedNumberWrap<T>::Add(ValueT amount)
    {
        Set(WrapAdd(Get(), amount, GetLowerBound(), GetUpperBound()));
    }

    template<class T>
    void DynamicBoundedNumberWrap<T>::Subtract(ValueT amount)
    {
        Set(WrapSubtract(Get(), amount, GetLowerBound(), GetUpperBound()));
    }

    template<class T>
    bool DynamicBoundedNumberWrap<T>::IsAtUpper() const
    {
        return wrapped.IsAtUpper();
    }

    template<class T>
    bool DynamicBoundedNumberWrap<T>::IsAtLower() const
    {
        wrapped.IsAtLower();
    }

    template<class T>
    typename DynamicBoundedNumberWrap<T>::ValueT DynamicBoundedNumberWrap<T>::GetUpperBound() const
    {
        return wrapped.GetUpperBound();
    }

    template<class T>
    typename DynamicBoundedNumberWrap<T>::ValueT DynamicBoundedNumberWrap<T>::GetLowerBound() const
    {
        return wrapped.GetLowerBound();
    }

    template<class T>
    typename DynamicBoundedNumberWrap<T>::ValueT DynamicBoundedNumberWrap<T>::Get() const
    {
        return wrapped.Get();
    }

    template<class T>
    bool DynamicBoundedNumberWrap<T>::operator==(const DynamicBoundedNumberWrap &arg) const
    {
        return wrapped == arg.wrapped;
    }

    template<class T>
    bool DynamicBoundedNumberWrap<T>::operator!=(const DynamicBoundedNumberWrap &arg) const
    {
        return wrapped != arg.wrapped;
    }

    template<class T>
    bool DynamicBoundedNumberWrap<T>::operator<(const DynamicBoundedNumberWrap &arg) const
    {
        return wrapped < arg.wrapped;
    }

    template<class T>
    bool DynamicBoundedNumberWrap<T>::operator<=(const DynamicBoundedNumberWrap &arg) const
    {
        return wrapped <= arg.wrapped;
    }

    template<class T>
    bool DynamicBoundedNumberWrap<T>::operator>(const DynamicBoundedNumberWrap &arg) const
    {
        return wrapped > arg.wrapped;
    }

    template<class T>
    bool DynamicBoundedNumberWrap<T>::operator>=(const DynamicBoundedNumberWrap &arg) const
    {
        return wrapped >= arg.wrapped;
    }

    template<class T>
    DynamicBoundedNumberWrap<T>::operator ValueT() const
    {
        return Get();
    }

    template<class T>
    DynamicBoundedNumberWrap<T>& DynamicBoundedNumberWrap<T>::operator=(const DynamicBoundedNumberWrap &arg)
    {
        wrapped = arg.wrapped;
        return *this;
    }

    template<class T>
    DynamicBoundedNumberWrap<T>& DynamicBoundedNumberWrap<T>::operator=(ValueT number)
    {
        Set(number);
        return *this;
    }

    template<class T>
    DynamicBoundedNumberWrap<T>& DynamicBoundedNumberWrap<T>::operator++()
    {
        Add();
        return *this;
    }

    template<class T>
    DynamicBoundedNumberWrap<T>& DynamicBoundedNumberWrap<T>::operator--()
    {
        Subtract();
        return *this;
    }

    template<class T>
    DynamicBoundedNumberWrap<T> DynamicBoundedNumberWrap<T>::operator++(int)
    {
        DynamicBoundedNumberWrap hold(*this);
        ++hold;
        return hold;
    }

    template<class T>
    DynamicBoundedNumberWrap<T> DynamicBoundedNumberWrap<T>::operator--(int)
    {
        DynamicBoundedNumberWrap hold(*this);
        --hold;
        return hold;
    }

    template<class T>
    DynamicBoundedNumberWrap<T> DynamicBoundedNumberWrap<T>::operator+(const DynamicBoundedNumberWrap &arg) const
    {
        DynamicBoundedNumberWrap hold(*this);
        hold.Add(arg.Get());
        return hold;
    }

    template<class T>
    DynamicBoundedNumberWrap<T>& DynamicBoundedNumberWrap<T>::operator+=(const DynamicBoundedNumberWrap &arg)
    {
        Add(arg.Get()));
        return *this;
    }

    template<class T>
    DynamicBoundedNumberWrap<T> DynamicBoundedNumberWrap<T>::operator-(const DynamicBoundedNumberWrap &arg) const
    {
        DynamicBoundedNumberWrap hold(*this);
        hold.Subtract(arg.Get());
        return hold;
    }

    template<class T>
    DynamicBoundedNumberWrap<T>& DynamicBoundedNumberWrap<T>::operator-=(const DynamicBoundedNumberWrap &arg)
    {
        Subtract(arg.Get());
        return *this;
    }

    template<class T>
    void DynamicBoundedNumberWrap<T>::SetLowerBound(ValueT setTo)
    {
        wrapped.SetLowerBound(setTo);
    }

    template<class T>
    void DynamicBoundedNumberWrap<T>::SetUpperBound(ValueT setTo)
    {
        wrapped.SetUpperBound(setTo);
    }

    template<class T>
    void DynamicBoundedNumberWrap<T>::IncrementLowerBound(ValueT amount)
    {
        ValueT distance = Get() - GetLowerBound();
        wrapped.IncrementLowerBound(amount);
        if (Get() < GetLowerBound())
            Set(GetUpperBound() - distance);
    }

    template<class T>
    void DynamicBoundedNumberWrap<T>::IncrementUpperBound(ValueT amount)
    {
        wrapped.IncrementUpperBound(amount);
    }

    template<class T>
    void DynamicBoundedNumberWrap<T>::DecrementLowerBound(ValueT amount)
    {
        wrapped.DecrementLowerBound(amount);
    }

    template<class T>
    void DynamicBoundedNumberWrap<T>::DecrementUpperBound(ValueT amount)
    {
        ValueT distance = GetUpperBound() - Get();
        wrapped.DecrementUpperBound(amount);
        if (Get > GetUpperBound())
            Set(distance + GetLowerBound());
    }
}