#pragma once

#include "PropertyCore.h"

namespace Atmos
{
    template<class T>
    class Property;

    template<class T>
    class ArithmeticPropertySlotInterface : public virtual PropertyCore<T>
    {
    public:
        virtual ~ArithmeticPropertySlotInterface() = 0;

        Property<T>& operator++()
        {
            SetInternal(GetInternal() + 1);
            return DowncastThis();
        }

        T operator++(int)
        {
            auto value = GetInternal();
            SetInternal(value + 1);
            return value;
        }

        Property<T>& operator--()
        {
            SetInternal(GetInternal() - 1);
            return DowncastThis();
        }

        T operator--(int)
        {
            auto value = GetInternal();
            SetInternal(value - 1);
            return value;
        }

        friend T operator+(const ArithmeticPropertySlotInterface& left, T right)
        {
            auto value = left.GetInternal();
            return value + right;
        }

        friend T operator+(T left, const ArithmeticPropertySlotInterface& right)
        {
            return left + right.GetInternal();
        }

        friend T operator+(const ArithmeticPropertySlotInterface& left, const ArithmeticPropertySlotInterface& right)
        {
            return left.GetInternal() + right.GetInternal();
        }

        friend T operator-(const ArithmeticPropertySlotInterface& left, T right)
        {
            auto value = left.GetInternal();
            return value - right;
        }

        friend T operator-(T left, const ArithmeticPropertySlotInterface& right)
        {
            return left - right.GetInternal();
        }

        friend T operator-(const ArithmeticPropertySlotInterface& left, const ArithmeticPropertySlotInterface& right)
        {
            return left.GetInternal() - right.GetInternal();
        }

        friend T operator*(const ArithmeticPropertySlotInterface& left, T right)
        {
            auto value = left.GetInternal();
            return value * right;
        }

        friend T operator*(T left, const ArithmeticPropertySlotInterface& right)
        {
            return left * right.GetInternal();
        }

        friend T operator*(const ArithmeticPropertySlotInterface& left, const ArithmeticPropertySlotInterface& right)
        {
            return left.GetInternal() * right.GetInternal();
        }

        friend T operator/(const ArithmeticPropertySlotInterface& left, T right)
        {
            auto value = left.GetInternal();
            return value / right;
        }

        friend T operator/(T left, const ArithmeticPropertySlotInterface& right)
        {
            return left / right.GetInternal();
        }

        friend T operator/(const ArithmeticPropertySlotInterface& left, const ArithmeticPropertySlotInterface& right)
        {
            return left.GetInternal() / right.GetInternal();
        }

        friend Property<T>& operator+=(ArithmeticPropertySlotInterface& left, T right)
        {
            left.SetInternal(left.GetInternal() + right);
            return Downcast(left);
        }

        friend T& operator+=(T& left, const ArithmeticPropertySlotInterface& right)
        {
            left += right.GetInternal();
            return left;
        }

        friend Property<T>& operator-=(ArithmeticPropertySlotInterface& left, T right)
        {
            left.SetInternal(left.GetInternal() - right);
            return Downcast(left);
        }

        friend T& operator-=(T& left, const ArithmeticPropertySlotInterface& right)
        {
            left -= right.GetInternal();
            return left;
        }

        friend Property<T>& operator*=(ArithmeticPropertySlotInterface& left, T right)
        {
            left.SetInternal(left.GetInternal() * right);
            return Downcast(left);
        }

        friend T& operator*=(T& left, const ArithmeticPropertySlotInterface& right)
        {
            left *= right.GetInternal();
            return left;
        }

        friend Property<T>& operator/=(ArithmeticPropertySlotInterface& left, T right)
        {
            left.SetInternal(left.GetInternal() / right);
            return Downcast(left);
        }

        friend T& operator/=(T& left, const ArithmeticPropertySlotInterface& right)
        {
            left /= right.GetInternal();
            return left;
        }
    protected:
        using PropertyCore<T>::GetInternal;
        using PropertyCore<T>::SetInternal;
    private:
        Property<T>& DowncastThis();
        const Property<T>& DowncastThis() const;

        static Property<T>& Downcast(ArithmeticPropertySlotInterface& cast);
        static const Property<T>& Downcast(const ArithmeticPropertySlotInterface& cast);
    };

    template<class T>
    ArithmeticPropertySlotInterface<T>::~ArithmeticPropertySlotInterface()
    {}

    template<class T>
    Property<T>& ArithmeticPropertySlotInterface<T>::DowncastThis()
    {
        return dynamic_cast<Property<T>&>(*this);
    }

    template<class T>
    const Property<T>& ArithmeticPropertySlotInterface<T>::DowncastThis() const
    {
        return dynamic_cast<const Property<T>&>(*this);
    }

    template<class T>
    Property<T>& ArithmeticPropertySlotInterface<T>::Downcast(ArithmeticPropertySlotInterface& cast)
    {
        return dynamic_cast<Property<T>&>(cast);
    }

    template<class T>
    const Property<T>& ArithmeticPropertySlotInterface<T>::Downcast(const ArithmeticPropertySlotInterface& cast)
    {
        return dynamic_cast<const Property<T>&>(cast);
    }
}